---
title: Potential Union Find
documentation_of: ../potential_union_find.hpp
---

UnionFind の頂点にポテンシャル $U(i)$ が追加された版。
## コンストラクタ
```cpp
PotentialUnionFind(int n)
```
頂点数 `n` で作成。
### 計算量
* $O(n)$

## root
```cpp
int root(int i)
```
`i` が属する連結成分の代表元を返す。
### 計算量
* $O(\log n)$

## unite
```cpp
bool unite(int i, int j, S x)
```
`i` と `j` を繋ぐ。

$U(i)-U(j)=x$ という条件について、
- 既存の条件と矛盾しない場合、この条件を追加し、`true` を返す。
- 矛盾している場合、この条件を破棄し、`false` を返す。

## delta
```cpp
optional<S> delta(int i, int j)
```
`i` と `j` が連結な場合、$U(i)-U(j)$ を返す。連結でない場合、`nullopt` を返す。
### 計算量
* $O(\log n)$