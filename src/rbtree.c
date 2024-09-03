#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->nil = nil;
  p->root = nil;
  return p;
}


void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left;
  
  if (y->left != t->nil)
    y->left->parent = x;
  y->parent = x->parent;
  
  if(x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x){
	node_t *y = x->left;
	x->left = y->left;

	if (y->right != t->nil)
    y->right->parent = x;
  y->parent = x->parent;

	if (x->parent == t->nil)
    t->root = y;
	else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  y->right = x;
  x->parent = y;
			
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    } else {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *z = (node_t *)malloc(sizeof(node_t));
  z->key = key;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  
  node_t *y = t->nil;
  node_t *x = t->root;
  while(x != t->nil){
	  y=x;
	  if (z->key < x->key)
		  x = x->left;
		else
			x = x->right;
  }
  
  z->parent = y;
  if (y == t->nil)
	  t->root = z;
	else if (z->key < y->key)
		y->left = z;
	else
		y->right = z;
		
	rbtree_insert_fixup(t, z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *current = t->root;
  while(current != t->nil){
    if (key < current->key)
      current = current->left;
    else if (key > current->key)
      current = current->right;
    else
      return current;
  }
  return NULL;
}


node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
