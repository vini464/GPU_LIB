#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../header/accel.h"
#include "../header/game.h"
#include "../header/rendering.h"
#include "../header/utils.h"

// Função para a thread de leitura dos botões
void *buttonListener(void *);
// inicia a configuração do acelerômetro
void startAccelListener();
// função para a thread de leitura do acelerômetro
void *accelListener(void *);
// Encerra a comunicação com o acelerômetro
void stopAccelListener();
// Roda toda a lógica do jogo
void game();

Color BOARD[BOARDHEIGHT][BOARDWIDTH];
int LISTEN_BTN, LISTEN_ACCEL, BUTTON, FD, WIDTH_CENTER, PTS;
boolean GAMEOVER, PAUSED = FALSE, OUT = FALSE;
Piece ACTUAL_PIECE, NEXT_PIECE;

int main(void) {
  srand(time(NULL)); // Definir uma seed "aleatória" para o rand
  int d, reset = 0, start = 0;
  boolean collide = FALSE;
  pthread_t btns_t, accel_t;

  // configuração inicial
  startAccelListener();
  initScreen();
  LISTEN_BTN = LISTEN_ACCEL = 1; // habilita a leitura do acelerômetro e dos botões
  // criação das threads
  pthread_create(&accel_t, NULL, accelListener, NULL);
  pthread_create(&btns_t, NULL, buttonListener, NULL);

  while (1) {
    BUTTON = 0;
    // espera o jogador iniciar o jogo
    if (!start) {
      printf("pressione algum botão para iniciar");
      while (!start) {
        if (BUTTON != 0) {
          start = 1;
          BUTTON = 0;
        }
      }
    }

    clearVideo();
    // roda o jogo, até o jogador perder, desistir ou reiniciar
    game();
    usleep(1000);

    if (GAMEOVER) { // se ele perdeu mostra a tela de game over
      BUTTON = 0;
      while (BUTTON == 0) { // espera o jogador apertar um botão 
        if (BUTTON == 1) { // se ele apertar o botão 1 encerra o programa
          reset = 0;
        } else if (BUTTON > 1) {
          reset = 1;
        }
      }
      if (!reset)
        break;
    } else { 
      if (OUT) // caso ele tenha desistido encerra o loop do jogo
        break;
      GAMEOVER = FALSE;
    }
  }
  // encerrando as threads
  LISTEN_ACCEL = 0;
  LISTEN_BTN = 0;
  pthread_join(btns_t, NULL);
  pthread_join(accel_t, NULL);
  // encerrando os drivers
  stopAccelListener();
  clearVideo();
  return 0;
}

void game() {
  int j, k;
  PTS = 0;
  printf("pontuacao: %d\n", PTS);
  GAMEOVER = FALSE;
// limpa a matriz
  for (j = 0; j < BOARDHEIGHT; j++) {
    for (k = 0; k < BOARDWIDTH; k++)
      BOARD[j][k] = WHITE;
  }
  int collide;
  while (GAMEOVER == FALSE) { // enquanto o jogador não perdeu
    collide = FALSE;
    ACTUAL_PIECE = getPiece(rand() % 17); // pega uma peça aleatória
    ACTUAL_PIECE.color = getColor(rand() % 6); // atribui uma cor aleatória para a peça
    while (!collide) {
      if (BUTTON == 1) { // pausa o jogo
        PAUSED = TRUE;
        BUTTON = 0;
      }
      while (PAUSED) {
        if (BUTTON > 0 && BUTTON < 4) { // continua o jogo
          PAUSED = FALSE;
          BUTTON = 0;
          if (BUTTON == 2) { // reinicia o jogo
            return;
          }
          if (BUTTON == 3) { // encerra o jogo
            OUT = 1;
            return;
          }
        }
      }
      collide = movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, BOARD, DOWN,
                          &GAMEOVER); // move a peça para baixo

      showMatrix(BOARDHEIGHT, BOARDWIDTH, BOARD, PAUSED, WIDTH_CENTER); // mostra a matriz do jogo na tela

      usleep(100000); // delay para cair a peça
    }
    gravity(BOARDHEIGHT, BOARDWIDTH, BOARD); // faz as peças cairem
    PTS += clearLine(BOARDWIDTH, BOARDHEIGHT, BOARD); // calcula a pontuação e elimina agrupamentos
  }
}


void startAccelListener() {
  int i;
  for (i = 0; i < 10; i++) {
    FD = open_and_mmap_dev_mem();
    if (FD == -1)
      printf("não foi possível abrir /dev/mem\n");
    else
      break;
  }
  if (FD == -1)
    exit(-1);
  I2C0_init();
  accel_init();
  accel_calibrate(60);
}
void *accelListener(void *arg) {
  int accel_read = 0;
  Direction dir;
  while (LISTEN_ACCEL) {
    get_direction(&accel_read);
    switch (accel_read) {
    case -1:
      dir = LEFT;
      break;
    case 1:
      dir = RIGHT;
      break;
    default:
      dir = STOP;
    }
    if (dir != STOP && !PAUSED && !GAMEOVER) {

      movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, BOARD, dir, &GAMEOVER);
      showMatrix(BOARDHEIGHT, BOARDWIDTH, BOARD, PAUSED, WIDTH_CENTER);
    }
    dir = STOP;
    usleep(200000);
  }
  return NULL;
}
void stopAccelListener() { close_and_unmap_dev_mem(FD); }

void *buttonListener(void *arg){
  int btn = 0;
  while (LISTEN_BTN) {
    btn = read_keys();
    if (btn == 1){
      BUTTON = 4;
    }
    else if (btn == 2){
      BUTTON = 3;
    }
    else if (btn == 4){
      BUTTON = 2;
    }
    else if (btn == 8){
      BUTTON = 1;
    }
  }
  return NULL;
}
