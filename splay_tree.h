#include <string>
#include <vector>

#ifndef SPLAYTREE_H
#define SPLAYTREE_H

class SplayTree {
public:

	class TreeNode {
	public:

		TreeNode *left;
		TreeNode *right;
		TreeNode *parent;

		int key;
		std::string *value;

		TreeNode(TreeNode *parent, int key, std::string *value);
		~TreeNode();

		void splay(SplayTree *inTree);
		void replaceChild(TreeNode *child, TreeNode *newChild);
		bool isValidBinaryTree();
		
	private:
		friend class SplayTree;

		void rotate(TreeNode *parentNode, SplayTree *inTree);
		TreeNode *getGrandParent();
		void setLeft(TreeNode *left);
		void setRight(TreeNode *right);
		void setParent(TreeNode *parent);
	};

	TreeNode *root;

	SplayTree();
	SplayTree(TreeNode *root);
	SplayTree(std::vector<int> & keys, std::vector<std::string *> & values);
	~SplayTree();

	// void insert(int key, std::string value);
	std::string *get(int key);
	// void remove(int key);

	bool isValidBinaryTree();
	
private:
	void splayToRoot(TreeNode *node);

};

#endif