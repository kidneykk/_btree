#ifndef BTREE_H_
#define BTREE_H_

#include <iosfwd>

template<class T>
struct node {
	node* left;
	node* right;
	node* parent;
	T data;
};

template<class T>
class btree {
private:
	node<T>* root;

	node<T>* create_node(const T& val, node<T>* parent) {
		node<T>* newNode = new node<T>;
		newNode->data = val;
		newNode->left = newNode->right = nullptr;
		newNode->parent = parent;
		return newNode;
	}

	void destroy_tree(node<T>* curNode) {
		if (curNode) {
			destroy_tree(curNode->left);
			destroy_tree(curNode->right);
			delete curNode;
		}
	}

private:
	node<T>* search(node<T>* curNode, const T& val) {
		if (!curNode || val == curNode->data) {
			return curNode;
		}
		if (val < curNode->data) search(curNode->left, val);
		else search(curNode->right, val);
	}

	node<T>* min_node(node<T>* curNode) {
		while (curNode->left) {
			curNode = curNode->left;
		}
		return curNode;
	}

	node<T>* max_node(node<T>* curNode) {
		while (curNode->right) {
			curNode = curNode->right;
		}
		return curNode;
	}

	node<T>* successor(node<T>* curNode) {
		if (curNode->right) {
			return min_node(curNode->right);
		}
		node<T>* succNode = curNode->parent;
		while (succNode && curNode == succNode->right) {
			curNode = succNode;
			succNode = succNode->parent;
		}
		return succNode;
	}

	node<T>* predecessor(node<T>* curNode) {
		if (curNode->left) {
			return max_node(curNode->left);
		}
		node<T>* predNode = curNode->parent;
		while (predNode && curNode == predNode->left) {
			curNode = predNode;
			predNode = predNode->parent;
		}
		return predNode;
	}

	void insert(const T& val, node<T>* curNode) {
		if (curNode->data > val) {
			if (curNode->left) {	// if left child exists
				insert(val, curNode->left);	// go deeper...
			}
			else {
				curNode->left = create_node(val, curNode->left);
			}
		}
		else {	// curNode->data <= val
			if (curNode->right) {	// if right child exists
				insert(val, curNode->right); // go deeper...
			}
			else {
				curNode->right = create_node(val, curNode->right);
			}
		}
	}

	// ------ NODE DELETION ------

	// auxiliary function: "plants" v in u
	void transplant(node<T>* u, node<T>* v) {
		if (u->parent == nullptr) {
			root = v;
		}
		else if (u == u->parent->left) {
			u->parent->left = v;
		}
		else {
			u->parent->right = v;
		}
		if (v != nullptr) {
			v->parent = u->parent;
		}
	}

	void delete_node(node<T>* delNode) {
		if (!delNode->left) {
			transplant(delNode, delNode->right);
		}
		else if (!delNode->right) {
			transplant(delNode, delNode->left);
		}
		else {
			node<T>* y = min_node(delNode->right);
			if (y->parent != delNode) {	// if 'delNode' is not parent of 'y'
				transplant(y, y->right);
				y->right = delNode->right;
				y->right->parent = y;
			}
			transplant(delNode, y);
			y->left = delNode->left;
			y->left->parent = y;
		}
	}

	// ------ TREE OUTPUT ------
	void preorder_walk(node<T>* curNode) {
		std::cout << curNode->data << ' ';
		if (curNode->left) preorder_walk(curNode->left);
		if (curNode->right) preorder_walk(curNode->right);
	}

	void inorder_walk(node<T>* curNode) {
		if (curNode->left) inorder_walk(curNode->left);
		std::cout << curNode->data << ' ';
		if (curNode->right) inorder_walk(curNode->right);
	}

	void postorder_walk(node<T>* curNode) {
		if (curNode->left) postorder_walk(curNode->left);
		if (curNode->right) postorder_walk(curNode->right);
		std::cout << curNode->data << ' ';
	}

public:
	btree() : root(nullptr) { }

	btree(const std::initializer_list<T>& i_list)
		: root(nullptr)
	{
		for (const auto& i_elem : i_list) {
			insert(i_elem);
		}
	}

	~btree() {
		destroy_tree(root);
	}

	void insert(const T& val) {
		if (!root) {
			root = create_node(val, nullptr);
		}
		else {
			insert(val, root);
		}
	}

	const T& minimum() const {
		return min_node(root);
	}

	const T& maximum() const {
		return max_node(root);
	}

	void successor(const T& val) {
		node<T>* succ;
		if (succ = successor(search(root, val)))
			std::cout << "successor (" << val << "): " << succ->data << '\n';
		else
			std::cout << "no successors for (" << val << ")\n";
	}

	void predecessor(const T& val) {
		node<T>* pred;
		if (pred = predecessor(search(root, val)))
			std::cout << "predecessor (" << val << "): " << pred->data << '\n';
		else
			std::cout << "no predecessors for (" << val << ")\n";
	}

	void preorder_walk() {
		preorder_walk(root);
	}

	void inorder_walk() {
		inorder_walk(root);
	}

	void postorder_walk() {
		postorder_walk(root);
	}
};

#endif