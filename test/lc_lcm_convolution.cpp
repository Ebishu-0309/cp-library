// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/lcm_convolution

#include "../template.hpp"

#include "../convolution/mul_div_convolution.hpp"

int main() {
	int n;
	cin >> n;
	vector<mint> a(n + 1), b(n + 1);
	rep1e(i, n) cin >> a[i];
	rep1e(i, n) cin >> b[i];
	auto c = lcm_convolution(a, b);
	rep1e(i, n) cout << c[i] << (i == n ? "\n" : " ");
}