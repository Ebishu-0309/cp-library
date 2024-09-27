---
title: Binary Trie
documentation_of: ../binary_trie.hpp
---

非負整数を 2 進表記で管理する Trie 木。
## コンストラクタ
```cpp
BinaryTrie()
```
空の Binary Trie を作成。
### 計算量
* $O(1)$

## insert
```cpp
void insert(T x)
```
`x` を追加。
### 計算量
* $O(\log A)$

## erase
```cpp
void erase(T x)
```
`x` を削除。
### 計算量
* $O(\log A)$

## contains
```cpp
bool contains(T x) 
```
`x` が含まれているか判定。
### 計算量
* $O(\log A)$

## max_element, min_element
```cpp
T max_element()
T min_element()
```
集合の最大値/最小値を返す。
### 計算量
* $O(\log A)$

## apply_xor
```cpp
void apply_xor(T x)
```
集合の要素全てに xor を作用させる。
### 計算量
* $O(1)$