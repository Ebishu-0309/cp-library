---
title: Sparse Table
documentation_of: ../sparse_table.hpp
---

結合法則・冪等性を満たす演算についての更新無し区間クエリを処理する。

## コンストラクタ
```cpp
SparseTable(const vector<S>& v)
```
数列 `v` で構築する。
### 計算量
* $O(N\log N)$

## prod
```cpp
S prod(int l, int r)
```
区間 $[l, r)$ の総積を返す。
### 計算量
* $O(1)$