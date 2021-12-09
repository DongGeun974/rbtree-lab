#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

node_t *make_node(rbtree *t,const key_t key)
{
  node_t *new_node = calloc(1, sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->parent = new_node->left = new_node->right = t->nil;
  return new_node;
}

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *NIL = calloc(1, sizeof(node_t));
  NIL->color = RBTREE_BLACK;
  NIL->key = 0;
  NIL->parent = NIL->right = NIL->left = NULL;

  p->root=NIL;
  p->nil= NIL;

  return p;
}

node_t *all_node_free(rbtree *t, node_t *root)
{
  if (root->left != t->nil)
  {
    free(all_node_free(t, root->left));
  }
  if (root->right != t->nil)
  {
    free(all_node_free(t, root->right));
  }
  return root;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil)
  {
    free(all_node_free(t,t->root));
  }
  free(t->nil);
  free(t);
}

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
        left_rotate(t,z->parent->parent);
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

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  insert_fixup(t, z);
  t->root->color = RBTREE_BLACK;
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t->root;
  while (x != t->nil && key != x->key)
  {
    if (key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }

  if (x->key == key)
  {
    return x;
  }
  return NULL;
  
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
  while(x != t->root && x->color == RBTREE_BLACK)
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

  free(p);

  return 0;
}

void make_array(const rbtree *t, node_t *node, key_t *arr, const key_t n, key_t *idx)
{
  if (*idx >= n || node == t->nil) return;
  make_array(t, node->left, arr, n, idx);
  arr[(*idx)++] = node->key;
  // printf("key %d, idx %d\n",node->key, *idx);
  make_array(t, node->right, arr, n, idx);

}
void _rbtree_to_array(const rbtree *t, node_t * curr, key_t *arr, size_t *pcnt, const size_t n){
    if(curr == t->nil){
        return;
    }
    _rbtree_to_array(t, curr->left, arr, pcnt, n);
    if(*pcnt < n){
        arr[(*pcnt)++] = curr->key;
    }
    else{
        return;
    }
    _rbtree_to_array(t, curr->right, arr, pcnt, n);
}

void node_tree(rbtree *t,node_t *node , int tap)
{
    // for (int i = 0; i < tap; i++)
    // {
    //     printf("\t");
    // }

    if (node != t->nil)
    {
      printf("%p, key : %d[%d], left : %d, right %d\n", node , node->key, node->color, node->left->key, node->right->key);      

      if (node->right != t->nil)
      {
          node_tree(t, node->right, tap+1);
      }
      if (node->left != t->nil)
      {
          node_tree(t, node->left, tap+1);
      }
    } 
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // // TODO: implement to_array
  
  if(t->root == t->nil){
      return 0;
  }
  else{
      key_t a = 0;
      key_t *cnt;
      cnt = &a;
      make_array(t, t->root, arr, n, cnt);
  }
  // node_tree(t, t->root, 0);
  return 0;
}