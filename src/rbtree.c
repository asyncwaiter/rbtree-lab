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
	x->left = y->right;

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

void delete_subtree(rbtree *t, node_t *node) {
    if (node == t->nil) {
      return;
    }
    delete_subtree(t, node->left);
    delete_subtree(t, node->right);
    free(node);
  }

void delete_rbtree(rbtree *t) {
  if (t == NULL) {
    return;
  }
  delete_subtree(t, t->root);
  free(t->nil);
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
  node_t *current = t->root;
  if (current == t->nil) {
    return NULL;
  }

  while (current->left != t->nil) {
    current = current->left;
  }

  return current;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *current = t->root;
  if (current == t->nil) {
    return NULL;
  }

  while (current->right != t->nil) {
    current = current->right;
  }
  return current;
}


void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rbtree_erase_fixup(rbtree *t, node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) { 
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    } else {
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED) { 
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {  
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) { 
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

node_t *rbtree_minimum(rbtree *t, node_t *x) {
  while (x->left != t->nil) {
    x = x->left;
  }
  return x;
}


int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z;
  node_t *x;
  color_t y_original_color = y->color;

  if (z->left == t->nil) {
    x = z->right;
    rbtree_transplant(t, z, z->right);
  } else if (z->right == t->nil) {
    x = z->left;
    rbtree_transplant(t, z, z->left);
  } else {
    y = rbtree_minimum(t, z->right);
    y_original_color = y->color;
    x = y->right;

    if (y->parent == z) {
      x->parent = y;
    } else {
      rbtree_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    rbtree_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  free(z);

  if (y_original_color == RBTREE_BLACK) {
    rbtree_erase_fixup(t, x);
  }

  return 0;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
