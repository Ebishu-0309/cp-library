using F = long double;
constexpr F EPS = 1e-13;

int sgn(F x) { return (x < -EPS ? -1 : (x > EPS ? 1 : 0)); }
bool same(F a, F b) { return sgn(a - b) == 0; }

struct vec2 {
    F x, y;
    vec2() = default;
    vec2(F x, F y) : x(x), y(y) {}
    bool operator<(vec2 v) const {
        if (!same(x, v.x)) return x < v.x;
        return y < v.y;
    }
    bool operator==(vec2 v) const { return same(x, v.x) && same(y, v.y); }

    vec2 operator-() const { return {-x, -y}; }
    vec2 operator+(vec2 v) const { return {x + v.x, y + v.y}; }
    vec2 operator-(vec2 v) const { return (*this) + (-v); }
    vec2 operator*(F s) const { return {x * s, y * s}; }
    vec2 operator/(F s) const { return (*this) * (1 / s); }

    F dot(vec2 v) const { return x * v.x + y * v.y; }
    F cross(vec2 v) const { return x * v.y - y * v.x; }

    F lengthSq() const { return this->dot(*this); }
};

/*
A--B^^C: +1
A--B__C: -1
B--A--C: +2
A--B--C: -2
A--C--B: 0
*/
int ccw(vec2 a, vec2 b, vec2 c) {
    int res = sgn((b - a).cross(c - a));
    if (res != 0) return res;
    if (sgn((a - b).dot(c - a)) > 0) return +2;
    if (sgn((b - a).dot(c - b)) > 0) return -2;
    return 0;
}
