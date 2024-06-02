struct Point {
    lint x, y;

    Point() = default;
    constexpr Point(lint x_, lint y_) : x(x_), y(y_) {}

    constexpr bool operator==(const Point &p) const { return x == p.x && y == p.y; }
    constexpr bool operator<(const Point &p) const {
        if (x != p.x) return x < p.x;
        return y < p.y;
    }

    constexpr Point operator+() const { return *this; }
    constexpr Point operator-() const { return {-x, -y}; }
    constexpr Point operator+(const Point &p) const { return {x + p.x, y + p.y}; }
    constexpr Point operator-(const Point &p) const { return {x - p.x, y - p.y}; }
    constexpr Point operator*(lint s) const { return {x * s, y * s}; }
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

    constexpr lint dot(const Point &p) const { return x * p.x + y * p.y; }
    constexpr lint cross(const Point &p) const { return x * p.y - y * p.x; }

    constexpr lint lengthSq() const { return x * x + y * y; }
    double length() const { return hypot(x, y); }
    constexpr lint distanceSq(const Point &p) const { return (*this - p).lengthSq(); }
    double distance(const Point &p) const { return (*this - p).length(); }

    friend constexpr Point operator*(lint s, const Point &p) { return p * s; }
    friend istream &operator>>(istream &is, Point &p) { return is >> p.x >> p.y; }
    friend ostream &operator<<(ostream &os, const Point &p) { return os << "(" << p.x << "," << p.y << ")"; }
};