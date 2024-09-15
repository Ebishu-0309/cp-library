#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <set>
#include <vector>

using namespace std;

namespace geometry {

using floating_point_type = long double;

constexpr floating_point_type EPS = 1e-13;
constexpr floating_point_type Pi = 3.141592653589793;

constexpr floating_point_type operator"" _deg(floating_point_type deg) { return static_cast<floating_point_type>(deg * Pi / 180.0); }
constexpr floating_point_type operator"" _deg(unsigned long long deg) { return static_cast<floating_point_type>(deg * Pi / 180.0); }

template <typename T, enable_if_t<is_integral_v<T>, nullptr_t> = nullptr>
constexpr int sgn(T a) noexcept {
    return (a < 0 ? -1 : (a > 0 ? 1 : 0));
}
template <typename T, enable_if_t<is_floating_point_v<T>, nullptr_t> = nullptr>
constexpr int sgn(T a) noexcept {
    return (a < -EPS ? -1 : (a > EPS ? +1 : 0));
}

template <typename T, typename U>
constexpr bool same(T a, U b) noexcept {
    return sgn(a - b) == 0;
}

template <typename T>
constexpr T square(T x) noexcept {
    return x * x;
}

// ax + by + c = 0, gcd(a,b,c) = 1
// a != 0 : a > 0
// a = 0: b > 0
tuple<long long, long long, long long> general_form(long long x1, long long y1, long long x2, long long y2) {
    long long a = y1 - y2, b = x2 - x1, c = x1 * y2 - x2 * y1;
    if (a < 0) a = -a, b = -b, c = -c;
    if (a == 0 && b < 0) b = -b, c = -c;
    long long g = gcd(gcd(a, b), c);
    return {a / g, b / g, c / g};
}

template <typename T>
struct Vector2D {
    using value_type = T;
    T x, y;

    Vector2D() = default;
    explicit Vector2D(T arg) : x(cos(arg)), y(sin(arg)) {}
    constexpr Vector2D(T x, T y) noexcept : x(x), y(y) {}

    constexpr bool operator<(const Vector2D& v) const noexcept {
        if (!same(x, v.x)) return x < v.x;
        return y < v.y;
    }
    constexpr bool operator>(const Vector2D& v) const noexcept { return v < *this; }
    constexpr bool operator==(const Vector2D& v) const noexcept { return same(x, v.x) && same(y, v.y); }
    constexpr bool operator!=(const Vector2D& v) const noexcept { return !(*this == v); }

    constexpr Vector2D operator+() const noexcept { return *this; }
    constexpr Vector2D operator-() const noexcept { return {-x, -y}; }

    constexpr Vector2D operator+(const Vector2D& v) const noexcept { return {x + v.x, y + v.y}; }
    constexpr Vector2D operator-(const Vector2D& v) const noexcept { return {x - v.x, y - v.y}; }
    constexpr Vector2D operator*(T s) const noexcept { return {x * s, y * s}; }
    constexpr Vector2D operator/(T s) const noexcept { return {x / s, y / s}; }

