#pragma once

template <typename T, auto transform, auto inv_transform>
vector<T> generalized_convolution(vector<T> a, vector<T> b) {
    const int n = a.size();
    transform(a);
    transform(b);
    for (int i = 0; i < n; ++i) {
        a[i] *= b[i];
    }
    inv_transform(a);
    return a;
}