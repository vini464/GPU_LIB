#include "../../header/rendering.h"

void initScreen() { gpu_open(); }

void clearVideo() {
  unsigned short cont = 0;
  for (cont = 0; cont < 4800; cont++)
    wbm(DISABLED, cont);
  for (cont = 1; cont < 32; cont++)
    wbr_sp(0, 0, 0, cont);
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
  unsigned int algs[6];
  int c;
  for (c = 0; c < 6; c++) {
    algs[c] = number % 10;
    number /= 10;
  }
  unsigned short d1, d2;
  d1 = algs[0];
  d1 = d1<< 8;
  d1 += algs[1];

  d2 = algs[2];
  d2 = d2 << 8;

  d2 += algs[3];
  d2 = d2 << 8;

  d2 += algs[4];
  d2 = d2 << 8;

  d2 += algs[5];
}
