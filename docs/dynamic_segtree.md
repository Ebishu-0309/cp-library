---
title: 動的セグ木
documentation_of: ../dynamic_segtree.hpp
---

座圧せずに使えるセグ木。
## コンストラクタ
```cpp
DynamicSegtree<S, op, e>(long long lmin, long long rmax)
```
取りうる index の区間を設定し、空のセグ木を構築。以降、$L=\mathrm{rmax}-\mathrm{lmin}$ とする。
### 計算量
* $O(1)$

## set
```cpp
void set(long long p, S x)
```
`p` 番目の要素を `x` に設定。
### 計算量
* $O(\log L)$

## get
```cpp
S get(long long p)
```
`p` 番目の要素を取得。
### 計算量
* $O(\log L)$

## prod
```cpp
S prod(long long l, long long r)
```
区間 $[l, r)$ の総積を取得。
### 計算量
* $O(\log L)$

## max_right
```cpp
template <class F>
long long max_right(long long l, F f) 
```
以下の条件を両方満たす `r` を返す。
* `r = l` もしくは `f(op[l, r)) = true` 
* `r = rmax` もしくは `f(op[l, r + 1)) = false`

`f` が単調ならば、`f(op[l, r)) = true` となる最大の `r` と解釈できる。
### 計算量
* $O(\log L)$

## min_left
```cpp
template <class F>
long long min_left(long long r, F f)
```
以下の条件を両方満たす `l` を返す。
* `l = r` もしくは `f(op[l, r)) = true`
* `l = lmin` もしくは `f(op[l - 1, r)) = false`

`f` が単調ならば、`f(op[l, r)) = true` となる最小の `l` と解釈できる。
### 計算量
* $O(\log L)$