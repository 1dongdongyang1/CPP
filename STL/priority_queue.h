#pragma once

namespace ddy
{
	template<class T>
	struct less
	{
		bool operator()(const T& x1, const T& x2)
		{
			return x1 < x2;
		}
	};

	template<class T>
	struct greater
	{
		bool operator()(const T& x1, const T& x2)
		{
			return x1 > x2;
		}
	};

	template<class T, class Container = vector<int>, class Compare = less<T>>
	class priority_queue
	{
	public:
		Compare com;
		void AdjustUp(int child)
		{
			int parent = (child - 1) / 2;
			while (child)
			{
				//if (_con[child] > _con[parent])
				if (com(_con[parent], _con[child]))
				{
					swap(_con[child], _con[parent]);
					child = parent;
					parent = (child - 1) / 2;
				}
				else break;
			}
		}

		void AdjustDown(int root)
		{
			int parent = root;
			int child = parent * 2 + 1;

			while (child < size())
			{
				//if (child + 1 < size() && _con[child + 1] > _con[child])
				if (child + 1 < size() && com(_con[child],_con[child+1]))
					child++;

				// if (_con[child] > _con[parent])
				if (com(_con[parent], _con[child]))
				{
					swap(_con[child], _con[parent]);
					parent = child;
					child = parent * 2 + 1;
				}
				else break;
			}
		}

		void push(const T& x)
		{
			_con.push_back(x);
			AdjustUp(size() - 1);
		}

		void pop()
		{
			swap(_con[0], _con[size() - 1]);
			_con.pop_back();
			AdjustDown(0);
		}

		T& top()
		{
			return _con[0];
		}

		size_t size()
		{
			return _con.size();
		}

		bool empty()
		{
			return _con.empty();
		}
	private:
		Container _con;
	};

	void test_priority_queue1()
	{
		//priority_queue<int> pq;
		priority_queue<int,vector<int>,greater<int>> pq;
		pq.push(9);
		pq.push(8);
		pq.push(6);
		pq.push(7);
		pq.push(1);
		pq.push(3);
		pq.push(5);
		pq.push(5);

		while (!pq.empty())
		{
			cout << pq.top() << " ";
			pq.pop();
		}
		cout << endl;
	}
}