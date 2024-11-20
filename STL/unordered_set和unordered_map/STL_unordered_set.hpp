#pragma once

#include "HashTable.hpp"

namespace ddy
{
	template<class K, class Hash = _Hash<K>>
	class unordered_set
	{
		struct SetKeyOfT
		{
			const K& operator()(const K& key)
			{
				return key;
			}
		};
	public:
		typedef typename HashTable<K, K, SetKeyOfT, Hash>::iterator iterator;

		iterator begin()
		{
			return _ht.begin();
		}

		iterator end()
		{
			return _ht.end();
		}

		pair<iterator, bool> Insert(const K& k)
		{
			return _ht.Insert(k);
		}

	private:
		HashTable<K, K , SetKeyOfT, Hash> _ht;
	};

	void test_unordered_set()
	{
		unordered_set<int> ht;
	}
}