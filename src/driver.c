#include "rbtree.h"
#include <stdio.h>


int main(int argc, char *argv[]) {
    rbtree *t = new_rbtree();
  key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  for (int i = 0; i < n; i++)
  {
    rbtree_insert(t, entries[i]);
  }
  rbtree_to_array(t, entries, n);
  for (int i = 0; i <n; i++)
  {
      printf("%d ", entries[i]);
  }
  delete_rbtree(t);
  if (t == NULL)
  {
      printf("a");
  }
  return 0;
}