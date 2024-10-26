#ifndef ACCEL_REGISTER_MAP_H
#define ACCEL_REGISTER_MAP_H

#define DEVID 0x00 //R, segura um valor fixo de ID que é 345 (11100101)

#define THRESH_TAP 0x1D /* vai de 250 (16g) até 0, quanto menor, mais facil detecar um movimento, The scale factor is 
62.5 mg/LSB (that is, 0xFF = 16 g)*/
 //Posso usar pra configurar o Shingle_Tap mode


#define OFSX 0x1E
#define OFSY 0x1F
#define OFSZ 0x20
//Offsets dos eixos X,Y,Z

#define DUR 0x21 /*Define o tempo maximo que um evento deve permanecer para acontecer um tap, usado para configurar
o double e o single tap.
O fator de escala é de 625 μs/LSB. Um valor de 0 desativa as funções de toque único (single tap) e toque duplo (double tap) */

#define LATENT 0x22 /*O registro Latent possui oito bits e contém um valor de tempo sem sinal (unsigned) que representa
o tempo de espera desde a detecção de um evento de toque (tap) até o início da janela de tempo (definida pelo registro Window) 
durante a qual um possível segundo evento de toque pode ser detectado. O fator de escala é de 1,25 ms/LSB.
Um valor de 0 desativa a função de toque duplo (double tap).*/

#define WINDOW 0x23 /*O registro Window define o período de tempo após o tempo de latência durante o qual um segundo toque
pode ser detectado como parte de um gesto de toque duplo. Esse período é escalado em 1,25 ms por unidade do registro.
Um valor de 0 desativa a função de toque duplo.*/

#define THRESH_ACT 0x24 /*O registro THRESH_ACT define o limite de aceleração necessário para detectar um evento de atividade.
Esse valor é escalado em 62,5 mg por unidade. Um valor de 0 pode levar a falsas detecções de atividade se a interrupção 
de atividade estiver habilitada.*/

/*Pequeno disclaimer, no projeto da placa usaremos o THRESH_ACT, o seu uso é para identificar inclinações ou movimentos gerais
O THRESH_TAP é melhor para toques rapidos e curtos*/

#define THRESH_INACT 0x25 /*O registro THRESH_INACT define o limite de aceleração abaixo do qual um evento é considerado
inatividade. Esse limite é escalado em 62,5 mg por unidade do registro.
Um valor de 0 pode causar detecções falsas de inatividade se a interrupção de inatividade estiver habilitada.*/

#define TIME_INACT 0x26 /*O registro TIME_INACT é de oito bits e contém um valor de tempo sem sinal (unsigned)
que representa o tempo necessário para que a aceleração seja menor do que o valor no registro THRESH_INACT para que 
a inatividade seja declarada. O fator de escala é de 1 segundo/LSB. Ao contrário das outras funções de interrupção,
que utilizam dados não filtrados (ver seção Threshold), a função de inatividade usa dados de saída filtrados.
Pelo menos uma amostra de saída deve ser gerada para que a interrupção de inatividade seja acionada. 
Isso faz com que a função pareça não responsiva se o registro TIME_INACT estiver configurado para um valor menor do que 
o tempo constante da taxa de dados de saída. Um valor de 0 resulta em uma interrupção quando os dados de saída 
forem menores do que o valor no registro THRESH_INACT.*/

#define ACT_INACT_CTL 0x27 //Julguei não necessario para o problema e não conseguir entender corretamente seu funcionamento

#define THRESH_FF 0x28 //Usado para detectar queda livre, provavelmente não vamos utilizar

#define TIME_FF 0x29 //Usado para detectar queda livre, provavelmente não vamos utilizar

#define TAP_AXES 0x2A //Usado para os taps modes, b0 = Z, b1 = Y, b2 = X, b3 = supressão, b0 a b2 ativam para cada eixo.

#define ACT_TAP_STATUS 0x2B /*Read Only, não dei a devida importancia, já que provavelmente não vamos utilizar os taps modes,
serve pelo oq entendi, para saber quais eixos estão ativados e de onde vem sua fonte de ativação?*/

#define BW_RATE 0x2C /*Bit LOW_POWER: Define o modo de operação como normal ou de baixa potência.
Bits Rate: Definem a largura de banda e a taxa de dados de saída, com o valor padrão configurado para 100 Hz.
Uma taxa de dados apropriada deve ser escolhida com base no protocolo de comunicação para evitar o descarte de amostras.
b4 = ligar ou desligar low_power, b3 ao b0 = determinar o rate*/

#define POWER_CTL 0x2D /*Link Bit = b5 : Vincula a detecção de atividade e inatividade.
AUTO_SLEEP Bit = b4: Habilita a alternância automática para o modo de sono se a inatividade for detectada.
Measure Bit = b3 : Define o modo de medição ou standby, 0 = standby, 1 = medição, sempre começa em 0 ao ligar.
Sleep Bit = b2: Define o modo normal ou modo de sono.
Wakeup Bits = b1,b0 : Controlam a frequência de leitura de dados no modo de sono.
Bem resumido, recomenda-se leitura do Data Sheet na pag 24 pois o assunto era bem extenso*/
#define INT_ENABLE 0x2E /*Configuração de Bits:

DATA_READY = b7: Habilita a interrupção quando novos dados estão disponíveis.
SINGLE_TAP = b6: Habilita a interrupção para eventos de "single tap" (toque único).
DOUBLE_TAP = b5: Habilita a interrupção para eventos de "double tap" (toque duplo).
Activity = b4: Habilita a interrupção para eventos de atividade.
Inactivity = b3: Habilita a interrupção para eventos de inatividade.
FREE_FALL = b2: Habilita a interrupção para eventos de queda livre.
Watermark = b1: Habilita a interrupção quando o número de amostras na FIFO atinge o valor definido no registro FIFO_CTL.
Overrun = b0: Habilita a interrupção quando há um "overrun" na FIFO, indicando que novos dados foram escritos na FIFO
antes que os dados antigos fossem lidos.
1 = habilita a função, 0 = desabilita
DATA_READY, Watermark, e Overrun: Esses bits habilitam apenas a saída da interrupção, enquanto as funções subjacentes sempre estão ativas.
Isso significa que, mesmo que a interrupção não esteja habilitada, o dispositivo ainda monitora esses eventos.

Recomendação de sempre configurar os bits de interrupção antes de habilitar suas saídas, para assim, garantirmos seu funcionamento correto */

#define INT_MAP 0x2F /*O INT_MAP Register permite configurar qual pino (INT1 ou INT2) receberá as interrupções de cada tipo de evento.
A configuração permite que múltiplos eventos sejam combinados para gerar uma interrupção em um único pino.
Como não vamos usar pinos(Se entendi bem), o proximo registrador que seria feito para essa leitura*/
#define INT_SOURCE  0x30 /*READ ONLY, O INT_SOURCE Register é usado para verificar quais interrupções foram acionadas.
Os bits indicam se eventos específicos ocorreram e podem ser apagados ao ler os registradores de dados ou o próprio INT_SOURCE Register,
dependendo do evento.
o pino INT1 ou INT2 se ligam como um OR nesse INT_SOURCE se meu entendimento foi correto, então independe de para qual pino vc deu o sinal
você consegue identificar no int source
tanto o int_map e o int_source seguem os eventos do INT_ENABLE e sua logica, exemplo, b1 = watermark*/

#define DATA_FORMAT 0x31 /*DATA_FORMAT Register: Este registro controla como os dados são apresentados nos registradores de dados (0x32 a 0x37).
Dependendo da configuração, os dados podem ter diferentes resoluções e intervalos.

SELF_TEST Bit = b7: Usado para verificar o funcionamento do sensor aplicando uma força interna.

SPI Bit = b6 : Determina se o dispositivo usa 3 fios ou 4 fios para comunicação SPI.

INT_INVERT Bit = b5 e b4: Define o nível de ativação dos sinais de interrupção.

FULL_RES Bit = b3: Controla a resolução dos dados de saída, aumentando a resolução em modo de plena resolução.

Justify Bit = b2: Define a forma como os dados são alinhados, o que pode influenciar a forma como você interpreta os dados lidos.

Range Bits = b1,b0: Estabelecem a faixa de medição do acelerômetro (±2 g = 00, ±4 g = 01, ±8 g = 10, ±16 g = 11),
que afeta a sensibilidade e a resolução do sensor.*/

#define DATA_X0 0x32 //READ ONLY
#define DATA_X1 0x33 //READ ONLY
#define DATA_Y0 0x34 //READ ONLY
#define DATA_Y1 0x35 //READ ONLY
#define DATA_Z0 0x36 //READ ONLY
#define DATA_Z1 0x37 //READ ONLY
/*Pequeno disclaimer, o register *0 é o menos significativo e o *1 o mais significativo, pelo oq entendi a leitura tem
uma resolução de 16 bits, então se usa 2 registradores de 8 bits*/

#define FIFO_CTL 0x38 /*Não conseguir discenir sobre o seu real funcionamento, acredito que seja para criar uma especie
de buffer, não sei se será util, então ignorei*/

#define FIFO_STATUS 0x39 //READ ONLY, usado para identificar qual o modo do FIFO.


/*O bit DOUBLE_TAP é ativado quando dois eventos de aceleração que são maiores do que o valor no registro
THRESH_TAP (Endereço 0x1D) ocorrem por um tempo menor do que o especificado no registro DUR (Endereço 0x21),
com o segundo toque começando após o tempo especificado no registro LATENT (Endereço 0x22), mas dentro do tempo
especificado no registro WINDOW (Endereço 0x23). Veja a seção de Detecção de Toque para mais detalhes.*/
/*"O bit SINGLE_TAP é ativado quando um único evento de aceleração que é maior que o valor no registro THRESH_TAP (Endereço 0x1D)
ocorre por um tempo menor do que o especificado no registro DUR (Endereço 0x21)" Trecho retirado e traduzido do Data Sheet*/
#endif
