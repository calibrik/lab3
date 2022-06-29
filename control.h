#ifndef CONTROL_H
#define CONTROL_H

#include "table.h"

struct Control {
	struct Table* table;
};


struct Control* create_control();
void delete_control(struct Control *control);

void search(struct Control *control);
void add(struct Control *control);
void del(struct Control *control);
void display(struct Control *control);
void del_family(struct Control *control);
void table_par(struct Control *control);

#endif
