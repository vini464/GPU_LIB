#include "../../header/rendering.h"

void initScreen() { gpu_open(); }

void clearVideo() {
  unsigned short cont = 0;
  for (cont = 0; cont < 4800; cont++)
    wbm(DISABLED, cont);
  for (cont = 1; cont < 32; cont++)
    wbr_sp(0, 0, 0, 0, cont);
  for (cont = 0; cont < 32; cont++)
    dp_square(0, 0, 0, 0, cont);
}

void shutdownScreen() {
  clearVideo();
  gpu_close();
}

void showMatrix(int height, int width, Color matrix[height][width], int paused,
                int width_center) {
  int line, col, x, y;
  unsigned short address, color;
  for (line = 5; line < height + 5; line++) {
    for (col = 14; col < width + 14; col++) {
      color =
          matrix[line - 5][col - 14] == 0 ? WHITE : matrix[line - 5][col - 14];
      for (x = line * 2; x < line * 2 + 2; x++) {
        for (y = col * 2; y < col * 2 + 2; y++) {
          address = (x * 80);
          address += y;
          wbm(color, address);
        }
      }
    }
  }
}

void showHex(unsigned int number) { // number should be < 1000000
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
