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
enum {OK,CHILD,PARENT,GRANDPARENT};
int insert(node_t **p,long k,void *v)
{
	static node_t *l=NULL; // Last inserted
	node_t *n=*p;
	int problem;
	if (k==n->k) {
		printf("Found.\n");
		n->v=v;
		return OK;
	} else if (k<n->k) {
		printf("Left. ");
		if (!n->l) {
			printf("Leaf.\n");
			l=new_node(k,v,RED);
			n->l=l;
			problem=n->c?PARENT:OK;
		} else
			problem=insert(&n->l,k,v);
	} else {
		printf("Right. ");
		if (!n->r) {
			printf("Leaf.\n");
			l=new_node(k,v,RED);
			n->r=l;
			problem=n->c?PARENT:OK;
		} else
			problem=insert(&n->r,k,v);
	}
	n=*p; // Update node in case rotations have occurred.
	switch (problem) { // Act based on who detects a problem
	case OK:
		printf("%ld receives OK signal\n",n->k);
		return OK;
	case CHILD: // return this if possible issue with parent
		printf("%ld receives report of possible issue with child.\n",n->k);
		if (red(n)) {
			printf("Issue detected; delegating to grandparent of child.\n");
			return GRANDPARENT;
		} else {
			printf("No issue detected.\n");
			return OK;
		}
	case PARENT: // return this if issue between self and child
		printf("%ld detects issue between self and child; delegating.\n",n->k);
		// delegate to grandparent for possible rotation
		return GRANDPARENT;
	case GRANDPARENT: // received if issue between child and grandchild
		printf("%ld receives report of issue between child and grandchild.\n",n->k);
		if (black(n)&&red(n->l)&&red(n->r)) {
			printf("%ld moves black down to fix excess red on path.\n",n->k);
			n->c=RED;
			n->l->c=BLACK;
			n->r->c=BLACK;
			return CHILD;
		} else if (red(n->l)^red(n->r)) {
			printf("%ld performs rotations to fix excess red on path.\n",n->k);
			if (red(n->r)&&red(n->r->l)) {
				printf("Rotate right to push outside. ");
				rrot(&n->r);
			} else if (red(n->l)&&red(n->l->r)) {
				printf("Rotate left to push outside. ");
				lrot(&n->l);
			}
			if (red(n->l)&&red(n->l->l)) {
				printf("Rotating self right.\n");
				rrot(p);
			} else if (red(n->r)&&red(n->r->r)) {
				printf("Rotating self left.\n");
				lrot(p);
			}
			return (*p)->c?CHILD:OK;
		}
	}
	return 0;
}
