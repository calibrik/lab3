#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>

// Generall

typedef struct Node_t Node;

typedef enum {
  false, true
} bool;

#ifdef LAB3A

struct Node_t {
	char *key;
	char *info;
	struct Node_t* par;
	struct Node_t* next;
};


Node* create_node(char *key, char *info, Node *par);
void delete_node(Node *node);

void delete_list_node(Node **node);

typedef int (*node_foo)(Node*, void*, void*);
void step_list(Node* head, void *key_data, void *result,node_foo action);

int find_node(Node *current, void *key_find, void *result);
int cut_node(Node *node, void *key_data, void *result);

// ToDo: форматированный вывод в файл
int display_node(Node *n, void *key_data, void *res);
int zero_parent(Node *n, void *key_data, void *res);
int this_child(Node *n, void *key_data, void *res);

#elif LAB3B

struct Node_t {
  size_t offset;	
  size_t len_key;
  size_t len_info;
  size_t parent;
  size_t next;
};

typedef struct Iterator_t Iterator;
struct Iterator_t {
  size_t prev;
  size_t pos;
  Node node;
};

size_t create_node(FILE *binary, char *key, char *info, size_t par,
                   size_t head);

typedef int (*node_foo)(FILE*, Iterator*, void*, void*);

void load_node(Node *node, size_t pos, FILE *from);
char* get_infokey(FILE* file, size_t offset,size_t len);

bool is_parent(FILE *binary, size_t pos, char *key);
int cut_node(FILE *binary, Iterator *i, void *key_data, void *res);
int find_node(FILE *binary, Iterator *i, void *key_data, void *res);
int display_node(FILE *binary, Iterator *i, void *key_data, void *res);
void step_list(FILE *binary, size_t head, void *key_data, void *result, node_foo action);
int zero_parent(FILE *binary, Iterator *i, void *key_data, void *res);
int this_child(FILE *binary, Iterator *i, void *key_data, void *res);
#endif // LAB3B

#endif // NODE_H
