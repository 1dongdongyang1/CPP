#pragma once

#include <vector>
#include <string>

template<class K>
struct _Hash
{
	const K& operator()(const K& key)
	{
		return key;
	}
};

template<>
struct _Hash<string>
{
	size_t operator()(const string& key)
	{
		//BKRD Hash
		size_t hash = 0;
		for (size_t i = 0; i < key.size(); i++)
		{
			hash *= 131;
			hash += key[i];
		}
		return hash;
	}
};

template<class T>
struct HashData
{
	T _data;
	HashData<T>* _next;

	HashData(const T& data)
		:_data(data)
		,_next(nullptr)
	{}
};

template<class K, class T, class KeyOfT, class Hash>
class HashTable;

template<class K, class T, class KeyOfT, class Hash>
struct __HashTableIterator
{
	typedef __HashTableIterator<K, T, KeyOfT, Hash> Self;
	typedef HashTable<K, T, KeyOfT, Hash> HT;
	typedef HashData<T> Node;
	Node* _node;
	HT* _pht;

	__HashTableIterator(Node* node, HT* pht)
		:_node(node)
		,_pht(pht)
	{}

	T* operator->()
	{
		return &_node->_data;
	}

	T& operator*()
	{
		return _node->_data;
	}

	bool operator!=(const Self& s)
	{
		return _node != s._node;
	}

	bool operator==(const Self& s)
	{
		return _node == s._node;
	}

	Self operator++()
	{
		if (_node->_next) _node = _node->_next;
		else
		{
			KeyOfT koft;
			size_t i = _pht->HashFunc(koft(_node->_data)) % _pht->_tables.size();
			i++;
			for (; i < _pht->_tables.size(); i++)
			{
				Node* cur = _pht->_tables[i];
				if (cur)
				{
					_node = cur;
					return *this;
				}
			}
			_node = nullptr;
		}
		return *this;
	}
};

template<class K, class T, class KeyOfT , class Hash>
class HashTable
{
	typedef HashData<T> Node;
public:
	friend struct __HashTableIterator<K, T, KeyOfT, Hash>;
	typedef __HashTableIterator<K, T, KeyOfT, Hash> iterator;

	iterator begin()
	{
		for (size_t i = 0; i < _tables.size(); i++)
		{
			if (_tables[i]) return iterator(_tables[i],this);
		}
		return end();
	}

	iterator end()
	{
		return iterator(nullptr,this);
	}

	~HashTable()
	{
		Clear();
	}

	void Clear()
	{
		for (size_t i = 0; i < _tables.size(); i++)
		{
			Node* cur = _tables[i];
			while (cur)
			{
				Node* next = cur->_next;
				delete cur;
				cur = next;
			}
			_tables[i] = nullptr;
		}
	}

	size_t HashFunc(const K& key)
	{
		Hash hash;
		return hash(key);
	}

	pair<iterator,bool> Insert(const T& data)
	{
		KeyOfT koft;
		Hash hash;

		//capacity expansion
		if (_tables.size() == _num)
		{
			//remove the nodes from the old table, 
			//recalculate their position in the new table,
			//and insert them 
			vector<Node*> newtables;
			size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
			newtables.resize(newsize);
			for (size_t i = 0; i < _tables.size(); i++)
			{
				Node* cur = _tables[i];
				while (cur)
				{
					Node* next = cur->_next;
					size_t index = hash(koft(cur->_data)) % newsize;
					cur->_next = newtables[index];
					newtables[index] = cur;

					cur = next;
				}
				_tables[i] = nullptr;
			}
			_tables.swap(newtables);
		}

		//calculate the position of data in the table
		size_t index = hash(koft(data)) % _tables.size();

		//1.find out if this value is in the table
		Node* cur = _tables[index];
		while (cur)
		{
			if (koft(cur->_data) == koft(data)) return make_pair(iterator(cur,this),false);
			else cur = cur->_next;
		}

		//2.insert the head into the linked list
		Node* newnode = new Node(data);
		newnode->_next = _tables[index];
		_tables[index] = newnode;

		_num++;
		return make_pair(iterator(newnode,this),true);
	}

	Node* Find(const K& key)
	{
		KeyOfT koft;
		size_t index = hash(key) % _tables.size();
		Node* cur = _tables[index];
		while (cur)
		{
			if (koft(cur->_data) == key) return cur;
			else cur = cur->_next;
		}
		return nullptr;
	}

	bool Erase(const K& key)
	{
		KeyOfT koft;
		size_t index = hash(key) % _tables.size();
		Node* cur = _tables[index];
		Node* pre = nullptr;
		while (cur)
		{
			if (koft(cur->_data) == key)
			{
				if (pre == nullptr) _tables[index] = cur->_next;
				else pre->_next = cur->_next;
				delete cur;
				_num--;
				return true;
			}
			else
			{
				pre = cur;
				cur = cur->_next;
			}
		}
		return false;
	}
private:
	vector<Node*> _tables;
	size_t _num = 0;
};

