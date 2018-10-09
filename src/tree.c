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
enum detector {OK,CHILD,PARENT,GRANDPARENT};
int r_insert(node_t **p,long k,void *v)
{
	static node_t *l=NULL; // Last r_inserted
	node_t *n=*p;
	enum detector d; // detector
	if (k==n->k) {
		n->v=v;
		return OK;
	} else if (k<n->k) {
		if (!n->l) {
			l=new_node(k,v,RED);
			n->l=l;
			d=n->c?PARENT:OK; // "If I'm red like my child, I have a problem."
		} else
			d=r_insert(&n->l,k,v); // "There is a problem if my child reports one."
	} else {
		if (!n->r) {
			l=new_node(k,v,RED);
			n->r=l;
			d=n->c?PARENT:OK; // "If I'm red like my child, I have a problem."
		} else
			d=r_insert(&n->r,k,v); // "There is a problem if my child reports one."
	}
	switch (d) {
	case OK: // "My descendants have reported that all is well."
		return OK;
	case CHILD: // "My child has reported a possible issue with me."
		if (red(n))
			return GRANDPARENT; // "They are right. Grandparent, what can you do?"
		else
			return OK; // "They are wrong. Everything is OK."
	case PARENT: // "I have noticed an issue with my child."
		return GRANDPARENT; // "Grandparent, what can you do?"
	case GRANDPARENT: // "My child has reported an issue with their child."
		if (red(n->l)&&red(n->r)) { // Two red children; push black down
			n->c=RED;
			n->l->c=BLACK;
			n->r->c=BLACK;
			return CHILD; // "Now I may have a problem with my parent."
		} else if (red(n->l)^red(n->r)) { // One red, one black; perform rotations
			if (red(n->r)&&red(n->r->l))
			// Rotate outwards if necessary
				rrot(&n->r);
			else if (red(n->l)&&red(n->l->r))
				lrot(&n->l);
			// Rotate self downward
			if (red(n->l)&&red(n->l->l))
				rrot(p);
			else if (red(n->r)&&red(n->r->r))
				lrot(p);
			return OK; // "Everything should be okay now."
		}
	}
	return 0;
}
void insert(node_t **p,long k,void *v)
{
	r_insert(p,k,v);
	(*p)->c=BLACK; // Make sure the root is a black node
}
void *lookup(node_t *n,long k)
{
	while (n&&k!=n->k) {
		if (k<n->k)
			n=n->l;
		else
			n=n->r;
	}
	if (n)
		return n->v;
	else
		return NULL;
}
