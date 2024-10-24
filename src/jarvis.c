#include "../header/gpu.h"
#include <stdlib.h>
#include <stdio.h>

void wbr_bg_handler();
void wbr_sp_handler();
void wsm_handler();
void wbm_handler();
void triangle_handler();
void square_handler();
void buttos_handler();
void hex_display_handler();

int main(void) {

  int choice, testing = 0;
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
    printf("[0] Exit.\n> ");
    scanf("%d", &choice);
    switch (choice) {}
  } while (testing);
  gpu_close();
  return 0;
}

void wbr_bg_handler() {
  int color = 0;
  do {
    printf("choose a color:");
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
      printf("Unsuported color");
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
    printf("[2] show a single sprite.\n> ");
    scanf("%d", &choice);
    if (choice <= 0 || choice > 2)
      printf("Unsuported option\n");
  } while (choice <= 0 || choice > 2);
  if (choice == 2) {
    for (unsigned short cont = 0; cont < 620; cont += 20) {
      posx = cont / 20;
      posy = cont % 20;
      wbr_sp(1, posx, posy, posx, posx); // confia que funciona
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
      if (posx < 0 || posy < 0 || posx > 31 || posy > 31)
        printf("Position out of range!\n");
    } while (posx < 0 || posy < 0 || posx > 31 || posy > 31);

    wbr_sp(1, posx, posy, sprID, reg);
  }
}

void wsm_handler() { // vai ficar pra depois
  printf("nothing to do yet\n");
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
    printf("choose a color:");
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
      printf("Unsuported color");
    }
  } while (color <= 0 || color > 8);
  switch (color) {

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
void triangle_handler(){
  unsigned short size, ptY, ptX, reg;
  printf("Insert the size, ptY, ptX and reg\n> ");
  scanf("%hu %hu %hu %hu", &size,&ptY,&ptX,&reg);
  int color = 0;
  do {
    printf("choose a color:");
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
      printf("Unsuported color");
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
void square_handler(){

  unsigned short size, ptY, ptX, reg;
  printf("Insert the size, ptY, ptX and reg\n> ");
  scanf("%hu %hu %hu %hu", &size,&ptY,&ptX,&reg);
  int color = 0;
  do {
    printf("choose a color:");
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
      printf("Unsuported color");
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
void buttos_handler(){
  int btn;
  do {
    system("clear");
    btn = read_keys();
    btn = ~btn;
    printf("btn: %d", btn);
  } while (btn != 1);
}
void hex_display_handler(){
  set_hex(0b0001001000001100, 0b00001110000110000000000000000000); // i should change this later
}
