// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/line_add_get_min

#include "../template.hpp"

#include "../dynamic_lichaotree.hpp"

int main() {
    int n, q;
    cin >> n >> q;

    DynamicLiChaoTree<lint> cht(-1000'000'010LL, 1000'000'010LL, Inf);

    while (n--) {
        lint a, b;
        cin >> a >> b;
        cht.add_line(a, b);
    }
    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            lint a, b;
            cin >> a >> b;
            cht.add_line(a, b);
        } else {
            lint p;
            cin >> p;
            cout << cht.query(p) << "\n";
        }
    }
}