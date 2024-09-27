// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/set_xor_min

#include "../template.hpp"

#include "../binary_trie.hpp"

int main() {
    BinaryTrie<int> trie;

    int q;
    cin >> q;
    while (q--) {
        int c, x;
        cin >> c >> x;
        if (c == 0)
            trie.insert(x);
        else if (c == 1)
            trie.erase(x);
        else {
            trie.apply_xor(x);
            cout << trie.min_element() << "\n";
            trie.apply_xor(x);
        }
    }
}