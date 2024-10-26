#include "../../header/game.h"

void movePieceDown(Piece *piece) {
  int j;
  boolean collide;
  for (j = 0; j < SQUARES; j++) {
    piece->older_pos[j].first = piece->actual_pos[j].first;
    piece->older_pos[j].second = piece->actual_pos[j].second;
    piece->actual_pos[j].first++;
   }
}

void movePieceLeft(Piece *piece) {
  int j;
  for (j = 0; j < SQUARES; j++) {
    piece->older_pos[j].first = piece->actual_pos[j].first;
    piece->older_pos[j].second = piece->actual_pos[j].second;
    piece->actual_pos[j].second--;
  }
}

void movePieceRight(Piece *piece) {
  int j;
  for (j = 0; j < SQUARES; j++) {
    piece->older_pos[j].first = piece->actual_pos[j].first;
    piece->older_pos[j].second = piece->actual_pos[j].second;
    piece->actual_pos[j].second++;
  }
}

void deleteInBoard(Pair square_pos, int height, int width,
                   Color board[height][width]) {
  if (square_pos.first >= 0 && square_pos.second >= 0) {
    board[square_pos.first][square_pos.second] = 0;
  }
}

void insertInBoard(Pair square_pos, Color color, int height, int width,
                   Color board[height][width]) {
  if (square_pos.first >= 0 && square_pos.second >= 0) {
    board[square_pos.first][square_pos.second] = color;
  }
}

boolean checkCollision(Pair square_pos, int height, int width,
                       Color board[height][width]) {
  int j, k;
  if (square_pos.first >= height || square_pos.second < 0 ||
      square_pos.second >= width ||
      (square_pos.first >= 0 && board[square_pos.first][square_pos.second]))
    return TRUE;
  return FALSE;
}

boolean movePiece(Piece *piece, int height, int width, Color board[height][width],
               Direction direction, boolean *gameover) {
  Piece new_pos = *piece;
  boolean collide = FALSE;


  switch (direction) {
  case LEFT:
    movePieceLeft(&new_pos);
    break;
  case RIGHT:
    movePieceRight(&new_pos);
    break;
  case DOWN:
    movePieceDown(&new_pos);
    break;
  case STOP:
    break;
  }

  int j;
  

  /** debugging
  printf("\n---%s---\n", direction == DOWN ? "down" : "right");
  for (j = 0; j < SQUARES; j++){
    printf("old: {%d, %d} | now: {%d,%d} \n", new_pos.older_pos[j].first, new_pos.older_pos[j].second, new_pos.actual_pos[j].first, new_pos.actual_pos[j].second);
  }
  **/
  for (j = 0; j < SQUARES; j++)
    deleteInBoard(new_pos.older_pos[j], height, width, board);

  for (j = 0; j < SQUARES; j++) { // checa se ao menos um quadrado colidiu

    collide = checkCollision(new_pos.actual_pos[j], height, width, board);
    if (collide) {
      break;
    }
  }

  if (!collide) {
    for (j = 0; j < SQUARES; j++){
      insertInBoard(new_pos.actual_pos[j], new_pos.color, height, width, board);
    }
    *piece = new_pos;
    return FALSE;
  } else {
    for (j = 0; j < SQUARES; j++){
      insertInBoard(new_pos.older_pos[j], new_pos.color, height, width, board);

    }
    if (collide && direction == DOWN) {
      checkGameOver(new_pos.older_pos, gameover);
      return TRUE;
    }
  }
  return FALSE;
}

void checkGameOver(Pair position[SQUARES], boolean *gameover) {
  int j;
  for (j = 0; j < SQUARES; j++) {
    Pair pos = position[j];
    if (pos.first < 0){
      *gameover = TRUE;
      return;
    }
  }
  *gameover = FALSE;
}

void gravity(int height, int width, Color board[height][width]){
  int k, j, aux;
  Color color;
  for (k = 0; k<width; k++){
    for (j = height-1; j >= 0; j--){
      if (board[j][k] > 0) {
        color = board[j][k];
        board[j][k] = 0;
        aux = j+1;
        while (aux < height && board[aux][k] == 0) aux++;
        board[aux-1][k] = color;
      }
    }
  }
}


int clearLine(int board_width, int board_height, Color board[board_height][board_width]) {
  int j, pts = 0, combo = 0;
  while (1) {
    for (j = 0; j < board_width; j++) {
      if (board[board_height - 1][j] == 0)
        return pts;
    }
    for (j = 0; j < board_width; j++) {
      board[board_height - 1][j] = 0;
    }
    pts += 10 + combo * combo;
    combo++;
    gravity(board_height, board_width, board);
  }
  return pts;
}
