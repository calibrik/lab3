#include "table.h"
#include <string.h>



#ifdef LAB3A

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

#elif LAB3B

static bool load_state_table(struct Table *table, char const *filename) {
  // @return: 
  //         true: ALL_RIGHT
  //         false: not found binary-file
  table->binary = fopen(filename, "rb+");
  if (table->binary == NULL)
    return false;

  fseek(table->binary, 0, SEEK_SET);
  fread(&table->head, sizeof(size_t), 1, table->binary);

  return true;
}
static void save_state_table(struct Table const *table) {
  fseek(table->binary, 0, SEEK_SET);
  fwrite(&table->head, sizeof(size_t), 1, table->binary);
}
struct Table* create_table(char const *filename) {
  struct Table *table = malloc(sizeof(struct Table));

  if (load_state_table(table, filename) == false) {
    // new_binary_file
    table->binary = fopen(filename, "wb+");
    table->head   = 0;
    save_state_table(table);
  }

  return table;
}
void delete_table(struct Table *table) {
  if (table) {
    save_state_table(table);
    fclose(table->binary);
    free(table);
  }
}
int add_table(struct Table *table, char *key, char *info, char *par_key) {
  // check_duplicate
  size_t duplicate = 0;
  step_list(table->binary, table->head, key, &duplicate, find_node);
  if (duplicate)
    return INCORRECT_KEY;
  
  // find_parent
  size_t parent = 0;
  if (table->head != 0) {
    step_list(table->binary, table->head, par_key, &parent, find_node);
    if (parent == 0)
      return INCORRECT_PARENT;
  }

  // add_new_node
  table->head = create_node(table->binary, key, info, parent, table->head);
  
  return OK;
}
int delete_children(Node *n, void *key_data, void *res) {

  return EXIT_SUCCESS;
}
int del_key(struct Table *table, char *key, bool needZeroParent) {
  /*Empty*/
  if (table->head == 0)
    return EXIT_FAILURE;
  
  if (needZeroParent == false) {
    recursia_del_family(table, key);
    return 0;
  }

  size_t trash = 0;

  Iterator head = {.pos = table->head};
  load_node(&head.node, head.pos, table->binary);
  if (find_node(table->binary, &head, key, &head.pos) == EXIT_FAILURE) {
    trash = table->head;
    table->head = head.node.next;
  }
  else {
    step_list(table->binary, table->head, key, &trash, cut_node);
  }

  if (trash == 0)
    return INCORRECT_KEY;
    
  Node trash_node;
  load_node(&trash_node, trash, table->binary);
  char *trash_key = get_infokey(table->binary, trash_node.offset, trash_node.len_key);
  if (needZeroParent == true) {
    step_list(table->binary, table->head, trash_key, NULL, zero_parent);
  }
  else {
    /*
    Iterator i = {.pos = table->head};
    while (i.pos) {
      load_node(&i.node, i.pos, table->binary);
      if (is_parent(table->binary, i.pos, trash_key)) {
        char *key_par = get_infokey(table->binary, i.node.offset, trash_node.len_key);
        i.pos = i.node.next;
        del_key(table, key_par, false);
        free(key_par);
      }
      else {
        i.pos = i.node.next;
      }
    }
    */
  }

  free(trash_key);
  return OK;
} 
int search_table(struct Table *table, char* key) {
  Iterator res = {0};
  step_list(table->binary, table->head, key, &res.pos, find_node);
  if (res.pos == 0)
    return INCORRECT_KEY;
  
  load_node(&res.node, res.pos, table->binary);
  display_node(table->binary, &res, NULL, NULL);
  return OK;
}
int step_table(struct Table *table, node_foo action) {
  step_list(table->binary, table->head, NULL, NULL, action);
  return OK;
}
int find_all_child(struct Table *table, char *key) {
  if (table->head == 0)
    return NOTHING_TABLE;

  step_list(table->binary, table->head, key, NULL, this_child);
  return OK;
}

