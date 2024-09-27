---
title: Dynamic Li Chao Tree
documentation_of: ../dynamic_lichaotree.hpp
---

## コンストラクタ
```cpp
DynamicLiChaoTree(T x_min, T x_max, T y_max)
```
$x$ の範囲および $y$ の最大値を設定して空の Li Chao Tree を構築する。以降、$L=x_\mathrm{max}-x_\mathrm{min}$ とする。
### 計算量
* $O(1)$

## add_line
```cpp
void add_line(T a, T b)
```
直線 $y=ax+b$ を追加する。
### 計算量
* $O(\log L)$

## add_segment
```cpp
void add_segment(T a, T b, T x_l, T x_r)
```
線分 $y=ax+b\:(x\in[x_l,x_r])$ を追加する。
### 計算量
* $O(\log^2 L)$

## query
```cpp
T query(T x)
```
点 $x$ における最小値を返す。直線/線分が存在しないときは $y_\mathrm{max}$ を返す。
### 計算量
* $O(\log L)$