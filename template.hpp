#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <complex>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#if __cplusplus >= 202002L
#include <bit>
#include <ranges>
#define TYPE(n) remove_cvref_t<decltype(n)>
#else
#define countl_zero __builtin_clzll
#define TYPE(n) remove_cv_t<remove_reference_t<decltype(n)>>
#endif

using namespace std;
using lint = long long;
using P = pair<lint, lint>;
using Pii = pair<int, int>;
using ull = unsigned long long;

struct FastIO {
    FastIO() {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
        cout << fixed << setprecision(15);
    }
} aaaAAAaaaAAA;

#define rep(i, n) for (TYPE(n) i = 0; i < (n); ++i)
#define repe(i, n) for (TYPE(n) i = 0; i <= (n); ++i)
#define rep1(i, n) for (TYPE(n) i = 1; i < (n); ++i)
#define rep1e(i, n) for (TYPE(n) i = 1; i <= (n); ++i)
#define repn(i, a, b) for (TYPE(a) i = (a); i < (b); ++i)
#define repne(i, a, b) for (TYPE(a) i = (a); i <= (b); ++i)
#define rrep(i, n) for (TYPE(n) i = (n); i >= 0; --i)
#define all(vec) begin(vec), end(vec)
#define rall(vec) rbegin(vec), rend(vec)

constexpr long long Mod = /** 1000'000'007LL /*/ 998244353LL /**/;
constexpr long long Inf = 2'000'000'000'000'000'010LL;
constexpr int IntInf = 1000'000'010;
constexpr double Pi = 3.141592653589793238;
constexpr double InvPi = 0.318309886183790671;

const int di[] = {0, -1, 0, 1, 0};
const int dj[] = {1, 0, -1, 0, 0};
pair<int, int> adj(int i, int j, int k) { return {i + di[k], j + dj[k]}; }
bool in(int i, int j, int h, int w) { return 0 <= i && i < h && 0 <= j && j < w; }

#if __has_include(<atcoder/all>)
#include <atcoder/all>

using namespace atcoder;
using mint = static_modint<Mod>;

template <int MOD>
inline istream &operator>>(istream &is, static_modint<MOD> &rhs) {
    long long tmp;
    is >> tmp;
    rhs = tmp;
    return is;
}
template <int ID>
inline istream &operator>>(istream &is, dynamic_modint<ID> &rhs) {
    long long tmp;
    is >> tmp;
    rhs = tmp;
    return is;
}
template <int MOD>
inline ostream &operator<<(ostream &os, const static_modint<MOD> &rhs) {
    return os << rhs.val();
}
template <int ID>
inline ostream &operator<<(ostream &os, const dynamic_modint<ID> &rhs) {
    return os << rhs.val();
}
// [0, n]
template <typename T>
auto enumerate_fact(int n) {
    vector<T> fact(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = i * fact[i - 1];
    return fact;
}
// [0, n]
template <int MOD>
auto enumerate_inv(int n) {
    vector<static_modint<MOD>> inv(n + 1);
    inv[1] = 1;
    for (int i = 2; i <= n; ++i) inv[i] = MOD - MOD / i * inv[MOD % i];
    return inv;
}
// [0, n]
template <typename T>
auto enumerate_factinv(int n, vector<T> inv) {
    vector<T> fact_inv(n + 1);
    fact_inv[0] = 1;
    for (int i = 1; i <= n; ++i) fact_inv[i] = fact_inv[i - 1] * inv[i];
    return fact_inv;
}
// [0, n]
template <int MOD>
auto enumerate_factinv(int n) {
    return enumerate_factinv(n, enumerate_inv<MOD>(n));
}

template <int MOD>
struct Binomial {
    using Fp = static_modint<MOD>;
    vector<Fp> fact, inv, fact_inv;

    explicit Binomial() = default;

    // [0, n]
    void build(int n) {
        fact = enumerate_fact<Fp>(n);
        inv = enumerate_inv<MOD>(n);
        fact_inv = enumerate_factinv<Fp>(n, inv);
    }

    Fp comb(int n, int r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        if (r == 0 || r == n) return 1;
        return fact[n] * fact_inv[n - r] * fact_inv[r];
    }

    Fp perm(int n, int r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        return fact[n] * fact_inv[n - r];
    }

    Fp multi(int n, int r) const {
        if (n == 0 && r == 0) return 1;
        if (n < 0 || r < 0) return 0;
        return r == 0 ? 1 : comb(n + r - 1, r);
    }
};
Binomial<Mod> binomial;
inline mint fact(int n) { return binomial.fact[n]; }
inline mint comb(int n, int r) { return binomial.comb(n, r); }
inline mint perm(int n, int r) { return binomial.perm(n, r); }
inline mint multi(int n, int r) { return binomial.multi(n, r); }

mint lagrange_interpolation(const vector<mint> &y, mint t) {
    const int n = (int)y.size();

    mint res = 0;

    auto inv = enumerate_inv<Mod>(n - 1), fact_inv = enumerate_factinv(n - 1, inv);

    vector<mint> prod2(n);
    prod2.back() = 1;
    for (int i = n - 1; i > 0; --i) {
        prod2[i - 1] = (t - i) * prod2[i];
    }

    mint prod1 = 1;
    for (int i = 0; i < n; ++i) {
        mint a = y[i];
        a *= fact_inv[i] * fact_inv[n - 1 - i];
        if ((n - 1 - i) & 1) a = -a;

        res += a * prod1 * prod2[i];

        prod1 *= (t - i);
    }

    return res;
}

template <typename T>
lint inversion_number(const vector<T> vec) {
    vector<T> v = vec;
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());

    const int n = vec.size();

    lint res = 0;

    fenwick_tree<int> b(n);
    for (int i = 0; i < n; ++i) {
        const int j = lower_bound(v.begin(), v.end(), vec[i]) - v.begin();
        res += b.sum(j + 1, n);
        b.add(j, 1);
    }

    return res;
}
#endif

