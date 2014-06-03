#include "splay_tree.h"
#include <assert.h>
#include <vector>
#include <string>

typedef SplayTree::TreeNode TreeNode;
using namespace std;

TreeNode::TreeNode(TreeNode *parent, int key, std::string *value){
	this->parent = parent;
	this->left = NULL;
	this->right = NULL;
	this->key = key;
	this->value = value;
}

void TreeNode::splay(SplayTree *inTree){
	/* A root-node should not be splayed. */
	assert(parent != NULL);

	TreeNode *grandParent = this->getGrandParent();

	if (grandParent == NULL) {
		// zig
		this->rotate(parent, inTree);
	}
	else if ((grandParent->right == parent && parent->left == this)
		|| (grandParent->left == parent && parent->right == this)) {
		// zig-zag
		this->rotate(parent, inTree);
		this->rotate(grandParent, inTree);
	}
	else {
		// zig-zig
		parent->rotate(grandParent, inTree);
		this->rotate(parent, inTree);
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
	for (int i = 1; i < keys.size(); i++) {
		TreeNode *newNode = new TreeNode(current, keys[0], values[i]);
		current->setRight(newNode);
		current = newNode;
	}
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