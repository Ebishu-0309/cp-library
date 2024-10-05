---
title: 双対セグ木
documentation_of: ../dual_segment_tree.hpp
---

双対セグ木。
## コンストラクタ
```cpp
1. DualSegmentTree<F, composition, id>(int n)
2. DualSegmentTree<F, composition, id>(vector<F> f)
```
* 作用素の型 `F`
* 作用素の合成 $f\circ g$ を返す関数 `F composition(F f, F g)`
* 作用素の単位元を返す関数 `F id()`

を受け取り、
1. 長さ `n` 、初期値 `id()` で構築。
1. `f` で構築。

### 計算量
* $O(n)$

## get
```cpp
F get(int p)
```
`p` 番目の作用素を取得。
### 計算量
* $O(\log n)$

## apply
```cpp
void apply(int l, int r, F f)
```
$k\in[l, r)$ に対して、$k$ 番目の作用素に `f` を合成。
### 計算量
* $O(\log n)$