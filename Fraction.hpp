template <typename T>
struct Fraction {
    using Float = long double;
    T p, q;  // p/q

    Fraction() : p(0), q(1) {}

    template <typename U>
    Fraction(U n) : p(n), q(1) {}

    template <typename U, typename V>
    Fraction(U p_, V q_) : p(p_), q(q_) {}

    bool operator==(const Fraction& f) const { return p * f.q == q * f.p; }

    Fraction operator-() const { return {-p, q}; }

    Fraction operator+(const Fraction& f) const { return {p * f.q + f.p * q, q * f.q}; }
    Fraction operator-(const Fraction& f) const { return this->operator+(-f); }
    Fraction operator*(const Fraction& f) const { return {p * f.p, q * f.q}; }

    Fraction inv() const { return {q, p}; }
    Fraction operator/(const Fraction& f) const { return *this * f.inv(); }

    Fraction& operator+=(const Fraction& f) { return *this = *this + f; }
    Fraction& operator-=(const Fraction& f) { return *this = *this - f; }
    Fraction& operator*=(const Fraction& f) { return *this = *this * f; }
    Fraction& operator/=(const Fraction& f) { return *this = *this / f; }

    int sgn(T x) const { return x == 0 ? 0 : x < 0 ? -1 : 1; }
    int sgn() const { return sgn(p) * sgn(q); }
    int sgn_add(const Fraction& f) const { return sgn(p * f.q + f.p * q) * sgn(q) * sgn(f.q); }

    template <typename U>
    U val() const {
        return U(p) / q;
    }

    bool operator<(const Fraction& f) const {
        return this->val<Float>() < f.val<Float>();
        // return f.sgn_add(-*this) > 0;
    }
    bool operator<=(const Fraction& f) const { return *this < f || *this == f; }
    bool operator>(const Fraction& f) const { return f < *this; }
    bool operator>=(const Fraction& f) const { return f <= *this; }

    friend ostream& operator<<(ostream& os, const Fraction& f) { return os << f.p << "/" << f.q; }
};