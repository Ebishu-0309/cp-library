---
title: Area of Union of Rectangles
documentation_of: ../rectangle_union.hpp
---

長方形の和集合の面積を計算する。

平面走査により区間加算・区間 0 の個数クエリに帰着され、これは区間加算・区間最小値&個数クエリにすることで遅延セグ木が使える。

## rectangle_union
```cpp
long long rectangle_union(vector<int> l, vector<int> r, vector<int> d, vector<int> u)
```
$[l_i,r_i]\times[d_i,u_i]$ で表される長方形の和集合の面積を返す。
### 計算量
* $O(N\log N)$


## rectangle_union_small
```cpp
long long rectangle_union_small(vector<int> l, vector<int> r, vector<int> d, vector<int> u)
```
`rectangle_union` の座圧無し版。
### 計算量
* $O(N\log N)$