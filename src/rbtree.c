#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;

  if (y->left != t->nil)
  {
    y->left->parent = x;
  }

  y->parent = x->parent;

  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;
  x->left = y->right;

  if (y->right != t->nil)
  {
    y->right->parent = x;
  }

  y->parent = x->parent;

  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->right)
  {
    x->parent->right = y;
  }
  else
  {
    x->parent->left = y;
  }

  y->right = x;
  x->parent = y;
}

node_t *make_node(rbtree *t,const key_t key)
{
  node_t *new_node = malloc(sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->parent = new_node->left = new_node->right = t->nil;
  return new_node;
}

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *NIL = malloc(sizeof(node_t));
  NIL->color = RBTREE_BLACK;
  NIL->key = 0;
  NIL->parent = NIL->right = NIL->left = NULL;

  p->root=NIL;
  p->nil= NIL;

  return p;
}

void all_node_free(rbtree *t, node_t *root)
{
  if (root->left != t->nil)
  {
    all_node_free(t, root->left);
  }
  if (root->right != t->nil)
  {
    all_node_free(t, root->right);
  }
  free(root);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil)
  {
    all_node_free(t, t->root);
  }
  free(t);
}

void insert_fixup(rbtree *t, node_t *z)
{
  while (z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        // case 1
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->right)
        {
          // case 2
          z = z->parent;
          left_rotate(t, z);
        }
        // case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }
    else
    {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        // case 1
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          // case 2
          z = z->parent;
          right_rotate(t, z);
        }
        // case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y = t->nil;
  node_t *x = t->root;
  node_t *z = make_node(t, key);

  while (x != t->nil)
  {
    y = x;
    if (z->key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }

  z->parent = y;

  if (y == t->nil)
  {
    t->root = z;
  }
  else if (z->key < y->key)
  {
    y->left = z;
  }
  else
  {
    y->right = z;
  }

  insert_fixup(t, z);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if (t->root != t->nil)
  {
    node_t *target_node = t->root;
    while(target_node->right == t->nil && target_node->left == t->nil)
    {
      if (target_node->key > key)
      {
        target_node = target_node->left;
      }
      else
      {
        target_node = target_node->right;
      }
    }

    if (target_node->key == key)
    {
      return target_node;
    }
    else
    {
      return t->nil;
    }
  }
  else
  {
    return t->nil;
  }
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *target_node = t->root;
  while (target_node->left != t->nil)
  {
    target_node = target_node->left;
  } 
  return target_node;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *target_node = t->root;
  while (target_node->right != t->nil)
  {
    target_node = target_node->right;
  }
  return target_node;
}

void trans_plant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;
}
void erase_fixup(rbtree *t, node_t *x)
{
  while(x != t->nil && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      node_t *w = x->parent->right;
      if(w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t,x->parent);
        w=x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color =RBTREE_BLACK;
          w->color = RBTREE_BLACK;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x=t->root;
      }
    }
    else
    {
      node_t *w = x->parent->left;
      if(w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w=x->parent->left;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color =RBTREE_BLACK;
          w->color = RBTREE_BLACK;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x=t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y = p;
  node_t *x;
  int y_original_color = y->color;

  if (p->left == t->nil)
  {
    x = p->right;
    trans_plant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    x = p->left;
    trans_plant(t, p, p->left);
  }
  else
  {
    y = y->right;
    while(y->left != t->nil)
    {
      y = y->left;
    }
    y_original_color = y->color;

    x = y->right;

    if (y->parent == p)
    {
      x->parent = y;
    }
    else
    {
      trans_plant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    trans_plant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_original_color == RBTREE_BLACK)
  {
    erase_fixup(t, x);
  }

  free(x);

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *tmp;
  int a;
  for (int i = 0; i < n; i++)
  {
    tmp = rbtree_insert(t,arr[i]);
  }

  for (int i = 0; i < n; i++)
  {
    tmp = rbtree_min(t);
    a = rbtree_erase(t,tmp);
    arr[i] = tmp->key;
  }
  return arr;
}