    constexpr Vector2D& operator+=(const Vector2D& v) noexcept {
        x += v.x;
        y += v.y;
        return *this;
    }
    constexpr Vector2D& operator-=(const Vector2D& v) noexcept {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    constexpr Vector2D& operator*=(T s) noexcept {
        x *= s;
        y *= s;
        return *this;
    }
    constexpr Vector2D& operator/=(T s) noexcept {
        x /= s;
        y /= s;
        return *this;
    }

    constexpr bool isZero() const noexcept { return x == T(0) && y == T(0); }
    bool hasNan() const { return std::isnan(x) || std::isnan(y); }

    constexpr T dot(const Vector2D& v) const noexcept { return x * v.x + y * v.y; }
    constexpr T cross(const Vector2D& v) const noexcept { return x * v.y - y * v.x; }

    floating_point_type length() const {
        if (isZero()) return 0.0;

        const T abs_x = std::abs(x);
        const T abs_y = std::abs(y);

        const auto [u, v] = std::minmax(abs_x, abs_y);
        const T t = u / v;

        return v * std::sqrt(std::fma(t, t, 1.0));
    }
    constexpr T lengthSq() const noexcept { return dot(*this); }

    floating_point_type distanceFrom(const Vector2D& v) const { return (v - *this).length(); }
    constexpr T distanceFromSq(const Vector2D& v) const noexcept { return (v - *this).lengthSq(); }

    Vector2D normalized() const { return *this / length(); }
    Vector2D& normalize() { return *this /= length(); }

    Vector2D rotated(floating_point_type angle) const {
        const T s = std::sin(angle);
        const T c = std::cos(angle);
        return {x * c - y * s, x * s + y * c};
    }
    Vector2D& rotate(floating_point_type angle) { return *this = rotated(angle); }

    floating_point_type arg() const { return atan2<floating_point_type>(y, x); }
    floating_point_type arg(const Vector2D& v) const {
        if (isZero() || v.isZero()) {
            return numeric_limits<T>::signaling_NaN();
        }

        return atan2<floating_point_type>(cross(v), dot(v));
    }
    floating_point_type arg(const Vector2D& a, const Vector2D& b) const { return (a - *this).arg(b - *this); }

    constexpr Vector2D lerp(const Vector2D& v, T f) const noexcept { return {x + (v.x - x) * f, y + (v.y - y) * f}; }

    constexpr friend Vector2D operator*(T s, const Vector2D& v) noexcept { return {s * v.x, s * v.y}; }
    friend istream& operator>>(istream& is, Vector2D& v) { return is >> v.x >> v.y; }
    friend ostream& operator<<(ostream& os, const Vector2D& v) { return os << "(" << v.x << ", " << v.y << ")"; }
};

template <typename T>
void argument_sort(vector<Vector2D<T>>& vec, const Vector2D<T>& center = Vector2D<T>(0, 0)) {
    auto orthant = [](const Vector2D<T>& v) {
        if (sgn(v.x) < 0 && sgn(v.y) < 0) return 0;
        if (sgn(v.x) >= 0 && sgn(v.y) < 0) return 1;
        if (sgn(v.x) == 0 && sgn(v.y) == 0) return 2;
        if (sgn(v.x) >= 0 && sgn(v.y) >= 0) return 3;
        return 4;
    };

    auto comp = [orthant, center](const Vector2D<T>& a, const Vector2D<T>& b) {
        const int o1 = orthant(a - center), o2 = orthant(b - center);
        if (o1 != o2) return o1 < o2;

        return (a - center).cross(b - center) > 0;
    };

    sort(vec.begin(), vec.end(), comp);
}

template <typename T>
vector<Vector2D<T>> convex_hull(vector<Vector2D<T>> points) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    int n = points.size(), k = 0;

    if (n <= 2) return points;

    vector<Vector2D<T>> res(n * 2);

    for (int i = 0; i < n; ++i) {
        while (k > 1 && sgn((res[k - 1] - res[k - 2]).cross(points[i] - res[k - 1])) <= 0) k--;
        res[k++] = points[i];
    }

    for (int i = n - 2, t = k; i >= 0; --i) {
        while (k > t && sgn((res[k - 1] - res[k - 2]).cross(points[i] - res[k - 1])) <= 0) k--;
        res[k++] = points[i];
    }

    res.resize(k - 1);
    return res;
}

/*
ABから見てBCは左に曲がるのなら +1
ABから見てBCは右に曲がるのなら -1
BACの順番で一直線上に並ぶなら +2
ABCの順番で一直線上に並ぶなら -2
CがAB上なら 0
*/
template <typename T>
constexpr int ccw(const Vector2D<T>& a, const Vector2D<T>& b, const Vector2D<T>& c) {
    const int res = sgn((b - a).cross(c - a));

    if (res != 0) return res;
    if (sgn((a - b).dot(c - a)) > 0) return +2;
    if (sgn((b - a).dot(c - b)) > 0) return -2;
    return 0;
}

using Vec2 = Vector2D<floating_point_type>;

struct Line {
    using value_type = Vec2::value_type;
    Vec2 begin, end;

    Line() = default;
    constexpr Line(const Vec2& begin, const Vec2& end) : begin(begin), end(end) {}
    constexpr Line(value_type sx, value_type sy, value_type gx, value_type gy) : begin(sx, sy), end(gx, gy) {}

