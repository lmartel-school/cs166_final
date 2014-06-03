#include "splay_tree.h"
#include <utility>

#ifndef ROPE_H
#define ROPE_H

class Rope {
public:

	Rope();
	Rope(SplayTree *tree);
	~Rope();

	int length();
	char at(int index);
	std::string report(int start, int end); // incl, excl

	std::pair<Rope *, Rope *> split(int index); // split AFTER index
	Rope *concat(Rope *right);
	Rope *insert(int index, std::string *s);
	Rope *remove(int start, int end); // incl, excl
	
private:
	SplayTree *tree;

	SplayTree::TreeNode *findNodeWithCharAtIndex(SplayTree::TreeNode *node, int index, int *reportLocalIndex);
	std::string inOrder(SplayTree::TreeNode *node, SplayTree::TreeNode *start, SplayTree::TreeNode *end);
	SplayTree::TreeNode *findNodeToReport(SplayTree::TreeNode *node, int start, int end);
	static int sumWeightsDownRightSpine(SplayTree::TreeNode *node);
};

#endif