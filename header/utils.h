#ifndef UTILS_H
#define UTILS_H

#define BOARDHEIGHT 20
#define BOARDWIDTH 10
#define SQUARES 4

// Apenas para deixar o código mais legível
typedef unsigned short int Color;
typedef enum boolean { TRUE = 1, FALSE = 0 } boolean;
typedef enum directions { STOP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 } Direction;

typedef struct pair {
  int first;
  int second;
} Pair;

// Estrutura de uma peça.
typedef struct piece {
  Pair actual_pos[4];
  Pair older_pos[4];
  Color color;
} Piece;


// Pega uma cor do vetor de cores já definido.
Color getColor(int index);
// Pega uma peça do vetor de peça já definido.
Piece getPiece(int index);

#endif // !UTILS_H