    // ax + by + c = 0
    Line(value_type a, value_type b, value_type c) {
        assert(sgn(a) != 0 || sgn(b) != 0);

        if (sgn(b) == 0) {
            begin = Vec2(-c / a, 0.0);
            end = Vec2(-c / a, 1.0);
        } else {
            begin = Vec2(0, -c / b);
            end = Vec2(1.0, -(a + c) / b);
        }
    }

    constexpr Vec2 vector() const { return end - begin; }

    constexpr Line& reverse() {
        const Vec2 tmp = begin;
        begin = end;
        end = tmp;
        return *this;
    }
    constexpr Line reversed() const { return {end, begin}; }

    constexpr Line& moveBy(const Vec2& v) {
        begin += v;
        end += v;
        return *this;
    }
    constexpr Line movedBy(const Vec2& v) const { return {begin + v, end + v}; }

    constexpr bool isParallel(const Line& line) const { return sgn(vector().cross(line.vector())) == 0; }
    constexpr bool isOrthogonal(const Line& line) const { return sgn(vector().dot(line.vector())) == 0; }

    floating_point_type length() const { return vector().length(); }
    constexpr value_type lengthSq() const { return vector().lengthSq(); }

    floating_point_type distanceFrom(const Vec2& v) const { return std::abs(vector().cross(v - begin)) / length(); }
    constexpr value_type distanceFromSq(const Vec2& v) const {
        const value_type d = vector().cross(v - begin);

        return d / lengthSq() * d;
    }

    optional<Vec2> intersectsAt(const Line& line) const {
        if (sgn(vector().cross(line.vector())) == 0) {
            if (sgn(line.distanceFromSq(begin)) == 0) return begin;
            return nullopt;
        }

        return begin + vector() * ((line.end - begin).cross(line.vector()) / vector().cross(line.vector()));
    }

    constexpr Vec2 projection(const Vec2& v) const { return begin + vector() * (v - begin).dot(vector()) / lengthSq(); }
    constexpr Vec2 reflection(const Vec2& v) const { return 2 * projection(v) - v; }

    // ax + by + c = 0
    constexpr tuple<value_type, value_type, value_type> generalForm() const {
        const Vec2 vec = vector();
        return {vec.y, -vec.x, end.cross(begin)};
    }

    friend istream& operator>>(istream& is, Line& rhs) { return is >> rhs.begin >> rhs.end; }
};

namespace segment {

using Segment = Line;

constexpr bool intersect(const Segment& a, const Segment& b) {
    return ccw(a.begin, a.end, b.begin) * ccw(a.begin, a.end, b.end) <= 0 && ccw(b.begin, b.end, a.begin) * ccw(b.begin, b.end, a.end) <= 0;
}

constexpr floating_point_type distanceSq(const Segment& a, const Vec2& v) {
    if (sgn(a.vector().dot(v - a.begin)) < 0 || sgn((-a.vector()).dot(v - a.end)) < 0) {
        return min(v.distanceFromSq(a.begin), v.distanceFromSq(a.end));
    }
    return a.distanceFromSq(v);
}

floating_point_type distance(const Segment& a, const Vec2& v) { return std::sqrt(distanceSq(a, v)); }

floating_point_type distance(const Segment& a, const Segment& b) {
    if (intersect(a, b)) return 0.0;

    return min({distance(a, b.begin), distance(a, b.end), distance(b, a.begin), distance(b, a.end)});
}

constexpr Vec2 projection(const Segment& a, const Vec2& v) {
    if (sgn(a.vector().dot(v - a.begin)) < 0) return a.begin;
    if (sgn((-a.vector()).dot(v - a.end)) < 0) return a.end;
    return a.projection(v);
}

}  // namespace segment

struct Triangle {
    using value_type = Vec2::value_type;
    Vec2 p0, p1, p2;

    Triangle() = default;
    constexpr Triangle(const Vec2& p0, const Vec2& p1, const Vec2& p2) : p0(p0), p1(p1), p2(p2) {}

    constexpr Vec2& p(size_t index) { return (&p0)[index]; }
    constexpr const Vec2& p(size_t index) const { return (&p0)[index]; }

