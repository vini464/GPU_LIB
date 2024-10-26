#include "../../header/rendering.h"

void initScreen() {
  gpu_open();
}

void clearVideo() {
  unsigned short cont = 0;
  for (cont = 0; cont < 4800; cont ++){
    wbm(DISABLED, cont);
  }
  for (cont = 1; cont < 32; cont ++)
    wbr_sp(0, 0, 0, cont);
}

void shutdownScreen() {
  clearVideo();
  gpu_close();
}

void showMatrix(int height, int width, Color matrix[height][width], int paused, int width_center) {
  int line, col;
  unsigned short address;
  clearVideo();
  for (line = 0; line < height; line++){
    for (col = 0; col < width; col++){
      address = line * 80;
      address += col;
      wbm(matrix[line][col], address);
    }
  }
}

