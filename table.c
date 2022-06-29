#include "table.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



struct Table* create_table() {
  struct Table *table = malloc(sizeof(struct Table));
  table->head = NULL;
  return table;
}
void delete_table(struct Table *table) {
  if (table) {
    delete_list_node(&table->head);
    free(table);
  }
}
int add_table(struct Table *table, char *key, char *info, char *par_key) {
  Node *key_exist = NULL;
  step_list(table->head, key, &key_exist, find_node);
  if (key_exist != NULL) {
      return INCORRECT_KEY;
  }

  Node *parent = NULL;
  if (table->head != NULL) {
    step_list(table->head, par_key, &parent, find_node);
    if (parent == NULL)
      return INCORRECT_PARENT;
  }

  Node *new_node = create_node(key, info, parent);
  new_node->next = table->head;
  table->head = new_node;

  return OK;
}
int delete_children(Node *n, void *key_data, void *res) {
  if (n->par == key_data)
    del_key(res, n->key, true);

  return EXIT_SUCCESS;
}
int del_key(struct Table *table, char *key, bool needZeroParent) {
  if (table->head == NULL)
    return NOTHING_TABLE;
  
  Node *trash = NULL;
  if (strcmp(table->head->key, key) == 0) {
    trash = table->head;
    table->head = table->head->next;
  }
  else {
    step_list(table->head, key, &trash, cut_node);
  }

  if (trash == NULL)
    return INCORRECT_KEY;

  if (needZeroParent == true) {
    step_list(table->head, trash, NULL, zero_parent);
  }
  else {
    Node *curr = table->head;
    while (curr != NULL) {
      if (curr->par == trash) {
        Node *t = curr;
        curr = curr->next;
        del_key(table, t->key, false);
      }
      else
        curr = curr->next;
    }
  }

  delete_node(trash);

  return OK;
} 
int search_table(struct Table *table, char* key)
{
  Node *res = NULL;
  step_list(table->head, key, &res, find_node);
  if (res == NULL)
    return INCORRECT_KEY;

	display_node(res, NULL, NULL);

	return OK;
}
int step_table(struct Table *table, node_foo action) {
  step_list(table->head, NULL, NULL, action);
  return OK;
}
int find_all_child(struct Table *table, char *key) {
  if (table->head == NULL)
    return NOTHING_TABLE;

  Node *parent = NULL;
  step_list(table->head, key, &parent, find_node);
  if (parent == NULL)
    return INCORRECT_PARENT;

  step_list(table->head, parent, NULL, this_child);
  return OK;
}