    constexpr Line side(size_t index) const {
        if (index == 0)
            return Line(p1, p2);
        else if (index == 1)
            return Line(p2, p0);
        else if (index == 2)
            return Line(p0, p1);
        else {
            throw std::out_of_range("Triangle::side() index out of range");
        }
    }

    floating_point_type angle(size_t index) const {
        const Vec2 vec[3] = {p(index), p((index + 1) % 3), p((index + 2) % 3)};

        const value_type res = vec[0].arg(vec[1], vec[2]);

        return fabs(res);
    }

    constexpr value_type area() const {
        const value_type res = (p1 - p0).cross(p2 - p0) / 2;
        return (0 < res ? res : -res);
    }

    constexpr Vec2 circumCenter() const {
        const value_type d0 = side(0).lengthSq(), d1 = side(1).lengthSq(), d2 = side(2).lengthSq(), t0 = d0 * (-d0 + d1 + d2),
                         t1 = d1 * (+d0 - d1 + d2), t2 = d2 * (+d0 + d1 - d2), t3 = t0 + t1 + t2;

        return t0 / t3 * p0 + t1 / t3 * p1 + t2 / t3 * p2;
    }

    Vec2 innerCenter() const {
        const value_type dist[3] = {side(0).length(), side(1).length(), side(2).length()};

        return (dist[0] * p0 + dist[1] * p1 + dist[2] * p2) / (dist[0] + dist[1] + dist[2]);
    }

    friend istream& operator>>(istream& is, Triangle& rhs) { return is >> rhs.p0 >> rhs.p1 >> rhs.p2; }
};

struct Circle {
    using value_type = Vec2::value_type;
    Vec2 center;
    value_type r;

    Circle() = default;
    explicit constexpr Circle(value_type r) : center(0.0, 0.0), r(r) {}
    constexpr Circle(value_type x, value_type y, value_type r) : center(x, y), r(r) {}
    constexpr Circle(const Vec2& center, value_type r) : center(center), r(r) {}

    constexpr bool intersects(const Line& line) const { return sgn(line.distanceFromSq(center) - r * r) <= 0; }

    constexpr bool intersects(const Circle& circle) const {
        const value_type d = center.distanceFromSq(circle.center);

        return sgn(square(r - circle.r) - d) <= 0 && sgn(d - square(r + circle.r)) <= 0;
    }

    constexpr bool circumcribes(const Circle& circle) const { return same(center.distanceFromSq(circle.center), square(r + circle.r)); }

    constexpr bool inscribes(const Circle& circle) const { return same(center.distanceFromSq(circle.center), square(r - circle.r)); }

    constexpr bool contains(const Circle& circle) const {
        return 0 < sgn(square(r - circle.r) - center.distanceFromSq(circle.center)) && 0 < sgn(r - circle.r);
    }

    constexpr value_type area() const { return Pi * r * r; }

    value_type commonArea(const Circle& circle) const {
        const value_type d_sq = center.distanceFromSq(circle.center);
        const value_type d = center.distanceFrom(circle.center);

        const value_type r1 = r;
        const value_type r2 = circle.r;

        if (sgn(d_sq - square(r1 + r2)) >= 0) return 0.0;
        if (sgn(d_sq - square(r1 - r2)) <= 0) return min(this->area(), circle.area());

        const value_type arg1 = acos(((r1 - r2) * (r1 + r2) + d_sq) / (2 * r1 * d));
        const value_type arg2 = acos(((r2 - r1) * (r2 + r1) + d_sq) / (2 * r2 * d));

        return r1 * r1 * (arg1 - sin(2 * arg1) / 2) + r2 * r2 * (arg2 - sin(2 * arg2) / 2);
    }

    constexpr Circle& moveBy(const Vec2& v) {
        center += v;
        return *this;
    }
    constexpr Circle movedBy(const Vec2& v) const { return {center + v, r}; }

    optional<vector<Vec2>> intersectsAt(const Line& line) const {
        if (!intersects(line)) return nullopt;

        vector<Vec2> res;

        const Vec2 v = line.projection(center);

        if (same(line.distanceFrom(center), r))
            res.emplace_back(v);
        else {
            value_type d = sqrt(r * r - (v - center).lengthSq());
            const Vec2 e = line.vector().normalized();

            res.emplace_back(v - d * e);
            res.emplace_back(v + d * e);
        }

        return res;
    }

