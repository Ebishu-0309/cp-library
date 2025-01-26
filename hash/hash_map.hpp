template <typename T, int LG = 20>
class HashMap {
    using u32 = unsigned int;
    using u64 = unsigned long long;

    static constexpr u32 N = 1 << LG;
    array<u64, N> key;
    array<T, N> val;
    bitset<N> vis;
    int siz = 0;

   public:
    T &operator[](u64 k) {
        u32 i = index(k);
        if (!vis[i]) {
            vis[i] = true;
            key[i] = k;
            val[i] = T{};
            siz++;
        }
        return val[i];
    }

    bool contains(u64 k) const { return vis[index(k)]; }

    int size() const { return siz; }
    bool empty() const { return siz == 0; }

   private:
    u32 h(u64 x) const {
        static const u64 RAND0 = chrono::steady_clock::now().time_since_epoch().count();
        return ((x + RAND0) * 0xbf58476d1ce4e5b9) >> (64 - LG);
    }
    u32 index(u64 k) const {
        u32 i = h(k);
        while (vis[i] && key[i] != k) {
            i = (i + 1) & (N - 1);
        }
        return i;
    }
};