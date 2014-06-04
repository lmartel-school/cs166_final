#include "rope.h"
#include "splay_tree.h"
#include <assert.h>
#include <stack>
#include <iostream>

typedef SplayTree::TreeNode TreeNode;
using namespace std;

Rope::Rope() {
  this->tree = new SplayTree(NULL);
}

Rope::Rope(SplayTree *tree) {
  this->tree = tree;
}

Rope::~Rope(){
  if(tree != NULL) delete tree;
}

int Rope::length(){
  // cout << tree->toString() << endl;
  return sumWeightsDownRightSpine(tree->root);
}

char Rope::at(int index){
  int localIndex;
  TreeNode *node = this->findNodeWithCharAtIndex(this->tree->root, index, localIndex);
  return node->value->at(localIndex);
}

string Rope::report(int start, int end){
  if (end <= start) return "";

  int localStart;
  int localEnd;
  TreeNode *startNode = this->findNodeWithCharAtIndex(this->tree->root, start, localStart);
  TreeNode *endNode = this->findNodeWithCharAtIndex(this->tree->root, end - 1, localEnd);
  if (startNode == endNode){
    return startNode->value->substr(start, end - start);
  }

  TreeNode *lca = this->findNodeToReport(tree->root, start, end);
  // cout << "lca:" << lca->toString() << endl;

  string res = startNode->value->substr(localStart);
  res += inOrder(lca, startNode, endNode);
  res += endNode->value->substr(0, 1 + localEnd);

  lca->splay(tree);

  return res;
}

Rope *Rope::concat(Rope *right){
  Rope *left = this;
  TreeNode *newRoot = new TreeNode(NULL, left->length(), new string(""));
  newRoot->setLeft(left->tree->root);
  newRoot->setRight(right->tree->root);

  return new Rope(new SplayTree(newRoot));
}

pair<Rope *, Rope *> Rope::split(int index){
<<<<<<< HEAD
  if (tree->root == NULL) return make_pair(new Rope(new SplayTree(NULL)), new Rope(new SplayTree(NULL)));
  if (index < 0) return make_pair(new Rope(new SplayTree(NULL)), this);
  int localIndex;
  TreeNode *node = this->findNodeWithCharAtIndex(this->tree->root, index, localIndex);
  // cout << node->toString();
  
  string *fullString = node->value;
  int fullLength = fullString->length();
  int leftLength = localIndex + 1;
  int rightLength = fullLength - leftLength;
  // cout << leftLength << " vs " << fullLength << endl;
  assert(leftLength <= fullLength);

  // If char before split is not the last char in a node, we must split that node
  if (leftLength != fullLength){
    TreeNode *newNode = new TreeNode(node->parent, leftLength, NULL);
    newNode->left = new TreeNode(newNode, leftLength, new string(fullString->substr(0, leftLength)));
    newNode->right = new TreeNode(newNode, rightLength, new string(fullString->substr(leftLength)));

    if (newNode->parent != NULL) newNode->parent->replaceChild(node, newNode);
    // delete node;
    node = newNode->left;
    // cout << newNode->toString() << endl;
  }

  // cout << node->toString() << endl;

  int weightSliced = 0;
  stack<Rope *> slicedRoots;

  // Do the split (see https://en.wikipedia.org/wiki/Rope_(computer_science)#Split)
  TreeNode *cur = node;
  while (cur->parent != NULL){
    TreeNode *up = cur->parent;
    if (cur == up->right) break;

    up->key -= weightSliced;
    TreeNode *toSlice = up->right;
    if (toSlice != NULL){
      up->right = NULL;
      toSlice->parent = NULL;
      slicedRoots.push(new Rope(new SplayTree(toSlice)));
      weightSliced += sumWeightsDownRightSpine(toSlice);
    }

    cur = up;
  }

  // Find the new root of the old tree
  while (cur->parent != NULL) cur = cur->parent;
  this->tree->root = cur;

  // If we're splitting at the end of the string, append an empty rope
  if (slicedRoots.empty()) slicedRoots.push(new Rope(new SplayTree(NULL)));

  // Merge sliced pieces into single rope
  while (slicedRoots.size() > 1){
    Rope *right = slicedRoots.top();
    slicedRoots.pop();
    Rope *left = slicedRoots.top();
    slicedRoots.pop();
    slicedRoots.push(left->concat(right));
  }

  return make_pair(this, slicedRoots.top());
=======
  if (tree->root == NULL) return make_pair(new Rope(new SplayTree(NULL)), new Rope(new SplayTree(NULL)));
  if (index < 0) return make_pair(new Rope(new SplayTree(NULL)), this);
  
  // cout << "seeking: " << index << endl;
  // cout << "in: " << tree->toString() << endl;
  int localIndex;
  TreeNode *node = this->findNodeWithCharAtIndex(this->tree->root, index, localIndex);
  // cout << this->report(0, this->length()) << endl;
  
  // cout << "found: " << node->toString() << endl;
  
  string *fullString = node->value;
  int fullLength = fullString->length();
  int leftLength = localIndex + 1;
  int rightLength = fullLength - leftLength;
  // cout << leftLength << " vs " << fullLength << endl;
  assert(leftLength <= fullLength);

  // If char before split is not the last char in a node, we must split that node
  if (leftLength != fullLength){
    TreeNode *newNode = new TreeNode(node->parent, leftLength, NULL);
    newNode->left = new TreeNode(newNode, leftLength, new string(fullString->substr(0, leftLength)));
    newNode->right = new TreeNode(newNode, rightLength, new string(fullString->substr(leftLength)));

    if (newNode->parent != NULL) newNode->parent->replaceChild(node, newNode);
    // delete node;
    node = newNode->left;
    // cout << newNode->toString() << endl;
  }

  // cout << node->toString() << endl;

  int weightSliced = 0;
  stack<Rope *> slicedRoots;

  // Do the split (see https://en.wikipedia.org/wiki/Rope_(computer_science)#Split)
  TreeNode *cur = node;
  while (cur->parent != NULL){
    TreeNode *up = cur->parent;
    if (cur == up->right) break;

    up->key -= weightSliced;
    TreeNode *toSlice = up->right;
    if (toSlice != NULL){
      up->right = NULL;
      toSlice->parent = NULL;
      slicedRoots.push(new Rope(new SplayTree(toSlice)));
      weightSliced += sumWeightsDownRightSpine(toSlice);
    }

    cur = up;
  }

  // Find the new root of the old tree
  while (cur->parent != NULL) cur = cur->parent;
  this->tree->root = cur;

  // If we're splitting at the end of the string, append an empty rope
  if (slicedRoots.empty()) slicedRoots.push(new Rope(new SplayTree(NULL)));

  // Merge sliced pieces into single rope
  while (slicedRoots.size() > 1){
    Rope *right = slicedRoots.top();
    slicedRoots.pop();
    Rope *left = slicedRoots.top();
    slicedRoots.pop();
    slicedRoots.push(left->concat(right));
  }

  return make_pair(this, slicedRoots.top());
>>>>>>> 7496801452a53cb98acdbc16c45feb8711f436ea
}

