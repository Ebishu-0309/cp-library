struct Point {
    long long x, y;

    Point() = default;
    constexpr Point(long long x_, long long y_) : x(x_), y(y_) {}

    constexpr bool operator==(const Point &p) const { return x == p.x && y == p.y; }
    constexpr bool operator<(const Point &p) const {
        if (x != p.x) return x < p.x;
        return y < p.y;
    }

    constexpr Point operator+() const { return *this; }
    constexpr Point operator-() const { return {-x, -y}; }
    constexpr Point operator+(const Point &p) const { return {x + p.x, y + p.y}; }
    constexpr Point operator-(const Point &p) const { return {x - p.x, y - p.y}; }
    constexpr Point operator*(long long s) const { return {x * s, y * s}; }
    constexpr Point &operator+=(const Point &p) {
        x += p.x;
        y += p.y;
        return *this;
    }
    constexpr Point &operator-=(const Point &p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    constexpr long long dot(const Point &p) const { return x * p.x + y * p.y; }
    constexpr long long cross(const Point &p) const { return x * p.y - y * p.x; }

    constexpr long long lengthSq() const { return x * x + y * y; }
    double length() const { return hypot(x, y); }
    constexpr long long distanceSq(const Point &p) const { return (*this - p).lengthSq(); }
    double distance(const Point &p) const { return (*this - p).length(); }

    friend constexpr Point operator*(long long s, const Point &p) { return p * s; }
    friend istream &operator>>(istream &is, Point &p) { return is >> p.x >> p.y; }
    friend ostream &operator<<(ostream &os, const Point &p) { return os << "(" << p.x << "," << p.y << ")"; }
};