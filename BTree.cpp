#include "BTree.h"


Node::Node(int N, bool isL) 
    : parent(nullptr)
    , numKeys(2 * N)
    , isLeaf(isL)
{}

// inserts a key into the keys vector of a node in order
// returns the integer position of the newly inserted key 
int Node::keyInsertion(int key)
{
  int size = keys.size();
  for (int i = 0; i < size; i++)
  {
    if (key < keys[i])
    {
      keys.insert(keys.begin() + i, key);
      return i;
    }
  }
  // if key > all keys, then insert at back
  keys.push_back(key);
  return keys.size() - 1;
}

// splits an overflowed node into 2 subnodes; hooks those subnodes up to the parent
void Node::Split()
{
  // create 2 nodes (left and right) to split the overfowed node
  bool isChildLeaf = (this->isLeaf) ? true : false;
  shared_ptr<Node> left = make_shared<Node>(Node(this->numKeys / 2, isChildLeaf));
  shared_ptr<Node> right = make_shared<Node>(Node(this->numKeys / 2, isChildLeaf));
  
  // find midPoint, m
  int midPoint = this->numKeys / 2;
  
  for (int i = 0; i < midPoint; i++)
  {
    // left node has subvector 0 to m-1
    left->keys.push_back(this->keys[i]);
    // right node has subvector m + 1 to size - 1
    right->keys.push_back(this->keys[midPoint + i + 1]);
    if (this->children.size())
    {
      left->children.push_back(this->children[i]);
      right->children.push_back(this->children[midPoint + i + 1]);
    }
  }
  if (this->children.size())
  {
    left->children.push_back(this->children[midPoint]);
    right->children.push_back(this->children[this->children.size() - 1]);
  }
  
  // hook up split nodes to parent
  shared_ptr<Node> parent = this->parent;
  left->parent = right->parent = parent;

  // insert key into parent keys vector
  int midKey = this->keys[midPoint];
  int insertionPos = parent->keyInsertion(midKey);
  // remove ith child from parent (ith child is the node that has been split)
  if (parent->children.size())
    parent->children.erase(parent->children.begin() + insertionPos);
  // left node (left subvector) corresponds to the ith child of the parent
  parent->children.insert(parent->children.begin() + insertionPos, left);
  // right node (right subvector) corresponds to the (i+1)th child of the parent
  parent->children.insert(parent->children.begin() + insertionPos + 1, right);
}

void Node::printKeys()
{
  cout << "\t< ";
  for (auto itr = keys.begin(); itr != keys.end(); ++itr)
    cout << *itr << ", ";
  cout << ">";
}



BTree::BTree()
    : root(nullptr)
    , order(1)
{}

BTree::BTree(int n) 
    : root(nullptr)
    , order(n) 
{}

shared_ptr<Node> BTree::findInsertionNode(int key)
{
  // find the correct node to insert too
  shared_ptr<Node> currNode = root;
  // descend the tree until we reach a leaf
  while (!currNode->isLeaf) 
  {
    int size = currNode->keys.size();
    if (key < currNode->keys.front()) // less than all keys, so descend far left
      currNode = currNode->children[0];
    else if (key > currNode->keys.back()) // greater than all keys, so descend far right
      currNode = currNode->children[size];
    else
    {
      for (int i = 1; i < size; i++)
      {
        // loop until we find the first key in the node greater than the new key. Descend to ith child
        if (key < currNode->keys[i])
        {
          currNode = currNode->children[i];
          break;
        }
      }
    }
  }
  return currNode;
}

void BTree::Insert(int key) 
{
  // if B-Tree is empty
  if (root == nullptr)
  {
    shared_ptr<Node> newNode = make_shared<Node>(Node(order, true));
    newNode->keys.push_back(key);
    root = newNode;
    return;
  }

  // perform insertion
  shared_ptr<Node> insertionNode = findInsertionNode(key);
  insertionNode->keyInsertion(key);
  
  // check if any nodes have overflowed due to the insertion and splits
  shared_ptr<Node> itr = insertionNode;
  while (itr != root)
  {
    // perform a split since we've overflowed our allowed num of keys
    if (insertionNode->keys.size() > insertionNode->numKeys)
      insertionNode->Split();
    // ascend to the parent to check if it has overflowed too
    itr = itr->parent;
  }
  // if root has overflowed, we need to create a parent since root's parent is nullptr
  if (root->keys.size() > root->numKeys)
  {
    shared_ptr<Node> parent = make_shared<Node>(Node(order, false));
    root->parent = parent;
    root->Split();
    root = parent;
  }
}

void BTree::printTree(shared_ptr<Node> currNode)
{
  int size = currNode->children.size();
  for (int i = 0; i < size; i++)
  {
    if (!currNode->isLeaf)
      printTree(currNode->children[i]);
    cout << "\t" << currNode->keys[i];
  }
  if (!currNode->isLeaf)
    printTree(currNode->children[size]);    
}


