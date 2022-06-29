#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "control.h"

int main()
{
	struct Control *control=create_control();

  void (*menu_item[])(struct Control*) = {NULL, search, add,
                                          del, display, del_family,
                                          table_par};
  const char *menu[] = {"0. EXIT","1. SEARCH", "2. ADD", "3. DEL", "4. DISPLAY", "5. DEL_FAMILY", "6. TABLE_PAR"}; 
#ifdef DEBUG
  FILE *file = fopen("input.txt", "r");
  FILE *tmp  = stdin;
  stdin = file;
  int iteration = 2;
#endif 
  int a;
  while (1) {
    for (int i=0;i<7;++i)
    {
      printf("%s\n", menu[i]);
    }
    scanf("%d%*c", &a);
    if (a == 0)
      break;
#ifdef DEBUG
    --iteration;
    if (iteration == 0)
      stdin = tmp;
#endif 
    menu_item[a](control);
  } 

  delete_control(control);
#ifdef DEBUG
  fclose(file);
#endif // DEBUG

  return 0;
}
