#include <string>
#include <vector>

class SplayTree {
public:

	class TreeNode {
	public:

		TreeNode *left;
		TreeNode *right;
		TreeNode *parent;

		int key;
		std::string *value;
		
		void splay();

		bool isValidBinaryTree();
		
	private:
		friend class SplayTree;
		SplayTree *owner;

		TreeNode(SplayTree *owner, TreeNode *parent, int key, std::string *value);
		void rotate(TreeNode *parentNode);
		void replaceChild(TreeNode *child, TreeNode *newChild);
		TreeNode *getGrandParent();
		void setLeft(TreeNode *left);
		void setRight(TreeNode *right);
		void setParent(TreeNode *parent);
	};

	TreeNode *root;

	SplayTree();
	SplayTree(TreeNode *root);
	SplayTree(std::vector<int> & keys, std::vector<std::string *> & values);

	// void insert(int key, std::string value);
	std::string *get(int key);
	// void remove(int key);

	TreeNode *makeNode(TreeNode *parent, int key, std::string *value);

	bool isValidBinaryTree();
	
private:
	void splayToRoot(TreeNode *node);

};