// top = max
template <typename T>
using prique = priority_queue<T>;
// top = min
template <typename T>
using prique_inv = priority_queue<T, vector<T>, greater<T>>;

template <typename T, typename U>
inline istream &operator>>(istream &is, pair<T, U> &rhs) {
    return is >> rhs.first >> rhs.second;
}
template <typename T>
inline istream &operator>>(istream &is, complex<T> &c) {
    T real, imag;
    is >> real >> imag;
    c.real(real);
    c.imag(imag);
    return is;
}
template <typename T, typename U>
inline ostream &operator<<(ostream &os, const pair<T, U> &rhs) {
    return os << "{" << rhs.first << ", " << rhs.second << "}";
}

#if __cplusplus >= 202002L
template <class T>
concept Container = requires(const T &v) {
    v.begin();
    v.end();
} && !is_same_v<T, string>;
template <Container T>
inline istream &operator>>(istream &is, T &v) {
    for (auto &e : v) is >> e;
    return is;
}
template <Container T>
inline ostream &operator<<(ostream &os, const T &v) {
    for (auto itr = v.begin(), end_itr = v.end(); itr != end_itr;) {
        os << *itr;
        if (++itr != end_itr) os << " ";
    }
    return os;
}
#else
template <typename T>
inline istream &operator>>(istream &is, vector<T> &v) {
    for (auto &e : v) is >> e;
    return is;
}
template <typename T>
inline ostream &operator<<(ostream &os, const vector<T> &v) {
    for (auto itr = v.begin(), end_itr = v.end(); itr != end_itr;) {
        os << *itr;
        if (++itr != end_itr) os << " ";
    }
    return os;
}
#endif

template <typename T, typename U>
inline bool chmin(T &a, const U b) {
    return a > b ? a = b, true : false;
}
template <typename T, typename U>
inline bool chmax(T &a, const U b) {
    return a < b ? a = b, true : false;
}
template <typename T, typename U, class Pr>
inline int getid(const vector<T> &v, const U &value, Pr pred) {
    return lower_bound(v.begin(), v.end(), value, pred) - v.begin();
}
template <typename T, typename U>
inline int getid(const vector<T> &v, const U &value) {
    return getid(v, value, less<>{});
}

