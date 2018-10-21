#include "tree.h"
enum {BLACK,RED};
void rrot(node_t **p)
{
	node_t *n=*p,*l=n->l;
	n->c=l->c;
	l->c=RED;
	n->l=l->r;
	l->r=n;
	*p=l;
}
void lrot(node_t **p)
{
	node_t *n=*p,*r=n->r;
	n->c=r->c;
	r->c=BLACK;
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
static inline bool red(node_t *n)
{
	return n&&n->c;
}
static inline bool black(node_t *n)
{
	return !n||!n->c;
}
void fix(node_t **p)
{
	node_t *n=*p;
	if (red(n->l))
		rrot(p);
	else if (red(n->r)&&red(n->r->r))
		lrot(p);
	n=*p;
	if (red(n->r)&&red(n->l)) {
		n->c=!n->c;
		n->l->c=!n->l->c;
		n->r->c=!n->r->c;
	}
}
void r_insert(node_t **p,long k,void *v)
{
	node_t *n=*p;
	if (!n) {
		*p=new_node(k,v,RED);
		return;
	} else if (k==n->k) {
		n->v=v;
		return;
	} else if (k<n->k)
		r_insert(&n->l,k,v);
	else
		r_insert(&n->r,k,v);
	fix(p);
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
node_t *cut_max(node_t **p)
{
	node_t *n=*p;
	if (n->r)
		n=cut_max(&n->r);
	else {
		*p=NULL;
		return n;
	}
	fix(p);
	return n;
}
void expunge(node_t **p,long k)
{
	node_t *n=*p;
	if (!n)
		return;
	if (k==n->k) {
		if (n->l&&n->r) {
			*p=cut_max(&n->l);
			(*p)->c=n->c;
			(*p)->r=n->r;
			(*p)->l=n->l;
		} else if (n->r)
			*p=n->r;
		else if (n->l)
			*p=n->l;
		else
			*p=NULL;
		free(n);
	} else if (k>n->k)
		expunge(&n->r,k);
	else
		expunge(&n->l,k);
	if (*p)
		fix(p);
}
