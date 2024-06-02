template <class S, auto op, auto e>
class DynamicSegtree {
    static_assert(std::is_convertible_v<decltype(op), std::function<S(S, S)>>, "op must work as S(S, S)");
    static_assert(std::is_convertible_v<decltype(e), std::function<S()>>, "e must work as S()");

   public:
    DynamicSegtree(int n_) : n(n_), root(nullptr) {}

    ~DynamicSegtree() { del(root); }

    void set(int p, S x) { set(root, 0, n, p, x); }
    S get(int p) { return get(root, 0, n, p); }
    S prod(int l, int r) { return prod(root, 0, n, l, r); }

   private:
    struct Node;
    using Node_t = Node *;
    struct Node {
        S val;
        Node_t l, r;

        Node(S v) : val(v), l(nullptr), r(nullptr) {}
    };

    S get(Node_t t) { return (t == nullptr ? e() : t->val); }

    int n;
    Node_t root;

    void set(Node_t &t, int l, int r, int p, S x) {
        if (t == nullptr) t = new Node(e());
        if (l + 1 == r) {
            t->val = x;
            return;
        }

        int m = (l + r) >> 1;  // [l, m) [m, r)
        if (p < m)
            set(t->l, l, m, p, x);
        else
            set(t->r, m, r, p, x);

        t->val = op(get(t->l), get(t->r));
    }

    S get(Node_t t, int l, int r, int p) {
        if (t == nullptr) return e();
        if (l + 1 == r) return t->val;
        int m = (l + r) >> 1;
        if (p < m) return get(t->l, l, m, p);
        return get(t->r, m, r, p);
    }

    // query: [l, r), now: [a, b)
    S prod(Node_t t, int a, int b, int l, int r) {
        if (t == nullptr || b <= l || r <= a) return e();
        if (l <= a && b <= r) return t->val;
        int c = (a + b) >> 1;
        return op(prod(t->l, a, c, l, r), prod(t->r, c, b, l, r));
    }

    void del(Node_t &t) {
        if (t == nullptr) return;
        del(t->l);
        del(t->r);
        delete t;
    }
};