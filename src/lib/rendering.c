#include "../../header/rendering.h"

void initScreen() {
  gpu_open();
}

void clearVideo() {
  unsigned short cont = 0;
  for (cont = 0; cont < 4800; cont++)
      wbm(DISABLED, cont);
  for (cont = 1; cont < 32; cont ++)
    wbr_sp(0, 0, 0, cont);
}

void shutdownScreen() {
  clearVideo();
  gpu_close();
}

void showMatrix(int height, int width, Color matrix[height][width], int paused, int width_center) {
  int line, col, x, y;
  unsigned short address, color;
  for (line = 5; line < height +5; line++){
    for (col = 14; col < width + 14; col++){
      color = matrix[line-5][col-14] == 0 ? WHITE : matrix[line-5][col-14];
      for (x = line*2; x < line*2+2; x++){
        for (y = col*2; y < col*2+2; y++){
     	        address = (x* 80);
      		address += y;
      		wbm(color, address);
	}
}      
    }
  }
}

