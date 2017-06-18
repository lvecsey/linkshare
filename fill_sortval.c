
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

int fill_sortval(char *str, size_t len, long int value) {

  sprintf(str, "%0.6x", value);
  
  return 0;
  
}

