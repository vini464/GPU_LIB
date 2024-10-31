#include <stdio.h>
#include <stdlib.h>

struct rec {
  int *k;
};


int main(void){
  FILE *f= fopen("foo", "r");
  struct rec r;
//  r.k = (int *) malloc(sizeof(int) * 5);
  if (!f)
    return 1;
  else{
//    fwrite(&r, sizeof(r.k), 1, f);
    fread(&r, sizeof(struct rec), 4, f);
    for (int c = 0; c < 5; c++)
    printf("%d\n", (r.k)[c]);
  }
  return 0;
}
