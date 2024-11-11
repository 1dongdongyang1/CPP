#pragma once
#include <iostream>
#include <assert.h>
using namespace std;

namespace ddy
{
	template<class T>
	struct __list_node
	{
		__list_node<T>* _prev;
		__list_node<T>* _next;
		T _data;

		__list_node(const T& x = T())
			:_data(x)
			,_prev(nullptr)
			,_next(nullptr)
		{}
	};

	template<class T, class Ref, class Ptr>
	struct __list_iterator
	{
		typedef __list_node<T> Node;
		typedef __list_iterator<T, Ref, Ptr> Self;
		Node* _node;

		__list_iterator(Node* node)
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

		Self& operator++()
		{
			_node = _node->_next;
			return *this;
		}

		Self operator++(int)
		{
			Self tmp(*this);
			//_node = _node->_next;
			++(*this);
			return tmp;
		}

		Self& operator--()
		{
			_node = _node->_prev;
			return *this;
		}

		Self operator--(int)
		{
			Self tmp(*this);
			//_node = _node->_next;
			--(*this);
			return tmp;
		}

		bool operator!=(const Self& it)const
		{
			return _node != it._node;
		}

		bool operator==(const Self& it)const
		{
			return _node == it._node;
		}
	};

	template<class T>
	class list
	{
		typedef __list_node<T> Node;
	public:
		typedef __list_iterator<T, T&, T*> iterator;
		typedef __list_iterator<T, const T&, const T*> const_iterator;

		iterator begin()
		{
			return iterator(_head->_next);
		}

		iterator end()
		{
			return iterator(_head);
		}

		const_iterator begin()const
		{
			return const_iterator(_head->_next);
		}

		const_iterator end()const
		{
			return const_iterator(_head);
		}

		list()
		{
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
		}


		list(const list<T>& lt1)
		{
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;

			const_iterator it = lt1.begin();
			while (it != lt1.end())
			{
				push_back(*it++);
			}
		}



		/*list<T>& operator=(const list<T>& lt)
		{
			if (this != &lt)
			{
				clear();
				for (auto e : lt)	push_back(e);
			}
			return *this;
		}*/

		list<T>& operator=(list<T>& lt)
		{
			swap(_head, lt._head);
			return *this;
		}

		~list()
		{
			clear();
			delete _head;
			_head = nullptr;
		}

		void clear()
		{
			iterator it = begin();
			while (it != end())
			{
				erase(it++);
			}
		}

		void push_back(const T& x)
		{
			/*Node* _tail = _head->_prev;
			Node* node = new Node(x);
			_tail->_next = node;
			node->_next = _head;
			node->_prev = _tail;
			_head->_prev = node;*/
			insert(end(), x);
		}

		void pop_back()
		{
			//erase(iterator(_head->_prev));
			erase(--end());
		}
		void push_front(const T& x)
		{
			insert(begin(), x);
		}
		void pop_front()
		{
			erase(begin());	
		}

		void insert(iterator pos, const T& x)
		{
			Node* newnode = new Node(x);
			Node* cur = pos._node;
			Node* prev = cur->_prev;
			prev->_next = newnode;
			newnode->_next = cur;
			newnode->_prev = prev;
			cur->_prev = newnode;
		}
		void erase(iterator pos)
		{
			assert(pos != end());

			Node* cur = pos._node;
			Node* prev = cur->_prev;
			Node* next = cur->_next;

			prev->_next = next;
			next->_prev = prev;

			delete cur;
		}
	private:
		Node* _head;
	};

	void print(const list<int>& lt)
	{
		list<int>::const_iterator it = lt.begin();
		while (it != lt.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;
	}

	void test1()
	{
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);

		lt.erase(lt.begin());
		
		print(lt);
	}
		
	void test2()
	{
		list<int> lt1;
		lt1.push_back(1);
		lt1.push_back(2);
		lt1.push_back(3);
		lt1.push_back(4);

		print(lt1);
		list<int> lt2;
		lt2 = lt1;

		print(lt2);

	}
}
