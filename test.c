#include "vga.h"
#include <stdint.h>

extern int32_t bg_bitmap_raw[4800];
extern int32_t map_memory();

unsigned short bg_bitmap[4800];

void setup_bg(){
  int cont;
  int32_t rgba;
  int8_t red, green, blue;
  unsigned short color;
  for (cont = 0; cont < 4800; cont++){
    rgba = bg_bitmap_raw[cont];
    red = rgba & 0b11111111;
    rgba = rgba >> 8;
    green = rgba & 0b11111111;
    rgba = rgba >> 8;
    blue = rgba & 0b11111111;
    
    red   /= 73; // normalizando o binario de 8 bits para 3
    green /= 73;
    blue  /= 73;
    
    color = red;
    color = color << 3;
    color += green;
    color = color << 3;
    color += blue;

    bg_bitmap[cont] = color;
  }
}

int main(void) {
  
  setup_bg();
  int32_t fpga_bridge = map_memory();
  setBgImg(fpga_bridge, bg_bitmap); 

  return 0;
}
