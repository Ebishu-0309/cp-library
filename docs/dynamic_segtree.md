---
title: 動的セグ木
documentation_of: ../dynamic_segtree.hpp
---

座圧せずに使えるセグ木。
## コンストラクタ
```cpp
DynamicSegtree(long long lmin, long long rmax)
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