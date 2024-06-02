template <typename T, bool isMax>
struct ConvexHullTrickAddMonotone {
    struct Line {
        T a, b;
        constexpr Line(T _a, T _b) : a(_a), b(_b) {}
    };

    deque<Line> deq;

    // l.a <= n.a <= m.a && l.a < m.a
    static constexpr bool update(const Line &l, const Line &m, const Line &n) {
        if (l.a == n.a) {
            return l.b < n.b;
        }
        if (n.a == m.a) {
            return m.b < n.b;
        }
        return (m.b - n.b) / (m.a - n.a) < (n.b - l.b) / (n.a - l.a);
    }

    template <typename U = T>
    static constexpr U calc(const Line &line, U x) {
        return line.a * x + line.b;
    }

    ConvexHullTrickAddMonotone() = default;

    // add ax + b
    void add(T a, T b) {
        if constexpr (!isMax) a *= -1, b *= -1;

        Line line(a, b);
        if (deq.empty()) {
            deq.push_back(line);
            return;
        }
        if (line.a <= deq.front().a) {
            if (line.a == deq.front().a) {
                if (line.b <= deq.front().b) return;
                deq.pop_front();
            }
            while (deq.size() >= 2 && !update(line, deq[1], deq.front())) deq.pop_front();
            deq.push_front(line);
        } else {
            if (line.a == deq.back().a) {
                if (line.b <= deq.back().b) return;
                deq.pop_back();
            }
            while (deq.size() >= 2 && !update(deq.rbegin()[1], line, deq.back())) deq.pop_back();
            deq.push_back(line);
        }
    }

    template <typename U = T>
    U query(U x) {
        int l = 0, r = deq.size();
        while (l + 1 < r) {
            int m = (l + r) >> 1;
            if (calc(deq[m], x) >= calc(deq[m - 1], x))
                l = m;
            else
                r = m;
        }
        if constexpr (isMax) return calc(deq[l], x);
        return -calc(deq[l], x);
    }
};