int del_childs(struct Table *table,char* key)
{
  /*
  Iterator i = {.pos = current};
  for (; i.pos; i.pos = i.node.next) { 
    load_node(&i.node, i.pos, binary);
    char *key_child=get_infokey(table->binary,i.node.offset,i.node.len_key);
    if (strcmp(key,key_child)==0) del_childs(table,key_child);
    //удалить нод
    free(key_child);
    */
  return 0;
}
char* get_parent_key(FILE *binary, size_t pos_parent) {
  if (pos_parent == 0) {
    return NULL;
  }

  Node parent;
  load_node(&parent, pos_parent, binary);
  char *key_parent = get_infokey(binary, parent.offset,
                           parent.len_key);

  return key_parent;
}
void del_by_parent(struct Table *table, char *key) {
  if (table->head == 0) {
    return;
  }

  Iterator iterator = {.prev = 0, .pos = table->head};
  load_node(&iterator.node, iterator.pos, table->binary);

  char *key_curr = get_infokey(table->binary, iterator.node.offset, 
                               iterator.node.len_key);

  /*get_infokey(table->binary, iterator.node.offset,
                            iterator.node.len_key);
                            */


  if (iterator.node.parent != 0) {
    /*
    Node parent;
    load_node(&parent, iterator.node.parent, table->binary);
    char *key_parent = get_infokey(table->binary, parent.offset,
                            parent.len_key);

                            */
    char *key_parent = get_parent_key(table->binary,
                                      iterator.node.parent);
    if (strcmp(key_parent, key) == 0) { // нашли ПЕРВЫЙ эл-т
      table->head = iterator.node.next;
      del_by_parent(table, key_curr);
      load_node(&iterator.node, iterator.pos, table->binary); 
    }
    free(key_parent);
  }
  while (iterator.node.next != 0) {
    free(key_curr);
    iterator.prev = iterator.pos;
    iterator.pos  = iterator.node.next;

    load_node(&iterator.node, iterator.pos, table->binary);

    char *key_parent = get_parent_key(table->binary,
                                      iterator.node.parent);
    if (key_parent == NULL) {
      continue; // нету родителя!
    }
    if (strcmp(key_parent, key) == 0) { // нашли ПЕРВЫЙ эл-т
      // отсоединить от предыдущего значения
      Node prev_node;
      load_node(&prev_node, iterator.prev, table->binary);
      
      prev_node.next = iterator.node.next;

      fseek(table->binary, iterator.prev, SEEK_SET);
      fwrite(&prev_node, sizeof(Node), 1, table->binary);

      key_curr = get_infokey(table->binary, iterator.node.offset, 
                             iterator.node.len_key);
      del_by_parent(table, key_curr);

      load_node(&iterator.node, iterator.pos, table->binary); 
    }
  }

  free(key_curr);
}
int recursia_del_family(struct Table *table,char* key)
{
  Iterator iterator = {.prev = 0, .pos = table->head};
  load_node(&iterator.node, iterator.pos, table->binary);

  char *key_it = get_infokey(table->binary, iterator.node.offset,
                            iterator.node.len_key);

  if (strcmp(key_it, key) == 0) { // нашли ПЕРВЫЙ эл-т
    table->head = iterator.node.next;
    // запустить точку входа в рекурсию
    del_by_parent(table, key_it);
  } else {

    while (iterator.node.next != 0) {
      free(key_it);

      iterator.prev = iterator.pos;
      iterator.pos  = iterator.node.next;

      load_node(&iterator.node, iterator.pos, table->binary);
      key_it = get_infokey(table->binary, iterator.node.offset,
                            iterator.node.len_key);

      if (strcmp(key_it, key) == 0) {
        //  подгрузить предыдущий элемент и сделать его указатель
        Node prev_node;
        load_node(&prev_node, iterator.prev, table->binary);
        // отвязать от списка
        //        next на (Найденный), следующим
        prev_node.next = iterator.node.next;
        // записать измененный нод в файл
        fseek(table->binary, iterator.prev, SEEK_SET);
        fwrite(&prev_node, sizeof(Node), 1, table->binary);

        del_by_parent(table, key_it);
        break;
      }
    }
  }
  free(key_it);
  return OK;
}

#endif // LAB3B
