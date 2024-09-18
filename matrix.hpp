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
};