#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "handle.h"


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
  printf("K: %-20s | I: %-20s ", n->key, n->info);
  if (n->par)
    printf("P: %-20s\n", n->par->key);
  else
    printf("P: (NULL)\n");

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
