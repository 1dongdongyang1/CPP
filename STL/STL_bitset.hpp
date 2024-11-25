#pragma once
#include <vector>
using namespace std;

namespace ddy
{
	class bitset
	{
	public:
		bitset(size_t N)
		{
			_bits.resize(N / 32 + 1, 0);
		}

		void set(size_t x)
		{
			size_t index = x / 32;
			size_t pos = x % 32;

			_bits[index] |= (1 << pos);
			_num++;
		}

		void reset(size_t x)
		{
			size_t index = x / 32;
			size_t pos = x % 32;

			_bits[index] &= ~(1 << pos);
			_num--;
		}

		bool test(size_t x)
		{
			size_t index = x / 32;
			size_t pos = x % 32;

			return _bits[index] & (1 << pos);
		}
	private:
		vector<int> _bits;
		size_t _num;
	};
}