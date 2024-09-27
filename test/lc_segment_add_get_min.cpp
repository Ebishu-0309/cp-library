// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/segment_add_get_min

#include "../template.hpp"

#include "../dynamic_lichaotree.hpp"

int main() {
    int n, q;
    cin >> n >> q;

    DynamicLiChaoTree<lint> cht(-1000'000'010LL, 1000'000'010LL, Inf);

    while (n--) {
        lint l, r, a, b;
        cin >> l >> r >> a >> b;
        cht.add_segment(a, b, l, r - 1);
    }
    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            lint l, r, a, b;
            cin >> l >> r >> a >> b;
            cht.add_segment(a, b, l, r - 1);
        } else {
            lint p;
            cin >> p;
            lint y = cht.query(p);
            if (y == Inf)
                cout << "INFINITY\n";
            else
                cout << y << "\n";
        }
    }
}