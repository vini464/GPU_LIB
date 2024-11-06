#include "../header/gpu.h"
#include <stdio.h>

typedef struct Sprite Sprite;
typedef struct node Node;

unsigned short snake[400];

struct Sprite {
  unsigned short pixels[20][20];
  int HEIGHT : 20; // não sei se isso funciona
  int WIDTH : 20;
};

struct node {
  Sprite spr;
  Node *next;
};

Node *ready_sprites;

void wbr_bg_handler();
void wbr_sp_handler();
void wsm_handler();
void wbm_handler();
void triangle_handler();
void square_handler();
void buttos_handler();
void hex_display_handler();
void clear_video();

int main(void) {
  Sprite spr;
  int choice, testing = 1;
  gpu_open();
  do {

    printf("What should i test?\n");
    printf("[1] WBR_BG;\n");
    printf("[2] WBR_SP;\n");
    printf("[3] WSM;\n");
    printf("[4] WBM;\n");
    printf("[5] DP_TRIANGLE;\n");
    printf("[6] DP_SQUARE;\n");
    printf("[7] BUTTONS;\n");
    printf("[8] HEX-DISPLAY;\n");
    printf("[9] Limpar tela;\n");
    printf("[0] Exit.\n> ");
    scanf("%d", &choice);
    switch (choice) {
    case 0:
      testing = 0;
      break;
    case 1:
      wbr_bg_handler();
      break;
    case 2:
      wbr_sp_handler();
      break;
    case 3:
      wsm_handler();
      break;
    case 4:
      wbm_handler();
      break;
    case 5:
      triangle_handler();
      break;
    case 6:
      square_handler();
      break;
    case 7:
      buttos_handler();
      break;
    case 8:
      hex_display_handler();
      break;
    case 9:
      clear_video();
      break;
    }
  } while (testing);
  printf("saindo...\n");
  gpu_close();
  return 0;
}

void wbr_bg_handler() {
  int color = 0;
  do {
    printf("choose a color:\n");
    printf("[1] YELLOW;\n");
    printf("[2] GREEN;\n");
    printf("[3] RED;\n");
    printf("[4] BLUE;\n");
    printf("[5] PURPLE;\n");
    printf("[6] CYAN;\n");
    printf("[7] WHITE;\n");
    printf("[8] BLACK;\n> ");
    scanf("%d", &color);
    if (color <= 0 || color > 8) {
      printf("Unsuported color\n");
    }
  } while (color <= 0 || color > 8);
  switch (color) {
  case 1:
    wbr_bg(YELLOW);
    break;
  case 2:
    wbr_bg(GREEN);
    break;
  case 3:
    wbr_bg(RED);
    break;
  case 4:
    wbr_bg(BLUE);
    break;
  case 5:
    wbr_bg(PURPLE);
    break;
  case 6:
    wbr_bg(CYAN);
    break;
  case 7:
    wbr_bg(WHITE);
    break;
  case 8:
    wbr_bg(BLACK);
    break;
  }
}

void wbr_sp_handler() {
  int choice = 0;
  int unsigned short posx, posy;
  int unsigned short sprID, reg;
  do {
    printf("What you want to do?\n");
    printf("[1] show a single sprite.\n");
    printf("[2] show all sprites.\n> ");
    scanf("%d", &choice);
    if (choice <= 0 || choice > 2)
      printf("Unsuported option\n");
  } while (choice <= 0 || choice > 2);
  if (choice == 2) {
    unsigned short l, c;
    sprID = 0;
    reg = 1;
    for (l = 0; l < 6; l++) {
      for (c = 0; c < 5; c++) {
        posx = c * 20;
        posy = l * 20;
        wbr_sp(1, posx, posy, sprID, reg);
        sprID++;
        reg++;
      }
    }
  } else {
    do {
      printf("Insert a position\n> ");
      scanf("%hu %hu", &posx, &posy);
      if (posx < 0 || posy < 0 || posx > 1023 || posy > 1023)
        printf("Position out of range!\n");
    } while (posx < 0 || posy < 0 || posx > 1023 || posy > 1023);
    do {
      printf("Insert the sprite id and the register\n> ");
      scanf("%hu %hu", &sprID, &reg);
      if (sprID < 0 || reg <= 0 || sprID > 31 || reg > 31)
        printf("you typed something wrong!\n");
    } while (sprID < 0 || reg <= 0 || sprID > 31 || reg > 31);

    wbr_sp(1, posx, posy, sprID, reg);
  }
}

