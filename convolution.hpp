template<typename T>
void fzt_sup(vector<T>& f) {
	const int n = f.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; ++j) {
			if (~j & i) {
				f[j] += f[j | i];
			}
		}
	}
}
template<typename T>
void fzt_sub(vector<T>& f) {
	const int n = f.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; ++j) {
			if (~j & i) {
				f[j | i] += f[j];
			}
		}
	}
}
template<typename T>
void ifzt_sup(vector<T>& f) {
	const int n = f.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; ++j) {
			if (~j & i) {
				f[j] -= f[j | i];
			}
		}
	}
}
template<typename T>
void ifzt_sub(vector<T>& f) {
	const int n = f.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; ++j) {
			if (~j & i) {
				f[j | i] -= f[j];
			}
		}
	}
}
template <typename T>
void fwt(vector<T>& f) {
	const int n = f.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; ++j) {
			if (~j & i) {
				const T x = f[j], y = f[j | i];
				f[j] = x + y, f[j | i] = x - y;
			}
		}
	}
}
template <typename T>
void ifwt(vector<T>& f) {
	const int n = f.size();
	static const T inv2 = T(1) / 2;
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; ++j) {
			if (~j & i) {
				const T x = f[j], y = f[j | i];
				f[j] = (x + y) * inv2, f[j | i] = (x - y) * inv2;
			}
		}
	}
}

// “Y‚¦Žš AND ô‚Ýž‚Ý
template<typename T>
vector<T> bitwise_and_convolution(vector<T> a, vector<T> b) {
	fzt_sup(a); fzt_sup(b);
	const int n = a.size();
	for (int i = 0; i < n; ++i) a[i] *= b[i];
	ifzt_sup(a);
	return a;
}
// “Y‚¦Žš OR ô‚Ýž‚Ý
template<typename T>
vector<T> bitwise_or_convolution(vector<T> a, vector<T> b) {
	fzt_sub(a); fzt_sub(b);
	const int n = a.size();
	for (int i = 0; i < n; ++i) a[i] *= b[i];
	ifzt_sub(a);
	return a;
}
// “Y‚¦Žš XOR ô‚Ýž‚Ý
template<typename T>
vector<T> bitwise_xor_convolution(vector<T> a, vector<T> b) {
	fwt(a); fwt(b);
	const int n = a.size();
	for (int i = 0; i < n; ++i) a[i] *= b[i];
	ifwt(a);
	return a;
}