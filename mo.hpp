class Mo {
	int n;
	vector<int> ls, rs;

public:
	explicit Mo(int _n) : n(_n) {}
	Mo(int _n, int q) : n(_n)
	{
		ls.reserve(q); rs.reserve(q);
	}

	// [l, r) 0-indexed
	void add_query(int l, int r) {
		ls.push_back(l);
		rs.push_back(r);
	}

	template<typename AL, typename AR, typename DL, typename DR, typename O>
	void build(const AL& add_left, const AR& add_right, const DL& del_left, const DR& del_right, const O& out) {
		const int q = ls.size(), w = max<int>(1, sqrt(3) * n / sqrt(2 * q));
		vector<int> ord(q);
		iota(ord.begin(), ord.end(), 0);
		sort(ord.begin(), ord.end(), [&](int i, int j) {
			const int iblock = ls[i] / w, jblock = ls[j] / w;
			if (iblock != jblock) return iblock < jblock;
			return (iblock & 1) ? rs[i] > rs[j] : rs[i] < rs[j];
			});
		int l = 0, r = 0;
		for (int i : ord) {
			while (ls[i] < l) add_left(--l);
			while (ls[i] > l) del_left(l++);
			while (rs[i] < r) del_right(--r);
			while (rs[i] > r) add_right(r++);
			out(i);
		}
	}

	template<typename A, typename D, typename O>
	void build(const A& add, const D& del, const O& out) { build(add, add, del, del, out); }
};