class BitVector {
    int b;
    vector<uint64_t> bit;
    vector<uint32_t> sum;

   public:
    BitVector() = default;
    BitVector(int n) : b((n + 63) >> 6) {
        bit.assign(b, 0ull);
        sum.assign(b, 0u);
    }

    void set(int p) { bit[p >> 6] |= 1ull << (p & 63); }
    bool operator[](int p) { return bit[p >> 6] >> (p & 63) & 1; }

    void build() {
        for (int i = 1; i < b; ++i) sum[i] = sum[i - 1] + __builtin_popcountll(bit[i - 1]);
    }

    int rank1(int p) { return sum[p >> 6] + __builtin_popcountll(bit[p >> 6] & ~(0xffffffffffffffffull << (p & 63))); }
    int rank0(int p) { return p - rank1(p); }
    int rank(int p, bool b) { return b ? rank1(p) : rank0(p); }
};