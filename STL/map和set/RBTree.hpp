#pragma once

enum Color
{
	RED,
	BLACK
};

template<class T>
struct RBTreeNode
{
	RBTreeNode<T>* _left;
	RBTreeNode<T>* _right;
	RBTreeNode<T>* _parent;

	T _data;

	Color _col;

	RBTreeNode(const T& data)
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _data(data)
		, _col(RED)		//newly added node is red
	{}
};

template<class T, class Ref, class Ptr>
struct __TreeIterator
{
	typedef RBTreeNode<T> Node;
	typedef __TreeIterator<T,Ref,Ptr> Self;
	Node* _node;

	__TreeIterator(Node* node)
		:_node(node)
	{}

	Ref operator*()
	{
		return _node->_data;
	}

	Ptr operator->()
	{
		return &_node->_data;
	}

	bool operator!=(const Self& s)
	{
		return _node != s._node;
	}

	bool operator==(const Self& s)
	{
		return _node == s._node;
	}

	Self& operator++()
	{
		//1.if the right is not empty, next node is the leftmost node of the right subtree
		if (_node->_right)
		{
			Node* subLeft = _node->_right;
			while (subLeft->_left) subLeft = subLeft->_left;
			_node = subLeft;
		}
		else //2.if the right is empty, next node is the parent node whose first child in the ancestor is the parent's left node
		{
			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent && cur == parent->_right)
			{
				cur = cur->_parent;
				parent = parent->_parent;
			}
			_node = parent;
		}
		return *this;
	}

	Self& operator--()
	{
		//1.if the left is not empty, next node is the rightmost node of the left subtree
		if (_node->_left)
		{
			Node* subRight = _node->_right;
			while (subRight->_right) subRight = subRight->_right;
			_node = subRight;
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent && cur == parent->_left)
			{
				cur = cur->_parent;
				parent = parent->_parent;
			}
			_node = parent;
		}
		return *this;
	}
};

template<class K, class T, class KofT>
class RBTree
{
	typedef RBTreeNode<T> Node;
public:
	typedef __TreeIterator<T, T&, T*> iterator;
	typedef __TreeIterator<T, const T&, const T*> const_iterator;
	iterator begin()
	{
		Node* cur = _root;
		while (cur && cur->_left) cur = cur->_left;
		return iterator(cur);
	}

	iterator end()
	{
		return iterator(nullptr);
	}

	pair<iterator,bool> Insert(const T& data)
	{
		//1.insert according to the rules of binary search tree
		if (!_root)
		{
			_root = new Node(data);
			_root->_col = BLACK;
			return make_pair(iterator(_root),true);
		}

		KofT koft;
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			parent = cur;
			if (koft(data) < koft(cur->_data)) cur = cur->_left;
			else if (koft(data) > koft(cur->_data)) cur = cur->_right;
			else return make_pair(iterator(cur),false);
		}

		cur = new Node(data);
		Node* newnode = cur;
		if (koft(data) < koft(parent->_data)) parent->_left = cur;
		else parent->_right = cur;
		cur->_parent = parent;
		
		//2. update color
		while (parent && parent->_col == RED)
		{
			//the conditional judgment of RBTree mainly relies on uncle
			Node* grandfather = parent->_parent;
			if (grandfather->_left == parent)
			{
				Node* uncle = grandfather->_right;
				//Case 1, uncle exists and is red
				//Case 2, parent == grandfather->_left, uncle does not exist or is black
				//Case 3, parent == grandfather->_right, uncle does not exist or is black
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					//upgrade
					cur = grandfather;
					parent = cur->_parent;
				}
				else //uncle does not exist or is black
				{
					//Case 3
					if (cur == parent->_right)
					{
						RotateL(parent);
						swap(parent, cur);
					}

					//Case 2 or transformed from Case 3
					RotateR(grandfather);
					grandfather->_col = RED;
					parent->_col = BLACK;

					break;
				}

			}
			else //grandfather->_right == parent
			{
				Node* uncle = grandfather->_left;
				if (uncle&& uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					if (cur == parent->_left)
					{
						RotateR(parent);
						swap(parent, cur);
					}
					RotateL(grandfather);
					grandfather->_col = RED;
					parent->_col = BLACK;
				}
			}
		}

		_root->_col = BLACK;
		return make_pair(iterator(newnode), true);
	}

	void InOrder()
	{
		_InOrder(_root);
	}
private:
	Node* _root = nullptr;

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* ppNode = parent->_parent;

		parent->_right = subRL;
		if (subRL)	subRL->_parent = parent;
		subR->_left = parent;
		parent->_parent = subR;

		if (_root == parent)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			if (parent == ppNode->_left)	ppNode->_left = subR;
			else ppNode->_right = subR;
			subR->_parent = ppNode;
		}
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* ppNode = parent->_parent;

		parent->_left = subLR;
		if (subLR)	subLR->_parent = parent;
		subL->_right = parent;
		parent->_parent = subL;

		if (_root == parent)
		{
			_root = subL;
			subL->_parent = nullptr;
		}
		else
		{
			if (parent == ppNode->_left)	ppNode->_left = subL;
			else ppNode->_right = subL;
			subL->_parent = ppNode;
		}
	}

	void _InOrder(Node* root)
	{
		if (!root)	return;

		_InOrder(root->_left);
		//cout << root->_kv.first << ":" << root->_kv.second << endl;
		_InOrder(root->_right);
	}
};


