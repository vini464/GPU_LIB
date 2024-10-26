#ifndef ACCEL_H
#define ACCEL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include "accel_register_map.h"

#define I2C0_BASE_ADDR 0xFFC04000  // Endereço base do controlador I2C0
#define I2C0_REG_SIZE  0x1000      // Tamanho da região mapeada (4KB)

#define SYSMGR_GENERALIO7 ((volatile unsigned int *) 0xFFD0849C) //Endereco do GPIO7
#define SYSMGR_GENERALIO8 ((volatile unsigned int *) 0xFFD084A0) //Endereco do GPIO8
#define SYSMGR_I2C0USEFPGA ((volatile unsigned int *) 0xFFD08704) //Sentido da rota 

#define I2C0_CON                0x00 //Configurar o I2C
#define I2C0_TAR                0x04 //Target, onde vamos nos comunicar
#define I2C0_DATA_CMD           0x10 //Registrador de leitura ou escrita no target
#define I2C0_FS_SCL_HCNT        0x1C //Configuração de velocidade
#define I2C0_FS_SCL_LCNT        0x20 //Configuração de velocidade
#define I2C0_CLR_INTR           0x40
#define I2C0_ENABLE             0x6C //Inicia ou aborta comunicação
#define I2C0_TXFLR              0x74
#define I2C0_RXFLR              0x78 //Checkar o buffer
#define I2C0_ENABLE_STATUS      0x9C //ler o status da comunicação

#define I2C0_SS_SCL_HCNT        0x14
#define I2C0_SS_SCL_LCNT        0x28
#define I2C0_INTR_MASK          0x30
#define I2C0_TX_ABRT            0x54
#define I2C0_TX_ABRT_SOURCE     0x80

#define ROUNDED_DIVISION(num, divisor) (((num) + (divisor / 2)) / divisor)

typedef int bool;
#define false 0;
#define true 1;


void configure_pinmux();

// Função para abrir especificamente o /dev/mem e retornar o descritor
int open_physical();

void close_physical(int fd);

// Função para mapear a região de memória do I2C0
int mmap_i2c0(int fd);

// Função para abrir e mapear /dev/mem
int open_and_mmap_dev_mem();

// Função para fechar e liberar /dev/mem
void close_and_unmap_dev_mem(int fd);

void write_register(volatile uint32_t *base, uint32_t offset, uint32_t value);

uint32_t read_register(volatile uint32_t *base, uint32_t offset);

//Escreve no acelerometro
void accel_reg_write(uint8_t address, uint8_t value);

//Ler no acelerometro
void accel_reg_read(uint8_t address, uint8_t *value);

//Inicia a comunicação com o acelerometro via I2C
void I2C0_init();

//Testa a comunicação via leitura do DEVID
bool test_communication();

//Inicia e configura o acelerometro
void accel_init();

//Calibra os offsets do acelerometro
void accel_calibrate(int average_index);

//Checka se tem dados para leitura
bool accel_isDataReady();

//Checka se possui uma atividade que passe do limiar configurado em Thresh_act dentro do I2C_init
bool accel_hadActivity();

//Le os valores do acelerometro via I2C e coloca num array de 8bits
void I2C_readXYZ(uint8_t address,uint8_t values[],uint8_t len);

//Faz a leitura dos valores de XYZ e coloca eles em 16 bits.
void accel_readXYZ(int16_t XYZ_Data[3]);

//Obtem a direção com base no valor de X
void get_direction(int *direcao);
#endif
