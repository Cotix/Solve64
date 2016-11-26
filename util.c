#include <stdlib.h>

unsigned long long rand64() {
  unsigned long long r = 0;
  for (int i=0; i<64; i++) {
    r = r*2 + rand()%2;
  }
  return r;
}
