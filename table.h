#ifndef TABLE_H 
#define TABLE_H

#include "node.h"

enum StatusProgram {
  OK,
  NOTHING_TABLE,
  INCORRECT_PARENT,
  INCORRECT_KEY
};

#ifdef LAB3A

struct Table {
  Node *head;
};

struct Table* create_table();

#elif LAB3B

struct Table {
  FILE  *binary;
  size_t head;
};

struct Table* create_table(char const *filename);

#endif // LAB3B

struct Table;

// create_table()
void delete_table(struct Table* table);
int search_table(struct Table *table, char *key);
int add_table (struct Table *table, char *key, char *info,char *par_key);
int del_key(struct Table *table, char *key, bool needZeroParent);
int find_all_child(struct Table *table, char *key);
int step_table(struct Table *table, node_foo action);
// End general
int recursia_del_family(struct Table *table,char* key);

#endif // TABLE_H
