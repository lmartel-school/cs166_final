#include "splay_tree.h"
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

typedef SplayTree::TreeNode TreeNode;
using namespace std;

TreeNode::TreeNode(TreeNode *parent, int key, std::string *value){
  this->parent = parent;
  this->left = NULL;
  this->right = NULL;
  this->key = key;
  this->value = value;
}

TreeNode::~TreeNode(){
  if (value != NULL) delete value;
  if (left != NULL) delete left;
  if (right != NULL) delete right;
}

void TreeNode::splay(SplayTree *inTree){
  /* A root-node should not be splayed. */
  // assert(parent != NULL);
  if (parent == NULL) return;

  TreeNode *grandParent = this->getGrandParent();

  if (grandParent == NULL) {
    // zig

    // cout << "will zig:" << parent->toString() << endl;

    this->rotate(parent, inTree);

    // cout << "did rotate:" << this->toString() << endl;
  }
  else if ((grandParent->right == parent && parent->left == this)
          || (grandParent->left == parent && parent->right == this)) {
    // zig-zag

    // cout << "will zigzag:" << grandParent->toString() << endl;

    this->rotate(parent, inTree);
    this->rotate(grandParent, inTree);

    // cout << "did rotate:" << this->toString() << endl;
  }
  else {
    // zig-zig

    // cout << "will zigzig:" << grandParent->toString() << endl;

    parent->rotate(grandParent, inTree);
    this->rotate(parent, inTree);

    // cout << "did rotate:" << this->toString() << endl;
  }
}

void TreeNode::rotate(TreeNode *parentNode, SplayTree *inTree) {
  assert(parentNode != NULL);
  assert(this->parent == parentNode);
  assert(parentNode->left == this || parentNode->right == this);
  TreeNode *newParent = parentNode->parent;

  if (parentNode == inTree->root) {
    inTree->root = this;
  }

  if (parentNode->left == this) {
    /* We're doing a right-rotation */
    if (newParent != NULL) {
      newParent->replaceChild(parentNode, this);
    }
    else {
      setParent(NULL);
    }
    parentNode->setLeft(this->right);
    this->setRight(parentNode);

    parentNode->key -= this->key;
  }
  else {
    /* We're doing a left-rotation */
    if (newParent != NULL) {
      newParent->replaceChild(parentNode, this);
    }
    else{
      setParent(NULL);
    }
    parentNode->setRight(this->left);
    this->setLeft(parentNode);

    this->key += parentNode->key;
  }
}

void TreeNode::replaceChild(TreeNode *child, TreeNode *newChild){
  if (child == left) {
    setLeft(newChild);
  }
  else {
    setRight(newChild);
  }
}

TreeNode *TreeNode::getGrandParent(){
  if (parent == NULL) return NULL;

  return parent->parent;
}

void TreeNode::setLeft(TreeNode *left){
  this->left = left;
  if (left != NULL) {
    left->setParent(this);
  }
}
void TreeNode::setRight(TreeNode *right){
  if (right != NULL) {
    right->setParent(this);
  }
  this->right = right;
}

void TreeNode::setParent(TreeNode *parent){
  this->parent = parent;
}

bool TreeNode::isValidBinaryTree() {
  bool valid = true;

  if (left != NULL) {
    if (key < left->key) valid = false;
    if (this != left->parent) valid = false;

    valid = valid && left->isValidBinaryTree();
  }

  if (right != NULL) {
    if (key > right->key) valid = false;
    if (this != right->parent) valid = false;

    valid = valid && right->isValidBinaryTree();
  }

  return valid;
}

void indent(int indentationLevel, string & builder) {
  for (int i = 0; i < indentationLevel; i++) {
    builder.append("\t");
  }
}

void TreeNode::toString(int indentationLevel, string & builder){
  builder.append("lol, to_string is broken --");
  if (value != NULL) builder.append(*value);
  builder.append("\n");
  indentationLevel++;
  if (left != NULL) {
    indent(indentationLevel, builder);
    builder.append("left: ");
    left->toString(indentationLevel, builder);
  }

  if (right != NULL) {
    indent(indentationLevel, builder);
    builder.append("right: ");
    right->toString(indentationLevel, builder);
  }
}

string TreeNode::toString(){
  string builder = "<Splay Tree>:\nroot: ";
  toString(0, builder);

  return builder;
}

string escape(string str) {
  return str;
}

string TreeNode::toJSON() {
  string str = "{\"name\":";
  int weight = this->weight();
  if (value == NULL) {
    stringstream ss;
    ss << weight;
    str += ss.str();
  } else {
    str += "\"" + escape(*value) + "\"";
  }
  if (left != NULL || right != NULL) {
    str += ",\"children\":[";
    bool addComma = false;
    if (left != NULL) {
      str += left->toJSON();
      addComma = true;
    }
    if (right != NULL) {
      if (addComma) str += ",";
      str += right->toJSON();
    }
    str += "]";
  }

  str += "}";
  return str;
}

int TreeNode::weight() {
  int weight = 0;
  if (value != NULL) weight += value->length();
  if (left != NULL) weight += left->weight();
  if (right != NULL) weight += right->weight();
  return weight;
}

// Splay tree

SplayTree::SplayTree(){
  this->root = NULL;
}

SplayTree::SplayTree(TreeNode *root){
  this->root = root;
}

SplayTree::SplayTree(vector<int> & keys, vector<string *> & values){
  if (keys.size() == 0) {
    root = NULL;
    return;
  }

  root = new TreeNode(NULL, keys[0], values[0]);
  TreeNode *current = root;
  for (unsigned int i = 1; i < keys.size(); i++) {
    TreeNode *newNode = new TreeNode(current, keys[0], values[i]);
    current->setRight(newNode);
    current = newNode;
  }
}

SplayTree::~SplayTree(){
  if(root != NULL) delete root;
}

string *SplayTree::get(int key){
  TreeNode *node;
  TreeNode *lastNode = NULL;

  for (node = root; node != NULL && node->key != key;) {
    if (key < node->key) {
      node = node->left;
    }
    else {
      node = node->right;
    }
    lastNode = node;
  }

  if (node == NULL) {
    if (lastNode != NULL) {
      splayToRoot(lastNode);
    }
    return NULL;
  }

  splayToRoot(node);
  return root->value;
}

void SplayTree::splayToRoot(TreeNode *node){
  while (root != node) {
    node->splay(this);
  }

  assert(this->isValidBinaryTree());
}

bool SplayTree::isValidBinaryTree(){
  if (root == NULL) return true;
  assert(root->parent == NULL);
  return root->isValidBinaryTree();
}

string SplayTree::toString(){
  return (root != NULL) ? root->toString() : "[root null]";
}

string SplayTree::toJSON(){
  return (root != NULL) ? root->toJSON() : "{}";
}
