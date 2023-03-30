
#include "BTree.h"
#include <iostream>

int main()
{
  BTree btree(1);
  int arr[] = { 1, 2, 3, 4, 9, 8, 7, 6, 5 };
  for (auto &elem : arr)
    btree.Insert(elem);
  
  shared_ptr<Node> root = btree.getRoot();
  //btree.printTree(root);
  return 0;
}