    optional<vector<Vec2>> intersectsAt(const Circle& circle) const {
        if (!intersects(circle)) return nullopt;

        const value_type d = center.distanceFrom(circle.center), arg = (circle.center - center).arg(),
                         theta = acos((d * d + r * r - circle.r * circle.r) / (2 * d * r));

        vector<Vec2> res;
        res.emplace_back(center + r * Vec2(arg + theta));
        res.emplace_back(center + r * Vec2(arg - theta));

        if (res[0] > res[1]) swap(res[0], res[1]);

        return res;
    }

    optional<vector<Vec2>> tangentPointFrom(const Vec2& v) const {
        const value_type d = center.distanceFromSq(v);

        if (sgn(d - r * r) < 0) return nullopt;
        if (same(d, r * r)) return vector<Vec2>{v};

        const auto [x1, y1] = v - center;

        const auto& opt = Circle(r).intersectsAt(Line(x1, y1, -r * r));

        vector<Vec2> res = opt.value();

        for (Vec2& v : res) v += center;

        if (res[0] > res[1]) swap(res[0], res[1]);

        return res;
    }
    optional<vector<Vec2>> commonTangentPointFrom(const Circle& circle) const {
        const value_type r1 = r, r2 = circle.r;

        vector<Vec2> res;

        if (!same(r1, r2)) {
            const Vec2 p((r2 * center - r1 * circle.center) / (r2 - r1));

            const auto& opt = tangentPointFrom(p);

            if (opt.has_value()) {
                for (const Vec2& vec : opt.value()) {
                    res.emplace_back(vec);
                }
            }
        } else {
            const value_type theta = (circle.center - center).arg();

            res.emplace_back(center + r1 * Vec2(theta + 90_deg));
            res.emplace_back(center + r1 * Vec2(theta - 90_deg));
        }

        const Vec2 q((r2 * center + r1 * circle.center) / (r1 + r2));

        const auto& opt = tangentPointFrom(q);

        if (opt.has_value()) {
            for (const Vec2& vec : opt.value()) {
                res.emplace_back(vec);
            }
        }

        if (res.empty()) return nullopt;

        sort(res.begin(), res.end());

        return res;
    }

    friend istream& operator>>(istream& is, Circle& rhs) { return is >> rhs.center >> rhs.r; }
};

struct Polygon {
    using value_type = Vec2::value_type;
    int n;
    vector<Vec2> vertex;

    Polygon() = default;
    Polygon(int _n) : n(_n), vertex(_n) {}
    Polygon(const vector<Vec2>& vec) : n(static_cast<int>(vec.size())), vertex(vec) {}

    Line side(size_t index) const { return Line(vertex[index], vertex[(index + 1) % n]); }
    Line edge(size_t index) const { return side(index); }

    Polygon& moveBy(const Vec2& v) {
        for (Vec2& vec : vertex) vec += v;
        return *this;
    }
    Polygon movedBy(const Vec2& v) const {
        vector<Vec2> new_vertex;
        for (const Vec2& vec : vertex) new_vertex.emplace_back(vec + v);

        return Polygon(new_vertex);
    }

    bool isConvex() const {
        for (int i = 0; i < n; ++i) {
            if (ccw(vertex[i], vertex[(i + 1) % n], vertex[(i + 2) % n]) == -1) return false;
        }
        return true;
    }

    bool isOnEdge(const Vec2& v) const {
        for (int i = 0; i < n; ++i) {
            if (ccw(vertex[i], vertex[(i + 1) % n], v) == 0) return true;
        }
        return false;
    }

    bool intersects(const Vec2& v) const {
        floating_point_type sum = 0.0;

        for (int i = 0; i < n; ++i) {
            sum += v.arg(vertex[i], vertex[(i + 1) % n]);
        }

        return sgn(sum - 2.0 * Pi) == 0;
    }

    double area() const {
        value_type sum = 0.0;

        for (int i = 0; i < n; ++i) {
            sum += vertex[i].cross(vertex[i == n - 1 ? 0 : i + 1]);
        }

        return 0.5 * fabs(sum);
    }