void wsm_handler() { // vai ficar pra depois
  int address;
  printf("indique onde voce quer guardar esse sprite: >\n");
  scanf("%d", &address);
  save_sprite(snake, address);
}
void wbm_handler() {
  int l, c;
  do {
    printf("Insert the line and the collumn\n> ");
    scanf("%d %d", &l, &c);
    if (l < 0 || c < 0 || l > 79 || c > 59)
      printf("Position out of range!\n");
  } while (l < 0 || c < 0 || l > 79 || c > 59);

  int color = 0;
  do {
    printf("choose a color:\n");
    printf("[1] YELLOW;\n");
    printf("[2] GREEN;\n");
    printf("[3] RED;\n");
    printf("[4] BLUE;\n");
    printf("[5] PURPLE;\n");
    printf("[6] CYAN;\n");
    printf("[7] WHITE;\n");
    printf("[8] BLACK;\n");
    printf("[0] DISABLE;\n> ");
    scanf("%d", &color);
    if (color < 0 || color > 8) {
      printf("Unsuported color\n");
    }
  } while (color < 0 || color > 8);
  switch (color) {

  case 0:
    wbm(DISABLED, (80 * l) + c);
    break;
  case 1:
    wbm(YELLOW, (80 * l) + c);
    break;
  case 2:
    wbm(GREEN, (80 * l) + c);
    break;
  case 3:
    wbm(RED, (80 * l) + c);
    break;
  case 4:
    wbm(BLUE, (80 * l) + c);
    break;
  case 5:
    wbm(PURPLE, (80 * l) + c);
    break;
  case 6:
    wbm(CYAN, (80 * l) + c);
    break;
  case 7:
    wbm(WHITE, (80 * l) + c);
    break;
  case 8:
    wbm(BLACK, (80 * l) + c);
    break;
  }
}
void triangle_handler() {
  unsigned short size, ptY, ptX, reg;
  printf("Insert the size, ptY, ptX and reg\n> ");
  scanf("%hu %hu %hu %hu", &size, &ptY, &ptX, &reg);
  int color = 0;
  do {
    printf("choose a color:\n");
    printf("[1] YELLOW;\n");
    printf("[2] GREEN;\n");
    printf("[3] RED;\n");
    printf("[4] BLUE;\n");
    printf("[5] PURPLE;\n");
    printf("[6] CYAN;\n");
    printf("[7] WHITE;\n");
    printf("[8] BLACK;\n> ");
    scanf("%d", &color);
    if (color <= 0 || color > 8) {
      printf("Unsuported color\n");
    }
  } while (color <= 0 || color > 8);
  switch (color) {

  case 1:
    dp_triangle(YELLOW, size, ptY, ptX, reg);
    break;
  case 2:
    dp_triangle(GREEN, size, ptY, ptX, reg);
    break;
  case 3:
    dp_triangle(RED, size, ptY, ptX, reg);
    break;
  case 4:
    dp_triangle(BLUE, size, ptY, ptX, reg);
    break;
  case 5:
    dp_triangle(PURPLE, size, ptY, ptX, reg);
    break;
  case 6:
    dp_triangle(CYAN, size, ptY, ptX, reg);
    break;
  case 7:
    dp_triangle(WHITE, size, ptY, ptX, reg);
    break;
  case 8:
    dp_triangle(BLACK, size, ptY, ptX, reg);
    break;
  }
}
void square_handler() {

  unsigned short size, ptY, ptX, reg;
  printf("Insert the size, ptY, ptX and reg\n> ");
  scanf("%hu %hu %hu %hu", &size, &ptY, &ptX, &reg);
  int color = 0;
  do {
    printf("choose a color:\n");
    printf("[1] YELLOW;\n");
    printf("[2] GREEN;\n");
    printf("[3] RED;\n");
    printf("[4] BLUE;\n");
    printf("[5] PURPLE;\n");
    printf("[6] CYAN;\n");
    printf("[7] WHITE;\n");
    printf("[8] BLACK;\n> ");
    scanf("%d", &color);
    if (color <= 0 || color > 8) {
      printf("Unsuported color\n");
    }
  } while (color <= 0 || color > 8);
  switch (color) {

  case 1:
    dp_square(YELLOW, size, ptY, ptX, reg);
    break;
  case 2:
    dp_square(GREEN, size, ptY, ptX, reg);
    break;
  case 3:
    dp_square(RED, size, ptY, ptX, reg);
    break;
  case 4:
    dp_square(BLUE, size, ptY, ptX, reg);
    break;
  case 5:
    dp_square(PURPLE, size, ptY, ptX, reg);
    break;
  case 6:
    dp_square(CYAN, size, ptY, ptX, reg);
    break;
  case 7:
    dp_square(WHITE, size, ptY, ptX, reg);
    break;
  case 8:
    dp_square(BLACK, size, ptY, ptX, reg);
    break;
  }
}
void buttos_handler() {
  int btn;
  do {
    btn = read_keys();
    btn = ~btn;
    btn = btn & 0b1111;
    printf("btn: %d\n", btn);
  } while (btn != 1);
}
void hex_display_handler() {
  unsigned int number;
  printf("Insira um número de 0 a 999999\n> ");
  scanf("%ud", &number);
  unsigned int algs[6], aux;
  int c;
  for (c = 0; c < 6; c++)
    algs[c] = 0;

  for (c = 0; c < 6; c++) {
    aux = number % 10;
    switch (aux) {
      case 0: 
        algs[c] = ZERO;
        break;
      case 1: 
        algs[c] = ONE;
        break;
      case 2: 
        algs[c] = TWO;
        break;
      case 3: 
        algs[c] = THREE;
        break;
      case 4: 
        algs[c] = FOUR;
        break;
      case 5: 
        algs[c] = FIVE;
        break;
      case 6: 
        algs[c] = SIX;
        break;
      case 7: 
        algs[c] = SEVEN;
        break;
      case 8: 
        algs[c] = EIGHT;
        break;
      case 9: 
        algs[c] = NINE;
        break;
    }
    number /= 10;
  }
  aux = 0;
  for (c=0; c < 4; c++){
    aux += algs[c] << 8*c;
}
  unsigned int aux2;
  aux2 = algs[4];
  aux2 += algs[5] <<  8;
  set_hex(aux2, aux);
  
}

void clear_video() {
  unsigned short cont = 0;
  for (cont = 0; cont < 4800; cont++)
    wbm(DISABLED, cont);
  for (cont = 1; cont < 32; cont++)
    wbr_sp(0, 0, 0, 0, cont);
  for (cont = 0; cont < 32; cont++)
    dp_square(0, 0, 0, 0, cont);
  wbr_bg(BLACK);
}
