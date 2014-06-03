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
		
		void splay(SplayTree *inTree);

		bool isValidBinaryTree();
		
	private:
		friend class SplayTree;

		TreeNode(TreeNode *parent, int key, std::string *value);
		void rotate(TreeNode *parentNode, SplayTree *inTree);
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

	bool isValidBinaryTree();
	
private:
	void splayToRoot(TreeNode *node);

};