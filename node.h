#ifndef NODE_H
#define NODE_H

struct Node_t {
	char *key;
	char *info;
	struct Node_t* par;
	struct Node_t* next;
};

typedef struct Node_t Node;
typedef int (*node_foo)(Node*, void*, void*);

Node* create_node(char *key, char *info, Node *par);
void delete_node(Node *node);

void delete_list_node(Node **node);
void step_list(Node* head, void *key_data, void *result,node_foo action);

int find_node(Node *current, void *key_find, void *result);
int cut_node(Node *node, void *key_data, void *result);

// ToDo: форматированный вывод в файл
int display_node(Node *n, void *key_data, void *res);
int zero_parent(Node *n, void *key_data, void *res);
int this_child(Node *n, void *key_data, void *res);

#endif // NODE_H