    double perimeter() const {
        double len = 0.0;
        for (int i = 0; i < n; ++i) {
            len += side(i).length();
        }
        return len;
    }

    value_type commonArea(const Circle& circle) const {
        if (n < 3) return 0.0;

        if (!same(circle.center.x, 0.0) || !same(circle.center.y, 0.0)) {
            return (*this).movedBy(-circle.center).commonArea(Circle(circle.r));
        }

        auto cross_area = [&](auto self, Circle c, Vec2 a, Vec2 b) -> value_type {
            const Vec2 va = c.center - a, vb = c.center - b;

            value_type f = va.cross(vb), res = 0.0;

            if (same(f, 0.0)) return 0.0;
            if (sgn(max(va.length(), vb.length()) - c.r) <= 0) return f;

            const Line line(a, b);

            if (sgn(segment::distance(line, c.center) - c.r) >= 0) {
                return c.r * c.r * va.arg(vb);
            }

            const auto opt = c.intersectsAt(line);
            vector<Vec2> vec1 = opt.value();

            const vector<Vec2> vec2{a, vec1.front(), vec1.back(), b};

            for (int i = 0, s = vec2.size(); i + 1 < s; ++i) {
                res += self(self, c, vec2[i], vec2[i + 1]);
            }

            return res;
        };

        value_type sum = 0.0;

        for (int i = 0; i < n; ++i) {
            sum += cross_area(cross_area, circle, vertex[i], vertex[(i + 1) % n]);
        }

        sum *= 0.5;

        return fabs(sum);
    }

    Polygon convexHull() const { return convex_hull(vertex); }

    Polygon convexCut(const Line& line) const {
        assert(isConvex());

        vector<Vec2> res;

        for (int i = 0; i < n; ++i) {
            const Vec2 &now = vertex[i], &nxt = vertex[(i + 1) % n];

            if (ccw(line.begin, line.end, now) != -1) res.emplace_back(now);
            if (ccw(line.begin, line.end, now) * ccw(line.begin, line.end, nxt) < 0) {
                const auto& opt = Line(now, nxt).intersectsAt(line);
                if (opt.has_value()) res.emplace_back(opt.value());
            }
        }

        return Polygon(res);
    }

    value_type diameter() const {
        const auto convex = (isConvex() ? (*this) : convexHull()).vertex;

        const size_t n = convex.size();

        if (n == 2)
            return convex[0].distanceFrom(convex[1]);
        else {
            size_t i = 0, j = 0;

            for (size_t k = 0; k < n; ++k) {
                if (!(convex[i] < convex[k])) i = k;
                if (convex[j] < convex[k]) j = k;
            }

            value_type res = 0.0;

            const size_t si = i, sj = j;

            while (i != sj || j != si) {
                res = max(res, convex[i].distanceFrom(convex[j]));

                if ((convex[(i + 1) % n] - convex[i]).cross(convex[(j + 1) % n] - convex[j]) < 0) {
                    i = (i + 1) % n;
                } else
                    j = (j + 1) % n;
            }

            return res;
        }
    }

    friend istream& operator>>(istream& is, Polygon& rhs) {
        for (Vec2& e : rhs.vertex) is >> e;
        return is;
    }
};
}  // namespace geometry

// 3D
namespace geometry {
template <typename T>
struct Vector3D {
    using value_type = T;
    T x, y, z;

    Vector3D() = default;
    constexpr Vector3D(T x, T y, T z) noexcept : x(x), y(y), z(z) {}

    constexpr bool operator<(const Vector3D& v) const noexcept {
        if (!same(x, v.x)) return x < v.x;
        if (!same(y, v.y)) return y < v.y;
        return z < v.z;
    }
    constexpr bool operator>(const Vector3D& v) const noexcept { return v < *this; }
    constexpr bool operator==(const Vector3D& v) const noexcept { return same(x, v.x) && same(y, v.y) && same(z, v.z); }
    constexpr bool operator!=(const Vector3D& v) const noexcept { return !(*this == v); }

    constexpr Vector3D operator+() const noexcept { return *this; }
    constexpr Vector3D operator-() const noexcept { return {-x, -y, -z}; }

