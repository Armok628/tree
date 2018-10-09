#include "tree.h"
void rrot(node_t **p)
{
	node_t *n=*p,*l=n->l;
	bool c=n->c;
	n->c=l->c;
	l->c=c;
	n->l=l->r;
	l->r=n;
	*p=l;
}
void lrot(node_t **p)
{
	node_t *n=*p,*r=n->r;
	bool c=n->c;
	n->c=r->c;
	r->c=c;
	n->r=r->l;
	r->l=n;
	*p=r;
}
node_t *new_node(long k,void *v,bool c)
{
	node_t *n=malloc(sizeof(node_t));
	n->k=k;
	n->v=v;
	n->c=c;
	n->l=NULL;
	n->r=NULL;
	return n;
}
enum {BLACK,RED};
static inline bool red(node_t *n)
{
	return n&&n->c;
}
static inline bool black(node_t *n)
{
	return !n||!n->c;
}
int insert(node_t **p,long k,void *v)
{
	static node_t *l=NULL;
	node_t *n=*p; // n is possible parent to added node
	printf("Inserting k=%ld.\n",k);
	int i=0;
	if (k==n->k) {
		n->v=v;
	} else if (k<n->k) {
		printf("Looking left.\n");
		if (!n->l) {
			printf("Found leaf; inserting.\n");
			l=new_node(k,v,RED);
			n->l=l;
			return n->c==RED;
		} else
			i=insert(&n->l,k,v);
	} else /*(k>n->k)*/ {
		printf("Looking right.\n");
		if (!n->r) {
			printf("Found leaf; inserting.\n");
			l=new_node(k,v,RED);
			n->r=l;
			return n->c==RED;
		} else
			i=insert(&n->r,k,v);
	}
	if (i) {
		printf("Red parent has red child.\n");
		if (black(n)&&red(n->l)&&red(n->r)) {
			printf("Push black down from grandparent.\n");
			n->c=RED;
			if (n->l)
				n->l->c=BLACK;
			if (n->r)
				n->r->c=BLACK;
		} else if (red(n->l)^red(n->r)) {
			printf("Rotate to fix extra redness. ");
			// Step 1
			if (n->l&&l==n->l->r) {
				printf("Rotate left to move outward. ");
				lrot(&n->l);
			} else if (n->r&&l==n->r->l) {
				printf("Rotate right to move outward. ");
				rrot(&n->r);
			}
			// Step 2
			if (l==n->l||(n->l&&l==n->l->l)) {
				printf("Rotate right.\n");
				rrot(p);
			} else if (l==n->r||(n->r&&l==n->r->r)) {
				printf("Rotate right.\n");
				lrot(p);
			}
		}
	}
	return 0;
}
