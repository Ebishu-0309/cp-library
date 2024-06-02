template <typename T, auto x_min, auto x_max, auto y_max>
class DynamicLiChaoTree {
    static_assert(std::is_convertible_v<decltype(x_min), std::function<T()>>, "x_min must work as T()");
    static_assert(std::is_convertible_v<decltype(x_max), std::function<T()>>, "x_max must work as T()");
    static_assert(std::is_convertible_v<decltype(y_max), std::function<T()>>, "y_max must work as T()");

   public:
    DynamicLiChaoTree() : root(nullptr) {}

    //~DynamicLiChaoTree() { del(root); }

    // add ax + b
    void add_line(T a, T b) {
        Line line(a, b);
        add_line(root, x_min(), x_max(), line.calc(x_min()), line.calc(x_max()), line);
    }

    // add ax + b [x_l, x_r]
    void add_segment(T a, T b, T x_l, T x_r) {
        Line line(a, b);
        add_segment(root, x_l, x_r, x_min(), x_max(), line);
    }

    // min at x
    T query(T x) { return query(root, x_min(), x_max(), x); }

   private:
    struct Line {
        T a, b;
        constexpr Line(T a_, T b_) noexcept : a(a_), b(b_) {}
        constexpr T calc(T x) const noexcept { return a * x + b; }
    };
    struct Node;
    using Node_t = Node *;
    struct Node {
        Line line;
        Node_t l, r;

        Node(Line line_) : line(line_), l(nullptr), r(nullptr) {}
        Node(T a, T b) : line(a, b), l(nullptr), r(nullptr) {}
    };
    Node_t root;

    static constexpr T add_eps() noexcept { return (is_integral_v<T> ? 1 : 0); }

    T get(Node_t t, T x) { return (t == nullptr ? y_max() : t->line.calc(x)); }

    void add_line(Node_t &t, T x_l, T x_r, T y_new_l, T y_new_r, Line new_line) {
        if (x_l > x_r) return;
        if (t == nullptr) {
            t = new Node(new_line);
            return;
        }

        const T y_l = t->line.calc(x_l), y_r = t->line.calc(x_r);

        if (y_l <= y_new_l && y_r <= y_new_r) return;
        if (y_new_l <= y_l && y_new_r <= y_r) {
            t->line = new_line;
            return;
        }

        T x_m = (x_l + x_r) / 2;
        if constexpr (is_integral_v<T>) {
            if (x_l + 1 == x_r) {
                t->l = new Node((y_l <= y_new_l) ? t->line : new_line);
                t->r = new Node((y_r <= y_new_r) ? t->line : new_line);
                return;
            }
        }

        T y_m = t->line.calc(x_m), y_new_m = new_line.calc(x_m);
        if (y_m <= y_new_m) {
            if (y_new_l <= y_l)
                add_line(t->l, x_l, x_m, y_new_l, y_new_m, new_line);
            else
                add_line(t->r, x_m + add_eps(), x_r, y_new_m + new_line.a * add_eps(), y_new_r, new_line);
        } else {
            swap(t->line, new_line);
            if (y_new_l <= y_l)
                add_line(t->r, x_m + add_eps(), x_r, y_m + new_line.a * add_eps(), y_r, new_line);
            else
                add_line(t->l, x_l, x_m, y_l, y_m, new_line);
        }
    }

    void add_line(Node_t &t, T x_l, T x_r, Line new_line) { add_line(t, x_l, x_r, new_line.calc(x_l), new_line.calc(x_r), new_line); }

    // query: [x_l, x_r], now: [x_a, x_b]
    void add_segment(Node_t &t, const T x_l, const T x_r, T x_a, T x_b, Line new_line) {
        if (x_a > x_b) return;
        if (x_b < x_l || x_r < x_a) return;
        if (x_l <= x_a && x_b <= x_r) {
            add_line(t, x_a, x_b, new_line);
            return;
        }

        if (t == nullptr) {
            t = new Node(0, y_max());
        }

        T x_c = (x_a + x_b) / 2;
        if constexpr (is_integral_v<T>) {
            if (x_a + 1 == x_b) {
                if (x_l <= x_a && x_a <= x_r) add_line(t->l, x_a, x_a, new_line);
                if (x_l <= x_b && x_b <= x_r) add_line(t->r, x_b, x_b, new_line);
                return;
            }
        }
        add_segment(t->l, x_l, x_r, x_a, x_c, new_line);
        add_segment(t->r, x_l, x_r, x_c + add_eps(), x_b, new_line);
    }

    T query(Node_t t, T x_l, T x_r, T x) {
        if (t == nullptr) return y_max();
        T y = t->line.calc(x);

        T x_m = (x_l + x_r) / 2;
        if constexpr (is_integral_v<T>) {
            if (x_l + 1 == x_r) {
                return min({y, (x == x_l ? get(t->l, x) : y_max()), (x == x_r ? get(t->r, x) : y_max())});
            }
        }

        if (x <= x_m) return min(y, query(t->l, x_l, x_m, x));
        return min(y, query(t->r, x_m + add_eps(), x_r, x));
    }

    void del(Node_t &t) {
        if (t == nullptr) return;
        del(t->l);
        del(t->r);
        delete t;
    }
};