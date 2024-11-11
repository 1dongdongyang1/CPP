#pragma once
#include<iostream>
using namespace std;

template<class K>
struct BSTreeNode
{
public:
	BSTreeNode<K>* _left;
	BSTreeNode<K>* _right;
	K _key;

	BSTreeNode(const K& key)
		:_left(nullptr)
		,_right(nullptr)
		,_key(key)
	{}
};

template<class K>
class BSTree		//Binary Search Tree
{
	typedef BSTreeNode<K> Node;
public:
	bool insert(const K& key)
	{
		if (_root == nullptr)
		{
			_root = new Node(key);
			return true;
		}

		Node* cur = _root;
		Node* prev = nullptr;
		while (cur)
		{
			prev = cur;
			if (key > cur->_key)	cur = cur->_right;
			else if (key < cur->_key)	cur = cur->_left;
			else return false;
		}
		cur = new Node(key);
		if (key > prev->_key)	prev->_right = cur;
		else prev->_left = cur;
		return true;
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr)	return;
		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	bool Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (key > cur->_key)	cur = cur->_right;
			else if (key < cur->_key) cur = cur->_left;
			else return true;
		}
		return false;
	}

	bool Erase(const K& key)
	{
		Node* cur = _root;
		Node* prev = nullptr;
		while (cur)
		{
			prev = cur;
			if (key > cur->_key)	cur = cur->_right;
			else if (key < cur->_key) cur = cur->_left;

			if (key == cur->_key)
			{
				if (cur->_left == nullptr)
				{
					if (cur == _root)	_root = cur->_right;
					else
					{
						if (prev->_left == cur)	prev->_left = cur->_right;
						else prev->_right = cur->_right;
					}
					delete cur;
				}
				else if (cur->_right == nullptr)
				{
					if (cur == _root)	_root = cur->_left;
					else
					{
						if (prev->_left == cur)	prev->_left = cur->_left;
						else prev->_right = cur->_left;
					}
					delete cur;
				}
				else
				{
					Node* rightMin = cur->_right;
					Node* rightMinPrev = cur;
					while (rightMin->_left)
					{
						rightMinPrev = rightMin;
						rightMin = rightMin->_left;
					}
					cur->_key = rightMin->_key;
					if (rightMinPrev == cur)	rightMinPrev->_right = rightMin->_right;
					else rightMinPrev->_left = rightMin->_right;
					delete rightMin;

				}
				return true;
			}
		}
		return false;
	}
private:
	Node* _root = nullptr;
};