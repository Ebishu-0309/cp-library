#pragma once

namespace transform {
// F[i] = sum_{j | i} f[j]
template <typename T>
void div_zeta(vector<T>& f) {
    const int n = f.size();
    vector<bool> is_prime(n, true);
    for (int p = 2; p < n; ++p) {
        if (is_prime[p]) {
            for (int k = 1; k * p < n; ++k) {
                is_prime[k * p] = false;
                f[k * p] += f[k];
            }
        }
    }
}

// f[i] = sum_{j | i} mu(i/j) F[j]
template <typename T>
void div_mobius(vector<T>& F) {
    const int n = F.size();
    vector<bool> is_prime(n, true);
    for (int p = 2; p < n; ++p) {
        if (is_prime[p]) {
            for (int k = (n - 1) / p; k >= 1; --k) {
                is_prime[k * p] = false;
                F[k * p] -= F[k];
            }
        }
    }
}

// F[i] = sum_{i | j} f[j]
template <typename T>
void mul_zeta(vector<T>& f) {
    const int n = f.size();
    vector<bool> is_prime(n, true);
    for (int p = 2; p < n; ++p) {
        if (is_prime[p]) {
            for (int k = (n - 1) / p; k >= 1; --k) {
                is_prime[k * p] = false;
                f[k] += f[k * p];
            }
        }
    }
}

// f[i] = sum_{i | j} mu(j/i) F[j]
template <typename T>
void mul_mobius(vector<T>& F) {
    const int n = F.size();
    vector<bool> is_prime(n, true);
    for (int p = 2; p < n; ++p) {
        if (is_prime[p]) {
            for (int k = 1; k * p < n; ++k) {
                is_prime[k * p] = false;
                F[k] -= F[k * p];
            }
        }
    }
}

template <typename T>
void fzt_sup(vector<T>& f) {
    const int n = f.size();
    for (int i = 1; i < n; i <<= 1) {
        for (int j = 0; j < n; ++j) {
            if (~j & i) {
                f[j] += f[j | i];
            }
        }
    }
}
template <typename T>
void fzt_sub(vector<T>& f) {
    const int n = f.size();
    for (int i = 1; i < n; i <<= 1) {
        for (int j = 0; j < n; ++j) {
            if (~j & i) {
                f[j | i] += f[j];
            }
        }
    }
}
template <typename T>
void ifzt_sup(vector<T>& f) {
    const int n = f.size();
    for (int i = 1; i < n; i <<= 1) {
        for (int j = 0; j < n; ++j) {
            if (~j & i) {
                f[j] -= f[j | i];
            }
        }
    }
}
template <typename T>
void ifzt_sub(vector<T>& f) {
    const int n = f.size();
    for (int i = 1; i < n; i <<= 1) {
        for (int j = 0; j < n; ++j) {
            if (~j & i) {
                f[j | i] -= f[j];
            }
        }
    }
}
template <typename T>
void fwt(vector<T>& f) {
    const int n = f.size();
    for (int i = 1; i < n; i <<= 1) {
        for (int j = 0; j < n; ++j) {
            if (~j & i) {
                const T x = f[j], y = f[j | i];
                f[j] = x + y, f[j | i] = x - y;
            }
        }
    }
}
template <typename T>
void ifwt(vector<T>& f) {
    const int n = f.size();
    static const T inv2 = T(1) / 2;
    for (int i = 1; i < n; i <<= 1) {
        for (int j = 0; j < n; ++j) {
            if (~j & i) {
                const T x = f[j], y = f[j | i];
                f[j] = (x + y) * inv2, f[j | i] = (x - y) * inv2;
            }
        }
    }
}
}  // namespace transform