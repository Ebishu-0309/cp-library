template <typename T>
struct LinearFunction {
    using F = LinearFunction;
    T a, b;

    constexpr LinearFunction() : a(0), b(0) {}
    constexpr LinearFunction(T k) : a(0), b(k) {}
    constexpr LinearFunction(T a_, T b_) : a(a_), b(b_) {}

    static constexpr F id() { return {1, 0}; }

    constexpr bool operator==(const F &f) const { return a == f.a && b == f.b; }

    constexpr F operator-() const { return {-a, -b}; }
    constexpr F operator+(const F &f) const { return F(*this) += f; }
    constexpr F operator-(const F &f) const { return F(*this) -= f; }
    constexpr F operator*(T k) const { return F(*this) *= k; }
    constexpr F operator/(T k) const { return F(*this) /= k; }

    constexpr F &operator+=(const F &f) {
        a += f.a, b += f.b;
        return *this;
    }
    constexpr F &operator-=(const F &f) { return *this += (-f); }
    constexpr F &operator*=(T k) {
        a *= k, b *= k;
        return *this;
    }
    constexpr F &operator/=(T k) {
        a /= k, b /= k;
        return *this;
    }

    // f(g(x))
    constexpr F composition(F g) const { return {a * g.a, a * g.b + b}; }

    constexpr F inv() const { return {1 / a, -b / a}; }

    // f(x-k)
    constexpr F shifted(T k) const { return F(*this).shifted(k); }
    constexpr F &shift(T k) {
        b -= a * k;
        return *this;
    }

    constexpr T calc(T x) const { return a * x + b; }
    constexpr T operator()(T x) const { return calc(x); }
    constexpr F operator()(const F &g) const { return composition(g); }

    friend constexpr F operator*(T k, const F &p) { return p * k; }
    friend istream &operator>>(istream &is, F &f) { return is >> f.a >> f.b; }
};