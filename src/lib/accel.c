#include "../../header/accel.h"


void configure_pinmux(){
*SYSMGR_I2C0USEFPGA = 0;
*SYSMGR_GENERALIO7 = 1;
*SYSMGR_GENERALIO8 = 1;
}

volatile uint32_t *i2c0_regs;

// Função para abrir especificamente o /dev/mem e retornar o descritor
int open_physical() {
    int fd;

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        printf("Erro ao abrir /dev/mem");
        return -1;
    }

    return fd;
}

void close_physical(int fd) {
    close(fd);
}

// Função para mapear a região de memória do I2C0
int mmap_i2c0(int fd) {
    i2c0_regs = (volatile uint32_t *) mmap(NULL, I2C0_REG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, I2C0_BASE_ADDR);
    if (i2c0_regs == MAP_FAILED) {
        printf("Erro ao mapear /dev/mem");
        return -1;
    }
    return 0;
}

// Função para abrir e mapear /dev/mem
int open_and_mmap_dev_mem() {
    int fd = open_physical();
    if (fd == -1) {
        return -1;
    }

    if (mmap_i2c0(fd) == -1) {
        close(fd);
        return -1;
    }

    return fd;
}

// Função para fechar e liberar o mapeamento
void close_and_unmap_dev_mem(int fd) {
    if (i2c0_regs) {
        munmap((void *)i2c0_regs, I2C0_REG_SIZE);
    }
    close_physical(fd);
}

void write_register(volatile uint32_t *base, uint32_t offset, uint32_t value){
    // O endereço do registro é calculado somando o offset ao endereço base,
    // e depois dividido por 4, pois cada registro é de 4 bytes (32 bits).
    base[offset / 4] = value;
}

uint32_t read_register(volatile uint32_t *base, uint32_t offset) {
    // O endereço do registro é calculado somando o offset ao endereço base,
    // e depois dividido por 4, pois cada registro é de 4 bytes (32 bits).
    return base[offset / 4];
}

//Inicia a comunicação com o acelerometro via I2C
void I2C0_init(){
    write_register(i2c0_regs,I2C0_ENABLE, 0x02); //Aborta qualquer comunicação
    while ((read_register(i2c0_regs,I2C0_ENABLE_STATUS) & 0x1) == 1 )
    {} //Espera a comunicação voltar ao normal
    write_register(i2c0_regs,I2C0_CON, 0x65); //Configura HPS como master, fast mode (400kb/s) e 7 bit adressing
    write_register(i2c0_regs,I2C0_TAR, 0x53); // Coloca o acelerometro como target.
    
    write_register(i2c0_regs,I2C0_FS_SCL_HCNT, 60+30);  //
    write_register(i2c0_regs,I2C0_FS_SCL_LCNT, 130+30); //

    write_register(i2c0_regs,I2C0_ENABLE, 0x01); // Volta a habilitar a comunicação
    while ((read_register(i2c0_regs,I2C0_ENABLE_STATUS) & 0x1) == 0 )
    {}
    //Espera a comunicação ser estabelecida
}

bool test_communication() {

    uint8_t device_id;
    accel_reg_read(DEVID, &device_id);

    if (device_id == 0xE5){ // 0xE5 é o valor que deve estar em DEVID sempre!
        return true;
    }
    return false;
}

void accel_init() {
    accel_reg_write(DATA_FORMAT, 0x03 | 0x08); //Coloca o formato de dados em 0x03 (+-16g) e resolução completa
    accel_reg_write(BW_RATE, 0x0B); //Coloca o fluxo de saida de dados em 200Hz
    accel_reg_write(THRESH_ACT,0x02); // Calibra a detecção de movimento(62,5 mg por unidade em g (gravidade da Terra))
    accel_reg_write(ACT_INACT_CTL, 0xFF);	//Termina de habilitar as interrupções por Threshold.
    accel_reg_write(INT_ENABLE,0x80 | 0x10); // Permitir detecção de Data_ready e Activity

    accel_reg_write(POWER_CTL,0x00); //Seta o power para 0, parando
    accel_reg_write(POWER_CTL,0x08); //Inicia a medição.
}

