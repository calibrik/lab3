#include "handle.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char* get_str(const char *label) {
  char *str = malloc(sizeof(char) * 128);
  if (label)
    printf("Enter %s: ", label);
  scanf("%s", str);
  
  return realloc(str, sizeof(char) * (strlen(str) + 1));
}
char* str_dup(const char *srcstr) {
  if (srcstr == NULL)
    return NULL;

  char *str = malloc(sizeof(char) * (strlen(srcstr) + 1));
  strcpy(str, srcstr);

  return str;
}