    constexpr Vector3D operator+(const Vector3D& v) const noexcept { return {x + v.x, y + v.y, z + v.z}; }
    constexpr Vector3D operator-(const Vector3D& v) const noexcept { return {x - v.x, y - v.y, z - v.z}; }
    constexpr Vector3D operator*(T s) const noexcept { return {x * s, y * s, z * s}; }
    constexpr Vector3D operator/(T s) const noexcept { return {x / s, y / s, z / s}; }

    constexpr Vector3D& operator+=(const Vector3D& v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    constexpr Vector3D& operator-=(const Vector3D& v) noexcept {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    constexpr Vector3D& operator*=(T s) noexcept {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    constexpr Vector3D& operator/=(T s) noexcept {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    constexpr bool isZero() const noexcept { return x == T(0) && y == T(0) && z == T(0); }
    bool hasNan() const { return isnan(x) || isnan(y) || isnan(z); }

    constexpr T dot(const Vector3D& v) const noexcept { return x * v.x + y * v.y + z * v.z; }
    constexpr Vector3D cross(const Vector3D& v) const noexcept { return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x}; }

    constexpr T lengthSq() const noexcept { return dot(*this); }
    floating_point_type length() const { return std::sqrt(lengthSq()); }

    floating_point_type distanceFrom(const Vector3D& v) const { return (v - *this).length(); }
    constexpr T distanceFromSq(const Vector3D& v) const noexcept { return (v - *this).lengthSq(); }

    Vector3D normalized() const { return *this / length(); }
    Vector3D& normalize() { return *this /= length(); }

    constexpr friend Vector3D operator*(T s, const Vector3D& v) noexcept { return {s * v.x, s * v.y, s * v.z}; }
    friend istream& operator>>(istream& is, Vector3D& v) { return is >> v.x >> v.y >> v.z; }
    friend ostream& operator<<(ostream& os, const Vector3D& v) { return os << "(" << v.x << ", " << v.y << ", " << v.z << ")"; }
};
}  // namespace geometry

// AOJ CGL
namespace geometry {
// x座標でsort済み
floating_point_type distance_closest_pair(vector<Vec2>& points, int left, int right) {
    if (right - left <= 1) return 1e20;

    int mid = (left + right) / 2;
    floating_point_type x = points[mid].x;
    floating_point_type d = min(distance_closest_pair(points, left, mid), distance_closest_pair(points, mid, right));

    inplace_merge(points.begin() + left, points.begin() + mid, points.begin() + right,
                  [](const Vec2& a, const Vec2& b) { return sgn(a.y - b.y) < 0; });

    vector<Vec2> a;

    for (int i = left; i < right; ++i) {
        if (sgn(fabs(points[i].x - x) - d) >= 0) continue;

        for (int j = static_cast<int>(a.size()) - 1; j >= 0; --j) {
            if (sgn(fabs((points[i] - a[j]).y) - d) >= 0) break;
            d = min(d, points[i].distanceFrom(a[j]));
        }

        a.emplace_back(points[i]);
    }

    return d;
}

// 垂直または水平線分の交点
// begin < end
vector<Vec2> intersections(const vector<Line>& lines) {
    const int n = static_cast<int>(lines.size());

    vector<pair<floating_point_type, int>> events;

    for (int i = 0; i < n; ++i) {
        const Line& line = lines[i];

        if (same(line.begin.y, line.end.y)) {  // 水平
            events.emplace_back(make_pair(line.begin.x, i));
            events.emplace_back(make_pair(line.end.x, i + 2 * n));
        } else {  // 垂直
            events.emplace_back(make_pair(line.begin.x, i + n));
        }
    }

    sort(events.begin(), events.end());

    set<floating_point_type> y_list;
    vector<Vec2> res;

    for (int i = 0; i < static_cast<int>(events.size()); ++i) {
        const int kind = events[i].second;
        const int id = kind % n;
        const Line& line = lines[id];

        if (kind < n) {  // 左端
            y_list.insert(line.begin.y);
        } else if (kind < 2 * n) {  // 垂直
            for (auto y : y_list) {
                if (line.begin.y - EPS <= y && y <= line.end.y + EPS) {
                    res.emplace_back(Vec2(line.begin.x, y));
                }
            }
        } else {  // 右端
            y_list.erase(line.end.y);
        }
    }

    return res;
}
}  // namespace geometry