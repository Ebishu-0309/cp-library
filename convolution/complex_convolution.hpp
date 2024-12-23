vector<complex<double>> convolution(vector<complex<double>> a, vector<complex<double>> b) {
	const auto zeta = [](double t) { return complex<double>(cos(2 * Pi * t), sin(2 * Pi * t)); };

	const int s = a.size(), t = b.size();
	int n = 1;
	while (n < s + t - 1) n <<= 1;
	a.resize(n); b.resize(n);

	const double ninv = 1.0 / n;

	vector<complex<double>> ws(n + 1);
	for (int i = 0; i <= n; ++i) {
		ws[i] = zeta(i * ninv);
	}

	for (int n1 = n / 2; n1 > 0; n1 /= 2) {
		for (int i = 0; i < n; i += n1 * 2) {
			for (int j = i; j < i + n1; ++j) {
				const auto w = ws[(j - i) * (n / (n1 * 2))];

				auto u = a[j], v = a[j + n1];
				a[j] = u + v, a[j + n1] = (u - v) * w;

				u = b[j], v = b[j + n1];
				b[j] = u + v, b[j + n1] = (u - v) * w;
			}
		}
	}

	for (int i = 0; i < n; ++i) {
		a[i] *= b[i] * ninv;
	}

	for (int n1 = 1; n1 < n; n1 *= 2) {
		for (int i = 0; i < n; i += n1 * 2) {
			for (int j = i; j < i + n1; ++j) {
				const auto w = ws[n - (j - i) * (n / (n1 * 2))];

				auto u = a[j], v = a[j + n1];
				a[j] = u + v * w, a[j + n1] = u - v * w;
			}
		}
	}

	a.resize(s + t - 1);
	return a;
}
vector<double> convolution(vector<double> a, vector<double> b) {
	const int s = a.size(), t = b.size();
	vector<complex<double>> a1(s), b1(t);
	for (int i = 0; i < s; ++i) a1[i] = a[i];
	for (int i = 0; i < t; ++i) b1[i] = b[i];
	const auto c1 = convolution(a1, b1);
	vector<double> c(s + t - 1);
	for (int i = 0; i < s + t - 1; ++i) c[i] = c1[i].real();
	return c;
}