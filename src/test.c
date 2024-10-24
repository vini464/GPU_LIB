#include "gpu.h"
#include <stdio.h>

int main(void){
  int btn = 1;
  int key;
  gpu_open();
  while (btn) {
    key = read_keys();
    printf("%d\n", key);
    if (key == 2) btn = 0;
  }
  set_hex(0b0100100000110000, 0b01110000000110000000000000000000);
  wbm(YELLOW, 0b1111);
  btn = 1;
  while (btn) {
    key = read_keys();
    printf("%d\n", key);
    if (key == 2) btn = 0;
  }
  wbr_bg(GREEN);
  gpu_close();
  return 0;
}
