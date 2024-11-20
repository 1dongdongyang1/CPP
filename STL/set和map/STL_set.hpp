#pragma once

#include "RBTree.hpp"
namespace ddy
{
	template<class K>
	class set
	{
		struct SetKofT
		{
			const K& operator()(const K& k)
			{
				return k;
			}
		};

	public:
		typedef typename RBTree<K, K, SetKofT>::iterator iterator;

		iterator begin()
		{
			return _t.begin();
		}

		iterator end()
		{
			return _t.end();
		}

		pair<iterator,bool> Insert(const K& k)
		{
			return _t.Insert(k);
		}
	private:
		RBTree<K, K,SetKofT> _t;
	};

}