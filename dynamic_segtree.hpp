template <class S, auto op, auto e>
class DynamicSegtree {
    static_assert(std::is_convertible_v<decltype(op), std::function<S(S, S)>>, "op must work as S(S, S)");
    static_assert(std::is_convertible_v<decltype(e), std::function<S()>>, "e must work as S()");

   public:
    DynamicSegtree(long long lmin, long long rmax) : lmin(lmin), rmax(rmax), root(nullptr) { assert(lmin <= rmax); }

    void set(long long p, S x) {
        assert(lmin <= p && p <= rmax);
        set(root, lmin, rmax, p, x);
    }

    S get(long long p) {
        assert(lmin <= p && p <= rmax);
        return get(root, lmin, rmax, p);
    }

    S prod(long long l, long long r) {
        assert(lmin <= l && l <= r && r <= rmax);
        return prod(root, lmin, rmax, l, r);
    }

    template <class F>
    long long max_right(long long l, F f) {
        assert(lmin <= l && l <= rmax);
        S sm = e();
        return max_right(root, lmin, rmax, l, f, sm);
    }

    template <class F>
    long long min_left(long long r, F f) {
        assert(lmin <= r && r <= rmax);
        S sm = e();
        return min_left(root, lmin, rmax, r, f, sm);
    }

   private:
    struct Node;
    using Node_t = unique_ptr<Node>;
    struct Node {
        S val;
        Node_t l, r;

        Node(S v) : val(v), l(nullptr), r(nullptr) {}
    };

    S get(const Node_t &t) { return (t == nullptr ? e() : t->val); }

    long long lmin, rmax;
    Node_t root;

    void set(Node_t &t, long long l, long long r, long long p, S x) {
        if (t == nullptr) t = make_unique<Node>(e());
        if (l + 1 == r) {
            t->val = x;
            return;
        }

        long long m = (l + r) >> 1;  // [l, m) [m, r)
        if (p < m)
            set(t->l, l, m, p, x);
        else
            set(t->r, m, r, p, x);

        t->val = op(get(t->l), get(t->r));
    }

    S get(const Node_t &t, long long l, long long r, long long p) {
        if (t == nullptr) return e();
        if (l + 1 == r) return t->val;
        long long m = (l + r) >> 1;
        if (p < m) return get(t->l, l, m, p);
        return get(t->r, m, r, p);
    }

    // query: [l, r), now: [a, b)
    S prod(const Node_t &t, long long a, long long b, long long l, long long r) {
        if (t == nullptr || b <= l || r <= a) return e();
        if (l <= a && b <= r) return t->val;
        long long c = (a + b) >> 1;
        return op(prod(t->l, a, c, l, r), prod(t->r, c, b, l, r));
    }

    // test: [a, b)
    // sm: f(sm) = true
    template <class F>
    long long max_right(const Node_t &t, long long a, long long b, long long l, F f, S &sm) {
        if (t == nullptr || b <= l) return rmax;
        if (l <= a && f(op(sm, t->val))) {
            sm = op(sm, t->val);
            return rmax;  // f(op[l, b)) = true
        }
        if (a + 1 == b) return a;  // f(op(sm, dat[a])) = false
        long long c = (a + b) >> 1;
        long long l_test = max_right(t->l, a, c, l, f, sm);
        if (l_test != rmax) return l_test;
        return max_right(t->r, c, b, l, f, sm);
    }

    // test: [a, b)
    // sm: f(sm) = true
    template <class F>
    long long min_left(const Node_t &t, long long a, long long b, long long r, F f, S &sm) {
        if (t == nullptr || r <= a) return lmin;
        if (b <= r && f(op(t->val, sm))) {
            sm = op(t->val, sm);
            return lmin;  // f(op[a, r)) = true
        }
        if (a + 1 == b) return b;  // f(op(dat[b - 1], sm)) = false
        long long c = (a + b) >> 1;
        long long r_test = min_left(t->r, c, b, r, f, sm);
        if (r_test != lmin) return r_test;
        return min_left(t->l, a, c, r, f, sm);
    }
};