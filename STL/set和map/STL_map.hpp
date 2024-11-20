#pragma once

#include "RBTree.hpp"
namespace ddy
{
	template<class K, class V>
	class map
	{
		struct MapKofT
		{
			const K& operator()(const pair<K,V>& kv)
			{
				return kv.first;
			}
		};

	public:
		typedef typename RBTree<K, pair<K, V>, MapKofT>::iterator iterator;

		iterator begin()
		{
			return _t.begin();
		}

		iterator end()
		{
			return _t.end();
		}

		pair<iterator, bool> Insert(const pair<K, V>& kv)
		{
			return _t.Insert(kv);
		}

		V& operator[](const K& key)
		{
			pair<iterator, bool> ret = _t.Insert(make_pair(key, V()));
			return ret.first->second;
		}
	private:
		RBTree<K, pair<K,V>, MapKofT> _t;
	};

}