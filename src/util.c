#include "util.h"
#include <string.h>
#include <stdlib.h>

char* cat(char* str1, char* str2) {
  int len = strlen(str1) + strlen(str2);
  char* out = (char*)(malloc((len + 1) * sizeof(char)));
  strcat(out, str1);
  strcat(out, str2);
  return out;
}
