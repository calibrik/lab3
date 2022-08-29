#include "control.h"
#include "handle.h"
#include <string.h>



#ifdef LAB3A

struct Control* create_control() {
	struct Control *control = malloc(sizeof(struct Control));
	control->table = create_table();
	return control;
}
#elif LAB3B

struct Control* create_control() {
	struct Control *control = malloc(sizeof(struct Control));
  char *filename = get_str("filename of binary");
	control->table = create_table(filename);
  free(filename);
	return control;
}

#endif // LAB3B

void delete_control(struct Control *control) { 
  delete_table(control->table);
  free(control);
}
const char* show_status(int index) {
  switch (index) {
    case OK:               return "Ok";
    case NOTHING_TABLE:    return "Nothing in table";
    case INCORRECT_PARENT: return "Incorrect parent";
    case INCORRECT_KEY:    return "Incorrect key";
    default:               return "Unknown status";
  }
}
void search(struct Control *control) {
	char *key = get_str("key");
  printf("%s\n", show_status(search_table(control->table,key)));
  free(key);
}
void add(struct Control *control) {
	char *key     = get_str("key");
	char *info    = get_str("info");
	char *par_key = get_str("parent key");
	int status = add_table(control->table, key, info, par_key);
  printf("%s\n", show_status(status));

  free(key);
  free(info);
  free(par_key);
}
void del(struct Control *control) {
  char *key = get_str("key");
  int status = del_key(control->table, key, true);
  printf("%s\n", show_status(status));
  free(key);
}
void display(struct Control *control) {
  step_table(control->table, display_node);
}
void del_family(struct Control *control) {
  char *key = get_str("key");
  int status = del_key(control->table, key, false);
  printf("%s\n", show_status(status));
  free(key);
}
void table_par(struct Control *control) {
  char *key = get_str("parent key");
  int status = find_all_child(control->table, key);
  printf("%s\n", show_status(status));
  free(key);
}
