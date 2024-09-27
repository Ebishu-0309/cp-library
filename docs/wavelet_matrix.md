---
title: Wavelet Matrix
documentation_of: ../wavelet_matrix.hpp
---

数列が事前に与えられているときにいろいろ出来る
## コンストラクタ
```cpp
CompressedWaveletMatrix(vector<T> v)
```
数列 `v` で構築する。
### 計算量
* $O(N\log A)$

## access
```cpp
T access(int p)
T operator[](int p)
```
`p` 番目の要素を返す。
### 計算量
* $O(\log A)$

## count
```cpp
int count(T x, int l, int r)
```
区間 $[l,r)$ に含まれる `x` の個数を返す。
### 計算量
* $O(\log A)$

## kth_smallest
```cpp
T kth_smallest(int l, int r, int k)
```
区間 $[l, r)$ に含まれる要素のうち `k` 番目 (0-indexed) に小さい要素を返す。
### 計算量
* $O(\log A)$

## range_freq
```cpp
int range_freq(int l, int r, T lower, T upper)
```
区間 $[l, r)$ に含まれる要素 $x$ のうち $x\in[lower, upper)$ である個数を返す。
### 計算量
* $O(\log A)$
