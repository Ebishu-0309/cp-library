template<class S, S(*op)(S, S), S(*e)()>
class SegmentTree2D {
public:
	SegmentTree2D(int _h, int _w) : SegmentTree2D(vector(_h, vector(_w, e()))) {}

	SegmentTree2D(vector<vector<S>> a) {
		int sizH = (int)(a.size()), sizW = (int)(a[0].size());
		logH = ceil_log2(sizH), logW = ceil_log2(sizW);
		h = (1 << logH), w = (1 << logW);

		dat.resize((2 * w) * (2 * h), e());
		for (int i = 0; i < sizH; ++i) {
			for (int j = 0; j < sizW; ++j) {
				get(i + h, j + w) = a[i][j];
			}
		}
		for (int i = 0; i < h; ++i) {
			for (int j = w - 1; j > 0; --j) {
				updateW(i + h, j);
			}
		}
		for (int j = 1; j < 2 * w; ++j) {
			for (int k = h - 1; k > 0; --k) {
				updateH(j, k);
			}
		}
	}

	void set(int i, int j, S x) {
		i += h; j += w;
		get(i, j) = x;
		for (j >>= 1; j > 0; j >>= 1) {
			updateW(i, j);
			for (int k = (i >>= 1); k > 0; k >>= 1) {
				updateH(j, k);
			}
		}
	}

	S prod(int i0, int j0, int i1, int j1) {
		i0 += h; i1 += h;
		S sl = e(), sr = e();
		while (i0 < i1) {
			if (i0 & 1) sl = op(sl, prodW(i0++, j0, j1));
			if (i1 & 1) sr = op(prodW(--i1, j0, j1), sr);
			i0 >>= 1; i1 >>= 1;
		}
		return op(sl, sr);
	}

	S at(int i, int j) { return get(i + h, j + w); }

private:
	int h, w, logH, logW;
	vector<S> dat;

	int ceil_log2(int x) {
		int res = 0;
		while ((1 << res) < x) ++res;
		return res;
	}

	S& get(int i, int j) { return dat[(2 * w) * i + j]; }

	void updateW(int i, int j) {
		get(i, j) = op(get(i, 2 * j), get(i, 2 * j + 1));
	}
	void updateH(int j, int k) {
		get(k, j) = op(get(2 * k, j), get(2 * k + 1, j));
	}

	S prodW(int k, int l, int r) {
		l += w; r += w;
		S sl = e(), sr = e();
		while (l < r) {
			if (l & 1) sl = op(sl, get(k, l++));
			if (r & 1) sr = op(get(k, --r), sr);
			l >>= 1; r >>= 1;
		}
		return op(sl, sr);
	}
};