#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class BTree;

class Node
{
  friend class BTree;
  private:
    shared_ptr<Node> parent;
    vector<int> keys;
    vector<shared_ptr<Node>> children;
    int numKeys;
    bool isLeaf;
  public:
    Node(int, bool);
    int keyInsertion(int);
    void Split();
    void printKeys();
};

class BTree
{
  private:
    shared_ptr<Node> root;
    int order;
  public:
    BTree();
    BTree(int);
    shared_ptr<Node> findInsertionNode(int);
    void Insert(int);
    shared_ptr<Node> getRoot() { return root; }
    void printTree(shared_ptr<Node>);
};

