// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/associative_array

#include "../template.hpp"

#include "../hash/hash_map.hpp"

int main() {
    HashMap<ull> mp;
    int q;
    cin >> q;
    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            ull k, v;
            cin >> k >> v;
            mp[k] = v;
        } else {
            ull k;
            cin >> k;
            cout << mp[k] << "\n";
        }
    }
}