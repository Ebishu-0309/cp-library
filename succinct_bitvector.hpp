#pragma once
#include <vector>
#include <array>
#include <immintrin.h>
using namespace std;

class BitVector {
private:
	using u8 = uint8_t;
	using u16 = uint16_t;

	int n, l;

	vector<u8> bit;
	vector<u16> large;
	vector<array<u8, 32>> small;

	static inline const u8 table[256] = {
		0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
	};

public:
	explicit BitVector(int _n)
		: n(_n)
		, l((_n + 255) >> 8)
	{
		bit.resize(l << 5);
		large.resize(l + 1);
		small.resize(l);
	}

	explicit BitVector(const vector<bool>& vec)
		: n(vec.size())
		, l((n + 255) >> 8)
	{
		bit.resize(l << 5);
		large.resize(l + 1);
		small.resize(l);

		for (int i = 0; i < n; ++i) set(i, vec[i]);
		build();
	}

	void set(int p, bool b) {
		const int bp = p >> 8;
		const int a = p & 7;
		if (b) bit[bp] |= 1 << a;
		else bit[bp] &= ~(1 << a);
	}

	bool get(int p) const { return bit[p >> 8] >> (p & 7) & 1; }
	bool operator[](int p) const { return get(p); }

	void build() {
		large[0] = 0;
		for (int i = 0; i < l; ++i) {
			small[i][0] = 0;
			for (int j = 0; j < 31; ++j) {
				small[i][j + 1] = small[i][j] + table[bit[(i << 5) + j]];
			}
			large[i + 1] = large[i] + small[i][31] + table[bit[((i + 1) << 5) - 1]];
		}
	}

	// count '1' in [0, p)
	int rank1(int p) const {
		const int lp = p >> 8;
		const int sp = (p & 255) >> 3;
		const int rem = bit[p >> 3] & _bzhi_u32(0xffffffffu, p & 7);

		return large[lp] + small[lp][sp] + table[rem];
	}

	// count '0' in [0, p)
	int rank0(int p) const { return p - rank1(p); }

	// minimum idx s.t. rank1(idx) = p
	int select(int p) const {
		if (p == 0) return 0;
		if (rank1(n) < p) return -1;

		int ng = 0, ok = n;
		while (ok - ng > 1) {
			int m = (ok + ng) / 2;
			(rank1(m) >= p ? ok : ng) = m;
		}

		return ok;
	}
};