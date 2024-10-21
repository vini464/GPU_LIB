#include "vga.h"
#include <stdint.h>

extern int32_t bg_bitmap_raw[4800];
extern int32_t map_memory();
extern void wbr_bg(int32_t fpga, unsigned short color);
extern void wbm(int32_t fpga, unsigned short color, int16_t pos);

unsigned short bg_bitmap[4800];

void setup_bg(){
  int cont;
  int32_t rgba;
  int8_t red, green, blue;
  unsigned short color;
  for (cont = 0; cont < 4800; cont++){
    rgba = bg_bitmap_raw[cont];
    blue = rgba & 0b11111111;
    rgba = rgba >> 8;
    rgba = rgba >> 8;
    green = rgba & 0b11111111;
    rgba = rgba >> 8;
    red = rgba & 0b11111111;
    
    red   /= 34; // normalizando o binario de 8 bits para 3
    green /= 34;
    blue  /= 34;
    
    color = red;
    color = color << 3;
    color += green;
    color = color << 3;
    color += blue;

   bg_bitmap[cont] = color;
 //     bg_bitmap[cont] = 0b111111111;
  }
}

int main(void) {
  int cont=4800; 
   setup_bg();
  int32_t fpga_bridge = map_memory();
  
  for (cont = 4799; cont >= 0; cont--){
  wbm(fpga_bridge, bg_bitmap[cont], cont);
}
  //setBgImg(fpga_bridge, bg_bitmap); 

  return 0;
}
