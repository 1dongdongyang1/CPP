#pragma once

#include "HashTable.hpp"

namespace ddy
{
	template<class K, class V, class Hash = _Hash<K>>
	class unordered_map
	{
		struct MapKeyOfT
		{
			const K& operator()(const pair<K, V>& kv)
			{
				return kv.first;
			}
		};
	public:
		typedef typename HashTable<K, pair<K, V>, MapKeyOfT, Hash>::iterator iterator;

		iterator begin()
		{
			return _ht.begin();
		}

		iterator end()
		{
			return _ht.end();
		}

		pair<iterator,bool> Insert(const pair<K, V>& kv)
		{
			return _ht.Insert(kv);
		}

		V& operator[](const K& key)
		{
			pair<iterator, bool> ret = _ht.Insert(make_pair(key,V()));
			return ret.first->second;
		}
	private:
		HashTable<K, pair<K, V>, MapKeyOfT, Hash> _ht;
	};

	void test_unordered_map()
	{
		unordered_map<int, int> ht;
		ht.Insert(make_pair(1, 1));
		ht.Insert(make_pair(2, 2));
		ht.Insert(make_pair(3, 3));
		ht.Insert(make_pair(4, 4));

		ht[4] = 3;
		ht[5];

		unordered_map<int, int>::iterator it = ht.begin();
		while (it != ht.end())
		{
			cout << it->first << ":" << it->second << endl;
			++it;
		}
		cout << endl;
	}
}