template <typename T>
T gcd(const vector<T> &vec) {
    T res = vec.front();
    for (T e : vec) {
        res = gcd(res, e);
        if (res == 1) return 1;
    }
    return res;
}
template <typename T>
T gcd(initializer_list<T> init) {
    auto first = init.begin(), last = init.end();
    T res = *(first++);
    for (auto itr = first; itr != last; ++itr) {
        res = gcd(res, *itr);
        if (res == 1) return 1;
    }
    return res;
}
template <typename T>
T lcm(const vector<T> &vec) {
    T res = vec.front();
    for (T e : vec) res = lcm(res, e);
    return res;
}
template <typename T>
T lcm(initializer_list<T> init) {
    auto first = init.begin(), last = init.end();
    T res = *(first++);
    for (auto itr = first; itr != last; ++itr) {
        res = lcm(res, *itr);
    }
    return res;
}

inline void YesNo(bool b) { cout << (b ? "Yes\n" : "No\n"); }
inline void YESNO(bool b) { cout << (b ? "YES\n" : "NO\n"); }
inline void takaao(bool b) { cout << (b ? "Takahashi\n" : "Aoki\n"); }
inline void aotaka(bool b) { cout << (b ? "Aoki\n" : "Takahashi\n"); }

// [l, r]
template <typename T>
T rand(T l, T r) {
    static mt19937 mt(random_device{}());
    if constexpr (is_integral_v<T>) {
        uniform_int_distribution<T> dist(l, r);
        return dist(mt);
    } else if constexpr (is_floating_point_v<T>) {
        uniform_real_distribution<T> dist(l, r);
        return dist(mt);
    }
}

bool is_prime_naive(lint x) {
    for (lint i = 2; i * i <= x; ++i) {
        if (x % i == 0) return false;
    }
    return 1 < x;
}

vector<lint> divisors(lint n) {
    vector<lint> f, l;
    for (lint x = 1; x * x <= n; ++x) {
        if (n % x == 0) {
            f.push_back(x);
            if (x * x != n) l.push_back(n / x);
        }
    }
    f.reserve(f.size() + l.size());
    copy(l.rbegin(), l.rend(), back_inserter(f));
    return f;
}

lint phi(lint n) {
    lint r = n;
    for (lint i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            r -= r / i;
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) r -= r / n;
    return r;
}

lint floor_sqrt(lint n) {
    if (n == 0 || n == 1) return n;
    lint x0 = 1LL << ((65 - countl_zero(static_cast<uint64_t>(n))) >> 1);
    lint x1 = (x0 + n / x0) >> 1;
    while (x1 < x0) {
        x0 = x1;
        x1 = (x0 + n / x0) >> 1;
    }
    return x0;
}
lint ceil_sqrt(lint n) {
    const lint f = floor_sqrt(n);
    if (f * f == n) return f;
    return f + 1;
}

template <typename T>
constexpr bool is_intersect(T l1, T r1, T l2, T r2) {
    return l1 <= r2 && l2 <= r1;
}
template <typename T>
constexpr bool is_intersect2(T l1, T r1, T l2, T r2) {
    return l1 < r2 && l2 < r1;
}

lint modinv(lint a, lint m = Mod) {
    lint b = m, u = 1, v = 0;
    while (b != 0) {
        lint t = a / b;
        a -= t * b;
        swap(a, b);
        u -= t * v;
        swap(u, v);
    }
    u %= m;
    if (u < 0) u += m;
    return u;
}
lint modpow(lint x, lint n, lint m = Mod) {
    if (m == 1) return 0;
    lint res = 1;
    x %= m;
    if (x < 0) x += m;
    while (n > 0) {
        if (n & 1) res = res * x % m;
        x = x * x % m;
        n >>= 1;
    }
    return res;
}
lint intpow(lint x, lint n) {
    lint res = 1;
    while (n > 0) {
        if (n & 1) res *= x;
        x *= x;
        n >>= 1;
    }
    return res;
}

template <typename T>
vector<T> compressed(vector<T> v) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    return v;
}

class Sieve {
   private:
    const int max_n;
    vector<int> sieve;

   public:
    explicit Sieve(int max_n) : max_n(max_n), sieve(max_n + 1) {
        iota(sieve.begin(), sieve.end(), 0);

        for (int i = 2; i * i <= max_n; ++i) {
            if (sieve[i] < i) continue;

            for (int j = i * i; j <= max_n; j += i) {
                if (sieve[j] == j) sieve[j] = i;
            }
        }
    }

