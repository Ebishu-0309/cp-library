// log_x(y) mod m
int modlog(int x, int y, int m) {
    if (m == 1 || y == 1) return 0;
    if (x == 0) {
        if (y == 0) return 1;
        if (y == 1) return 0;
        return -1;
    }
    const int g = gcd(x, m);
    if (g == 1) {
        modint::set_mod(m);
        const int p = ceil(sqrt(m));

        unordered_map<int, int> mp;
        mp.reserve(p);
        modint x_pow = 1;
        for (int j = 0; j < p; ++j) {
            auto itr = mp.find(x_pow.val());
            if (itr == mp.end()) {
                mp[x_pow.val()] = j;
            }
            x_pow *= x;
        }

        modint a = modint(x).inv().pow(p).val(), a_pow = 1;
        for (int i = 0; i < p; ++i) {
            const int v = (a_pow * y).val();
            auto itr = mp.find(v);
            if (itr != mp.end()) {
                int j = (*itr).second;
                return p * i + j;
            }
            a_pow *= a;
        }
        return -1;
    }

    if (y % g != 0) return -1;
    const int x2 = x / g, y2 = y / g, m2 = m / g;
    modint::set_mod(m2);
    const int k2 = modlog(x, (modint(y2) / x2).val(), m2);
    if (k2 == -1) return -1;
    return k2 + 1;
}