Rope *Rope::insert(int index, string *str){
  Rope *newPiece = new Rope(new SplayTree(new TreeNode(NULL, str->length(), str)));
  if (tree->root == NULL) return newPiece;
  pair<Rope *, Rope *> pieces = this->split(index - 1);
  return pieces.first->concat(newPiece)->concat(pieces.second);
}

Rope *Rope::remove(int start, int end){
  pair<Rope *, Rope *> pieces = this->split(end - 1);
  Rope *after = pieces.second;
  pieces = pieces.first->split(start - 1);
  // cout << pieces.first->toString() << pieces.second->toString() << endl;
  // delete pieces.second;
  return pieces.first->concat(after);
}

std::string Rope::json() {
  return tree->toJSON();
}

string Rope::toString(){
  return tree->toString();
}

// Begin private helpers

TreeNode *Rope::findNodeWithCharAtIndex(TreeNode *node, int index, int & reportLocalIndex){
  assert(node != NULL);
  if (node->left == NULL && node->right == NULL){
    if (node->parent != NULL){
      // Rebalance tree as much as possible without ripping leaves
      node->parent->splay(this->tree);
    }
    reportLocalIndex = index;
    return node;
  }

  if (index < node->key){
    if (node->left != NULL){
      return this->findNodeWithCharAtIndex(node->left, index, reportLocalIndex);
    } else {
      assert(node->right != NULL);
      return this->findNodeWithCharAtIndex(node->right, index, reportLocalIndex);
    }
  } else {
    return this->findNodeWithCharAtIndex(node->right, index - node->key, reportLocalIndex);
  }
}

int Rope::sumWeightsDownRightSpine(TreeNode *node){
  int sum = 0;
  while (node != NULL){
    sum += node->key;
    node = node->right;
  }
  return sum;
}

string Rope::inOrder(TreeNode *node, TreeNode *start, TreeNode *end){
  stack<TreeNode *> toVisit;
  bool started = false;
  string result;
  while (!toVisit.empty() || node != NULL){
    if (node == NULL){
      node = toVisit.top();
      toVisit.pop();
      if (node == end) return result;
      if (started && node->value != NULL) result += *(node->value);
      if (node == start) started = true;
      node = node->right;
    }
    else {
      toVisit.push(node);
      node = node->left;
    }
  }
  assert(false);
  return "";
}

TreeNode *Rope::findNodeToReport(TreeNode *node, int start, int end){
  assert(node != NULL);
  if (start >= node->key && end >= node->key){
    return findNodeToReport(node->right, start - node->key, end - node->key);
  }
  else if (start < node->key && end < node->key){
    if (node->left != NULL){
      return findNodeToReport(node->left, start, end);
    } else {
      return node;
    }
  }
  else {
    return node;
  }
}
