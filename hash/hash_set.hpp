template <int LG = 20>
class HashSet {
    using u32 = unsigned int;
    using u64 = unsigned long long;

    static constexpr u32 N = 1 << LG;
    array<u64, N> key;
    bitset<N> vis, has;
    int siz = 0;

   public:
    void insert(u64 x) {
        const u32 i = index(x);
        if (!vis[i]) {
            vis[i] = true;
            has[i] = true;
            key[i] = x;
            siz++;
        }
    }

    void erase(u64 x) {
        const u32 i = index(x);
        if (has[i]) {
            has[i] = false;
            siz--;
        }
    }

    bool contains(u64 x) { return has[index(x)]; }

    int size() const { return siz; }
    bool empty() const { return siz == 0; }

   private:
    u32 h(u64 x) const {
        static const u64 RAND0 = chrono::steady_clock::now().time_since_epoch().count();
        return ((x + RAND0) * 0xbf58476d1ce4e5b9) >> (64 - LG);
    }
    u32 index(u64 x) const {
        u32 i = h(x);
        while (vis[i] && key[i] != x) {
            i = (i + 1) & (N - 1);
        }
        return i;
    }
};