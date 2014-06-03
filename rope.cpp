#include "rope.h"
#include "splay_tree.h"
#include <assert.h>
#include <stack>

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
	return sumWeightsDownRightSpine(tree->root);
}

char Rope::at(int index){
	int *localIndex = NULL;
	TreeNode *node = this->findNodeWithCharAtIndex(this->tree->root, index, localIndex);
	return node->value->at(*localIndex);
}

string Rope::report(int start, int end){
	int *localStart = NULL;
	int *localEnd = NULL;
	TreeNode *startNode = this->findNodeWithCharAtIndex(this->tree->root, start, localStart);
	TreeNode *endNode = this->findNodeWithCharAtIndex(this->tree->root, end, localEnd);

	TreeNode *lca = this->findNodeToReport(this->tree->root, start, end);
	
	string res = startNode->value->substr(*localStart);
	res += inOrder(lca, startNode, endNode);
	res += endNode->value->substr(0, 1 + *localEnd);

	lca->splay(tree);

	return res;
}

Rope *Rope::concat(Rope *right){
	Rope *left = this;
	TreeNode *newRoot = new TreeNode(NULL, left->length(), new string(""));
	newRoot->left = left->tree->root;
	newRoot->right = right->tree->root;

	return new Rope(new SplayTree(newRoot));
}

pair<Rope *, Rope *> Rope::split(int index){
	int *localIndex = NULL;
	TreeNode *node = this->findNodeWithCharAtIndex(this->tree->root, index, localIndex);
	string *fullString = node->value;
	int fullLength = fullString->length();
	int leftLength = *localIndex + 1;
	int rightLength = fullLength - leftLength;
	assert(leftLength <= fullLength);

	// If char before split is not the last char in a node, we must split that node
	if (leftLength != fullLength){
		TreeNode *newNode = new TreeNode(node->parent, leftLength, NULL);
		newNode->left = new TreeNode(newNode, leftLength, new string(fullString->substr(0, leftLength)));
		newNode->right = new TreeNode(newNode, rightLength, new string(fullString->substr(leftLength)));

		newNode->parent->replaceChild(node, newNode);
		delete node;
		node = newNode->left;
	}

	int weightSliced = 0;
	stack<Rope *> slicedRoots;

	// Do the split (see https://en.wikipedia.org/wiki/Rope_(computer_science)#Split)
	TreeNode *cur = node;
	while (cur->parent != NULL){
		TreeNode *parent = cur->parent;
		if (cur == parent->right) break;

		parent->key -= weightSliced;
		TreeNode *toSlice = parent->right;
		if (toSlice != NULL){
			parent->right = NULL;
			toSlice->parent = NULL;
			slicedRoots.push(new Rope(new SplayTree(toSlice)));
			weightSliced += sumWeightsDownRightSpine(toSlice);
		}

		cur = parent;
	}

	// Merge sliced pieces into single rope
	while (slicedRoots.size() > 1){
		Rope *right = slicedRoots.top();
		slicedRoots.pop();
		Rope *left = slicedRoots.top();
		slicedRoots.pop();
		slicedRoots.push(left->concat(right));
	}

	return make_pair(this, slicedRoots.top());
}

Rope *Rope::insert(int index, string *str){
	pair<Rope *, Rope *> pieces = this->split(index);
	Rope *newPiece = new Rope(new SplayTree(new TreeNode(NULL, str->length(), str)));
	return pieces.first->concat(newPiece)->concat(pieces.second);
}

Rope *Rope::remove(int start, int end){
	pair<Rope *, Rope *> pieces = this->split(end);
	Rope *after = pieces.second;
	pieces = pieces.first->split(start);
	delete pieces.second;
	return pieces.first->concat(after);
}

// Begin private helpers

TreeNode *Rope::findNodeWithCharAtIndex(TreeNode *node, int index, int *reportLocalIndex){
	assert(node != NULL);
	if (node->key < index){
		return this->findNodeWithCharAtIndex(node->right, index - node->key, reportLocalIndex);
	} else if (node->left != NULL){
		return this->findNodeWithCharAtIndex(node->left, index, reportLocalIndex);
	} else {
		if (node->parent != NULL){

			// Rebalance tree as much as possible without ripping leaves
			node->parent->splay(this->tree);
		}

		*reportLocalIndex = index;
		return node;
	}
}

int Rope::sumWeightsDownRightSpine(TreeNode *node){
	if (node == NULL) return 0;
	return node->key + sumWeightsDownRightSpine(node->right);
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
		return findNodeToReport(node->left, start, end);
	}
	else {
		return node;
	}
}