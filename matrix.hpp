template <typename T>
struct Matrix : public vector<vector<T>> {
    using Base = vector<vector<T>>;
    using Base::front;
    using Base::size;
    using Base::vector;

    Matrix(int h, int w) : Matrix(h, vector<T>(w)) {}

    int sizeH() const { return size(); }
    int sizeW() const { return front().size(); }

    Matrix operator+() const { return *this; }
    Matrix operator-() const {
        Matrix res(*this);
        for (auto &e : res) e = -e;
        return res;
    }
    Matrix operator*(T k) const { return Mat(*this) *= k; }
    Matrix operator/(T k) const { return Mat(*this) /= k; }
    Matrix operator+(const Matrix &mat) const { return Mat(*this) += mat; }
    Matrix operator-(const Matrix &mat) const { return Mat(*this) -= mat; }

    Matrix operator*(const Matrix &mat) const {
        int n = sizeH(), m = sizeW(), l = mat.sizeW();
        Matrix res(n, l);
        for (int i = 0; i < n; ++i) {
            for (int k = 0; k < m; ++k) {
                for (int j = 0; j < l; ++j) {
                    res[i][j] += (*this)[i][k] * mat[k][j];
                }
            }
        }
        return res;
    }

    Matrix operator*=(T k) {
        for (auto &e : *this) e *= k;
        return *this;
    }
    Matrix operator/=(T k) {
        assert(k != 0);
        *this *= 1 / k;
        return *this;
    }

    Matrix &operator+=(const Matrix &mat) {
        int h = sizeH(), w = sizeW();
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                (*this)[i][j] += mat[i][j];
            }
        }
        return *this;
    }
    Matrix &operator-=(const Matrix &mat) { return *this += -mat; }
    Matrix &operator*=(const Matrix &mat) { return *this = (*this) * mat; }

    static Matrix identity(int n) {
        Matrix res(n, n);
        for (int i = 0; i < n; ++i) {
            res[i][i] = 1;
        }
        return res;
    }

    Matrix pow(long long k) const {
        Matrix res = identity(sizeH()), tmp = *this;
        while (k > 0) {
            if (k & 1) {
                res *= tmp;
            }
            tmp *= tmp;
            k >>= 1;
        }
        return res;
    }

    T det() const {
        Matrix mat(*this);
        int n = mat.sizeH();
        T prod = 1;
        for (int j0 = 0; j0 < n; ++j0) {
            bool found = false;
            for (int i0 = j0; i0 < n; ++i0) {
                if (mat[i0][j0] != 0) {
                    if (i0 != j0) {
                        swap(mat[i0], mat[j0]);
                        prod *= -1;
                    }
                    prod *= mat[j0][j0];
                    T inv = 1 / mat[j0][j0];
                    for (auto &e : mat[j0]) e *= inv;
                    for (int i1 = 0; i1 < n; ++i1) {
                        if (j0 != i1) {
                            T mul = mat[i1][j0];
                            for (int j1 = j0; j1 < n; ++j1) {
                                mat[i1][j1] -= mul * mat[j0][j1];
                            }
                        }
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                return 0;
            }
        }
        return prod;
    }

    optional<Matrix> inv() const {
        int n = (*this).sizeH();
        Matrix mat(n, 2 * n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                mat[i][j] = (*this)[i][j];
            }
            mat[i][i + n] = 1;
        }
        for (int j0 = 0; j0 < n; ++j0) {
            bool found = false;
            for (int i0 = j0; i0 < n; ++i0) {
                if (mat[i0][j0] != 0) {
                    if (i0 != j0) {
                        swap(mat[i0], mat[j0]);
                    }
                    T inv = 1 / mat[j0][j0];
                    for (auto &e : mat[j0]) e *= inv;
                    for (int i1 = 0; i1 < n; ++i1) {
                        if (j0 != i1) {
                            T mul = mat[i1][j0];
                            for (int j1 = j0; j1 < 2 * n; ++j1) {
                                mat[i1][j1] -= mul * mat[j0][j1];
                            }
                        }
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                return nullopt;
            }
        }
        Matrix res(n, n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                res[i][j] = mat[i][j + n];
            }
        }
        return res;
    }

    friend ostream &operator<<(ostream &os, const Matrix &mat) {
        for (auto itr1 = mat.begin(), end_itr1 = mat.end(); itr1 != end_itr1;) {
            const auto &v = *itr1;
            for (auto itr2 = v.begin(), end_itr2 = v.end(); itr2 != end_itr2;) {
                os << *itr2;
                if (++itr2 != end_itr2) os << " ";
            }
            if (++itr1 != end_itr1) os << "\n";
        }
        return os;
    }
};