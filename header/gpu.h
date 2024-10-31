#ifndef GPU_H
#define GPU_H

#define RED      0b000000111
#define GREEN    0b000111000
#define BLUE     0b111000000
#define YELLOW   0b000111111
#define PURPLE   0b111000111
#define CYAN     0b111111000
#define WHITE    0b111111111
#define BLACK    0b000000000
#define DISABLED 0b111111110

#define ONE      0b11111001
#define TWO      0b10100100
#define THREE    0b10110000
#define FOUR     0b10011001
#define FIVE     0b10010010
#define SIX      0b10000010
#define SEVEN    0b11111000
#define EIGHT    0b10000000
#define NINE     0b10011000
#define ZERO     0b11000000

typedef unsigned short u_short ;
void gpu_open();
void gpu_close();

void wbm(u_short bgr, u_short address);
void wbr_bg(u_short bgr);
void wbr_sp(u_short act, unsigned int posX_Y, u_short offset, u_short reg);
void wsm(u_short spr_address, u_short bgr);
void dp_triangle(u_short bgr, u_short size, unsigned int refPosY_X, u_short reg);
void dp_square(u_short bgr, u_short size, unsigned int refPosY_X, u_short reg);
void set_hex(u_short d5_d4, int d3_d2_d1_d0);
int read_keys();

// utility functions

#endif // !GPU_H
