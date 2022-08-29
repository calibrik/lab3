#include "node.h"
#include "handle.h"
#include <string.h>


static void print_node(FILE *file, char *key, char *info, char *par) {
  fprintf(file, "Key: %-10s|Info: %-10s|Par: %s\n", key, info,
          par ? par : "NULL");
}

#ifdef LAB3A

Node* create_node(char *key, char *info, Node *par) {
  Node *node = malloc(sizeof(Node));
  node->key  = str_dup(key);
  node->info = str_dup(info);
	node->par  = par;
  node->next = NULL;
	return node;
}
void delete_node(Node *node) {
  if (node) {
    free(node->key);
    free(node->info);
    free(node);
  }
}
void delete_list_node(Node **node) {
  Node *trash = NULL;
  while (*node) {
    trash = *node;
    *node = (*node)->next;
    delete_node(trash);
  }
}
void step_list(Node* head, void *key_data, void *result, node_foo action) {
  for(; head != NULL; head = head->next) {
    if (action(head, key_data, result) != EXIT_SUCCESS)
      break;
  }
}
int find_node(Node *current, void *key_find, void *result) {
  if (strcmp(current->key, (char*)key_find) == 0) {
    Node **res = result;
    *res = current;
    return EXIT_FAILURE; // STOP for
  }
  return EXIT_SUCCESS;
}
int display_node(Node *n, void *key_data, void *res) {
  if (n->par)
    print_node(stdout, n->key, n->info, n->par->key);
  else
    print_node(stdout, n->key, n->info, NULL);

 return EXIT_SUCCESS;
}
int cut_node(Node *node, void *key_data, void *result) {
  if (node->next == NULL)
    return EXIT_FAILURE; // Stop for 

  if (strcmp(node->next->key, (char*)key_data) == 0) {
    Node **res = result;
    *res = node->next;
    node->next = node->next->next;

    (*res)->next = NULL;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int zero_parent(Node *n, void *key_data, void *res) {
  if (n->par == key_data)
    n->par = NULL;

  return EXIT_SUCCESS;
}
int this_child(Node *n, void *key_data, void *res) {
  if (n->par == key_data)
    display_node(n, NULL, NULL);

  return EXIT_SUCCESS;
}

#elif LAB3B

size_t create_node(FILE *binary, char *key, char *info, size_t par,
                   size_t head) {
  // @return: position new node
  Node node;

  fseek(binary, 0, SEEK_END);
  node.offset   = ftell(binary);
  node.len_key  = strlen(key) + 1;
  node.len_info = strlen(info) + 1;
  fwrite(key, sizeof(char), node.len_key, binary);
  fwrite(info, sizeof(char), node.len_info, binary);

  node.parent   = par;
  node.next     = head;
  
  fseek(binary, 0, SEEK_END);
  size_t pos_node = ftell(binary);

//  display_node(binary, &(Iterator){pos_node, node}, NULL, NULL);
  fwrite(&node, sizeof(Node), 1, binary);
  return pos_node;
}
void load_node(Node *node, size_t pos, FILE *from) {
  fseek(from, pos, SEEK_SET);
  fread(node, sizeof(Node), 1, from);
}
char* get_infokey(FILE* file, size_t offset,size_t len) {
	char* infokey = malloc(sizeof(char) * len);
	fseek(file, offset, SEEK_SET);
	fread(infokey, sizeof(char), len, file);
	return infokey;
}
int find_node(FILE *binary, Iterator *i, void *key_data, void *res) {
  char *key  = get_infokey(binary, i->node.offset, i->node.len_key);

  int status = EXIT_SUCCESS;
  if (strcmp(key, (char*)key_data) == 0) {
    size_t *result = res;
    *result = i->pos;
    status = EXIT_FAILURE;
  }

  free(key);
  return status;
}
int display_node(FILE *binary, Iterator *i, void *key_data, void *res) {
  char *key  = get_infokey(binary, i->node.offset, i->node.len_key);
  char *info = get_infokey(binary, i->node.offset + i->node.len_key, i->node.len_info);

  char *par_key = NULL;
  if (i->node.parent) {
    Node parent;
    load_node(&parent, i->node.parent, binary);
    par_key = get_infokey(binary, parent.offset, parent.len_key);
  }

  print_node(stdout, key, info, par_key);
  if (par_key)
    free(par_key);

  free(info);
  free(key);
  return EXIT_SUCCESS;
}
void step_list(FILE *binary, size_t current, void *key_data, void *result, node_foo action) {
  Iterator i = {.pos = current};
  for (; i.pos; i.pos = i.node.next) { 
    load_node(&i.node, i.pos, binary);
    if (action(binary, &i, key_data, result) != EXIT_SUCCESS)
      break;
  }
}
int cut_node(FILE *binary, Iterator *i, void *key_data, void *res) {
  if (i->node.next == 0)
    return EXIT_FAILURE; // Stop for 

  Iterator next = {.pos = i->node.next};
  load_node(&next.node, next.pos, binary);
  if (find_node(binary, &next, key_data, res) == EXIT_FAILURE) {
    // На текущей позиции указатель на следующий из next
    i->node.next = next.node.next;
    fseek(binary, i->pos, SEEK_SET);
    fwrite(&i->node, sizeof(Node), 1, binary);

    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int zero_parent(FILE *binary, Iterator *i, void *key_data, void *res) {
  Iterator parent = {.pos = i->node.parent};
  load_node(&parent.node, parent.pos, binary);

  if (find_node(binary, &parent, key_data, &parent.pos) == EXIT_FAILURE) {
    i->node.parent = 0;
    fseek(binary, i->pos, SEEK_SET);
    fwrite(&i->node, sizeof(Node), 1, binary);
  }

  return EXIT_SUCCESS;
}
bool is_parent(FILE *binary, size_t pos, char *key) {
  if (pos == 0)
    return false;

  bool result = false;
  Node parent;
  load_node(&parent, pos, binary);
  char *par_key = get_infokey(binary, parent.offset, parent.len_key);
  if (strcmp(par_key, (char*)key) == 0) {
    result = true;
  }
  free(par_key);

  return result;
}
int this_child(FILE *binary, Iterator *i, void *key_data, void *res) {
  if (is_parent(binary, i->node.parent, key_data))
    display_node(binary, i, key_data, res);
  /*
  Iterator parent = {.pos = i->node.parent};
  load_node(&parent.node, parent.pos, binary);
  char *par_key = get_infokey(binary, parent.node.offset, parent.node.len_key);
  if (strcmp(par_key, (char*)key_data) == 0) {
    display_node(binary, i, key_data, res);
  }
  free(par_key);
  */

  return EXIT_SUCCESS;
}

#endif // LAB3B
