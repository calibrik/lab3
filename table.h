#ifndef TABLE_H
#define TABLE_H

#include "node.h"

#define OK 0
#define NOTHING_TABLE 1
#define INCORRECT_PARENT 2
#define INCORRECT_KEY 3

typedef enum {
  false, true
} bool;

struct Table {
  Node *head;
};

struct Table* create_table();
void delete_table(struct Table* table);

int search_table(struct Table *table, char *key);
int add_table (struct Table *table, char *key, char *info,char *par_key);
int del_key(struct Table *table, char *key, bool needZeroParent);
int find_all_child(struct Table *table, char *key);
int step_table(struct Table *table, node_foo action);

#endif // TABLE_H