void accel_calibrate(int average_index){

    int average_x = 0;
    int average_y = 0;
    int average_z = 0;
    int16_t XYZ[3];
    int8_t offset_x;
    int8_t offset_y;
    int8_t offset_z;

    accel_reg_write(POWER_CTL,0x00); // Coloca no modo de standby
    
    accel_reg_read(OFSX,(uint8_t *)&offset_x); //Le os 3 offsets
    accel_reg_read(OFSY,(uint8_t *)&offset_y);
    accel_reg_read(OFSZ,(uint8_t *)&offset_z);

    uint8_t saved_bw; 
    accel_reg_read(BW_RATE, &saved_bw); // Salva o bitrate antigo
    accel_reg_write(BW_RATE, 0x0a);

    uint8_t saved_dataF;
    accel_reg_read(DATA_FORMAT, &saved_dataF); // Salva o Data_format antigo
    accel_reg_write(DATA_FORMAT, 0x03 | 0x08);

    accel_reg_write(POWER_CTL,0x08); // Coloca no modo de medição
    int i = 0;
    while(i < average_index){
        if (accel_isDataReady()) // Checka se tem um dado para ser lido
        {
            accel_readXYZ(XYZ); // Le e adiciona a uma media os valores de X,Y,Z
            average_x += XYZ[0];
            average_y += XYZ[1];
            average_z += XYZ[2];
            i++;
        }  
    }
    average_x = ROUNDED_DIVISION(average_x,average_index); //Divide os valores de XYZ de forma que garanta o surgimento
    average_y = ROUNDED_DIVISION(average_y,average_index); //de um número inteiro e arredondado para cima.
    average_z = ROUNDED_DIVISION(average_z,average_index);

    accel_reg_write(POWER_CTL,0x00); //Volta para o modo Standby

    //printf("Average X=%d, Y=%d, Z=%d\n", average_x, average_y, average_z);

    // Ajusta o valor de offset_xyz aplicando uma correção baseada no valor médio de 'average_xyz'.
    offset_x += ROUNDED_DIVISION(0-average_x, 4); //
    offset_y += ROUNDED_DIVISION(0-average_y, 4);
    offset_z += ROUNDED_DIVISION(0-average_z, 4);
    accel_reg_write(OFSX,offset_x); //Aplica os novos offsets
    accel_reg_write(OFSY,offset_y);
    accel_reg_write(OFSZ,offset_z);

    //Volta a configuração para antes da calibração
    accel_reg_write(BW_RATE,saved_bw);
    accel_reg_write(DATA_FORMAT,saved_dataF);
    accel_reg_write(POWER_CTL,0x08);
}

//Checka se tem algum dado a ser lido
bool accel_isDataReady(){
    bool bReady = false;
    uint8_t data8;

    accel_reg_read(INT_SOURCE,&data8);
     if (data8 & (0x80))
        bReady = true;
    
    return bReady;
}

//Checka se houve uma movimentação acima do limiar configurado em Thresh_Act.
bool accel_hadActivity(){
    bool bReady = false;
    uint8_t data8;

    accel_reg_read(INT_SOURCE,&data8);
     if (data8 & (0x10))
        bReady = true;
    
    return bReady;
}

//Escreve no acelerometro
void accel_reg_write(uint8_t address, uint8_t value){
    
    write_register(i2c0_regs,I2C0_DATA_CMD, address + 0x400);//Manda um sinal de start
    write_register(i2c0_regs,I2C0_DATA_CMD, value); //Faz a escrita
}

//Ler no acelerometro
void accel_reg_read(uint8_t address, uint8_t *value){
    write_register(i2c0_regs,I2C0_DATA_CMD, address + 0x400); //Manda um sinal de start
    write_register(i2c0_regs,I2C0_DATA_CMD, 0x100); //Manda um pedido de leitura
    while (read_register(i2c0_regs,I2C0_RXFLR) == 0) //Checka o buffer esperando colocar um valor para leitura
    {}
    *value = (uint8_t)read_register(i2c0_regs,I2C0_DATA_CMD); //Faz a leitura e armazena em value
}

//Le os valores do acelerometro via I2C e coloca num array de 8bits
void I2C_readXYZ(uint8_t address,uint8_t values[],uint8_t len){

    write_register(i2c0_regs,I2C0_DATA_CMD, address + 0x400); //Envia um sinal de start para o primeiro registrador

    int i;

    for(i=0;i<len;i++){
        write_register(i2c0_regs,I2C0_DATA_CMD, 0x100); //Envia um sinal de leitura para os proximos 6 registradores
    }
    int byte_index = 0; //Cria uma variavel para percorrer o array guardado no buffer do I2C

    while (read_register(i2c0_regs,I2C0_RXFLR) < len){} //Espera tudo ser lido e colocado no buffer

    while (len)
    {
        if (read_register(i2c0_regs,I2C0_RXFLR)>0) //Checka o buffer
        {
            values[byte_index] = (uint8_t)read_register(i2c0_regs,I2C0_DATA_CMD);
            byte_index++;
            len--;
        }
    }
}
//Faz a leitura dos valores de XYZ e coloca eles em 16 bits.
void accel_readXYZ(int16_t XYZ_Data[3]) {
    uint8_t XYZ_8bits[6];
    I2C_readXYZ(DATA_X0, (uint8_t *)&XYZ_8bits, 6);

    XYZ_Data[0] = (XYZ_8bits[1] << 8) | XYZ_8bits[0]; 
    XYZ_Data[1] = (XYZ_8bits[3] << 8) | XYZ_8bits[2];
    XYZ_Data[2] = (XYZ_8bits[5] << 8) | XYZ_8bits[4];

}
//Obtem a direção com base no valor de X
void get_direction(int *direcao){
	if (accel_hadActivity()){
		int16_t XYZ_Data[3];	
		accel_readXYZ(XYZ_Data);
		int16_t X_Data = XYZ_Data[0];

		if (X_Data > 40)
		*direcao = 1;
		else if (X_Data < -40)
		*direcao =  -1;
		else 
		*direcao = 0;}
}


