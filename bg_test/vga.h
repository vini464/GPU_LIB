#ifndef VGA_H
#define VGA_H

#include <stdint.h>

void setSpr(int32_t fpga_bridge, short int *bitmap_vector, int offset);

void setBgImg(int32_t fpga_bridge, unsigned short bitmap[4800]);

#endif // !VGA_H
