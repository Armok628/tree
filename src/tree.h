#ifndef TREE_H
#define TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct node {
	long k;
	void *v;
	struct node *l,*r;
	bool c;
} node_t;

node_t *new_node(long,void *,bool);
void insert(node_t **,long,void *);
#endif
