namespace structures {
	struct S {
		int val, cnt;

		static S op(S x, S y) {
			if (x.val != y.val) return (x.val < y.val ? x : y);
			else return { x.val, x.cnt + y.cnt };
		}
		static S e() { return { INT_MAX, 0 }; }
	};
	struct F {
		int add;

		static S mapping(F f, S x) { return { x.val + f.add, x.cnt }; }
		static F composition(F f, F g) { return { g.add + f.add }; }
		static F id() { return { 0 }; }
	};
}
long long rectangle_union(vector<int> l, vector<int> r, vector<int> d, vector<int> u) {
	using namespace structures;

	const int n = l.size();
	vector<int> xs(2 * n), ys(2 * n);
	copy(l.begin(), l.end(), xs.begin()); copy(r.begin(), r.end(), xs.begin() + n);
	copy(d.begin(), d.end(), ys.begin()); copy(u.begin(), u.end(), ys.begin() + n);
	sort(xs.begin(), xs.end()); sort(ys.begin(), ys.end());
	xs.erase(unique(xs.begin(), xs.end()), xs.end()); ys.erase(unique(ys.begin(), ys.end()), ys.end());

	const int m = xs.size(), w = ys.size();

	vector<int> l1 = l, r1 = r, d1 = d, u1 = u;
	for (auto& e : l1) e = lower_bound(xs.begin(), xs.end(), e) - xs.begin();
	for (auto& e : r1) e = lower_bound(xs.begin(), xs.end(), e) - xs.begin();
	for (auto& e : d1) e = lower_bound(ys.begin(), ys.end(), e) - ys.begin();
	for (auto& e : u1) e = lower_bound(ys.begin(), ys.end(), e) - ys.begin();

	vector<vector<pair<int, pair<int, int>>>> lr(w);
	{
		vector<int> siz(w);
		for (int i = 0; i < n; ++i) siz[d1[i]]++, siz[u1[i]]++;
		for (int i = 0; i < w; ++i) lr[i].reserve(siz[i]);
	}
	for (int i = 0; i < n; ++i) {
		lr[d1[i]].emplace_back(1, make_pair(l1[i], r1[i]));
		lr[u1[i]].emplace_back(-1, make_pair(l1[i], r1[i]));
	}

	lazy_segtree<S, S::op, S::e, F, F::mapping, F::composition, F::id> seg(m);
	for (int i = 0; i < m - 1; ++i) seg.set(i, S{ 0, xs[i + 1] - xs[i] });

	long long res = 0;
	for (int i = 0; i < w - 1; ++i) {
		for (auto [k, p] : lr[i]) seg.apply(p.first, p.second, F{ k });

		auto [val, cnt] = seg.all_prod();
		long long tmp = (xs[m - 1] - xs[0]) - (val == 0 ? cnt : 0);
		res += tmp * (ys[i + 1] - ys[i]);
	}
	return res;
}
long long rectangle_union_small(vector<int> l, vector<int> r, vector<int> d, vector<int> u) {
	using namespace structures;

	const int n = l.size();
	const int m = *max_element(r.begin(), r.end()), w = *max_element(u.begin(), u.end()) + 1;

	vector<vector<pair<int, pair<int, int>>>> lr(w);
	{
		vector<int> siz(w);
		for (int i = 0; i < n; ++i) siz[d[i]]++, siz[u[i]]++;
		for (int i = 0; i < w; ++i) lr[i].reserve(siz[i]);
	}
	for (int i = 0; i < n; ++i) {
		lr[d[i]].emplace_back(1, make_pair(l[i], r[i]));
		lr[u[i]].emplace_back(-1, make_pair(l[i], r[i]));
	}

	lazy_segtree<S, S::op, S::e, F, F::mapping, F::composition, F::id> seg(m);
	for (int i = 0; i < m; ++i) seg.set(i, S{ 0, 1 });

	long long res = 0;
	for (int i = 0; i < w - 1; ++i) {
		for (auto [k, p] : lr[i]) seg.apply(p.first, p.second, F{ k });

		auto [val, cnt] = seg.all_prod();
		res += m - (val == 0 ? cnt : 0);
	}
	return res;
}