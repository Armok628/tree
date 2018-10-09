#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/tree.h"
void print_tree(node_t *n,int d)
{
	for (int i=0;i<d;i++)
		putchar('|');
	printf("%ld (%s)\n",n->k,n->c?"RED":"BLACK");
	if (n->l) {
		putchar('L');
		print_tree(n->l,d+1);
	}
	if (n->r) {
		putchar('R');
		print_tree(n->r,d+1);
	}
}
void print_traverse(node_t *n)
{
	if (n->l)
		print_traverse(n->l);
	printf("%ld\n",n->k);
	if (n->r)
		print_traverse(n->r);
}
int main(int argc,char **argv)
{
	srand(time(NULL));
	node_t *n=new_node(0,NULL,0);
	printf("Starting with %ld\n",n->k);
	for (int i=0;i<=37;i++)
		insert(&n,i,NULL);
	print_tree(n,1);
	printf("sizeof(node_t): %lu\n",sizeof(node_t));
	return 0;
}
