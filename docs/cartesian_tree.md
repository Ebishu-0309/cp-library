---
title: Cartesian Tree
documentation_of: ../cartesian_tree.hpp
---

## cartesian_tree
```cpp
template <typename T, class Compare = less<>>
vector<int> cartesian_tree(vector<T> a, Compare comp = Compare{})
```
数列 `a` の Cartesian Tree を `comp` に従って求める。

返り値は各頂点の親である。根の場合は `-1` となる。