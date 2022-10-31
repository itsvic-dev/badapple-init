#include "load.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main() {
  printf("Loading...\n");
  for (int i = 1; i < 6573; i++) {
    load_frame(i);
  }
  for (int i = 1; i < 6573; i++) {
    clock_t start = clock();
    show_frame(i);
    clock_t stop = clock();
    usleep(33333 - (double)(stop - start) / CLOCKS_PER_SEC * 1000);
  }
  return 0;
}
