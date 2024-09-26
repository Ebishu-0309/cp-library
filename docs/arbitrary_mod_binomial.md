---
title: 任意mod二項係数
documentation_of: ../arbitrary_mod_binomial.hpp
---

任意 mod で二項係数を計算。
## コンストラクタ
```cpp
ArbitraryModBinomial(int m) 
```
mod を設定。
### 計算量
* $O(m \log m)$

## calc
```cpp
long long calc(long long n, long long r)
```
二項係数 $\binom{n}{r} \bmod m$ を計算。
### 計算量
* $m$ の素因数の種類数を $\omega$ として、$O(\omega(\log m + \log n))$