    unordered_map<int, int> calc(int x) const {
        assert(x <= max_n);
        unordered_map<int, int> res;
        while (x > 1) {
            ++res[sieve[x]];
            x /= sieve[x];
        }
        return res;
    }

    vector<int> enumerate_prime(int x) const {
        assert(x <= max_n);
        vector<int> primes;
        for (int i = 2; i <= x; ++i) {
            if (sieve[i] == i) {
                primes.push_back(i);
            }
        }
        return primes;
    }
};

struct UnionFind {
    int n;
    vector<int> par, rank, siz, es;  // [root(i)]
    int c;

    UnionFind() = default;

    explicit UnionFind(int _n) : n(_n), par(_n), rank(_n), siz(_n, 1), es(_n), c(_n) { iota(par.begin(), par.end(), 0); }

    int root(int x) {
        while (par[x] != x) x = par[x] = par[par[x]];
        return x;
    }

    bool same(int x, int y) { return root(x) == root(y); }

    void unite(int x, int y) {
        if (x == y) return;

        x = root(x);
        y = root(y);
        if (x == y)
            ++es[x];
        else {
            c--;
            if (rank[x] < rank[y]) {
                par[x] = y;
                siz[y] += siz[x];
                es[y] += es[x] + 1;
            } else {
                par[y] = x;
                if (rank[x] == rank[y]) ++rank[x];
                siz[x] += siz[y];
                es[x] += es[y] + 1;
            }
        }
    }

    int size(int x) { return siz[root(x)]; }

    vector<int> roots() {
        vector<int> res;
        res.reserve(c);

        for (int i = 0; i < n; ++i) {
            if (par[i] == i) {
                res.push_back(i);
            }
        }

        return res;
    }

    vector<vector<int>> groups() {
        vector<vector<int>> res(n);

        for (int i = 0; i < n; ++i)
            if (par[i] == i) res[i].reserve(siz[i]);
        for (int i = 0; i < n; ++i) res[root(i)].push_back(i);

        res.erase(remove_if(res.begin(), res.end(), [](const vector<int> &v) { return v.empty(); }), res.end());

        return res;
    }
};

template <typename T>
class BinaryIndexedTree {
   private:
    int n;
    vector<T> dat;

   public:
    BinaryIndexedTree() = default;

    explicit BinaryIndexedTree(const int size) : n(size), dat(size + 1) {}

    explicit BinaryIndexedTree(const vector<T> &vec) : n(vec.size()), dat(n + 1) {
        for (int i = 0; i < n; ++i) dat[i + 1] = vec[i];
        for (int i = 1; i <= n; ++i) {
            const int j = i + (i & -i);
            if (j <= n) dat[j] += dat[i];
        }
    }

    // 0-indexed
    void add(const int a, const T v) {
        for (int x = a + 1; x <= n; x += (x & -x)) dat[x] += v;
    }

    // 0-indexed
    void set(const int a, const T v) { add(a, v - get(a)); }

    void reset() { fill(dat.begin(), dat.end(), T(0)); }

    // [0, a)
    T sum(const int a) const {
        T res = 0;
        for (int x = a; x > 0; x -= (x & -x)) res += dat[x];
        return res;
    }

    // [a, b)
    T sum(const int a, const int b) const { return sum(b) - sum(a); }

    T get(const int i) const { return sum(i, i + 1); }

    // min i s.t. sum(i) >= w
    int lower_bound(T w) const {
        int x = 0, r = 1;
        while (r < n) r <<= 1;
        for (int i = r; i > 0; i >>= 1) {
            if (x + i <= n && dat[x + i] < w) {
                w -= dat[x + i];
                x += i;
            }
        }
        return x + 1;
    }
    // min i s.t. sum(i) > w
    int upper_bound(T w) const {
        int x = 0, r = 1;
        while (r < n) r <<= 1;
        for (int i = r; i > 0; i >>= 1) {
            if (x + i <= n && dat[x + i] <= w) {
                w -= dat[x + i];
                x += i;
            }
        }
        return x + 1;
    }
};

constexpr int msb(long long x) { return 63 - countl_zero(static_cast<uint64_t>(x)); }