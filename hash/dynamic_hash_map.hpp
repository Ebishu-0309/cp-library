template <typename T>
class DynamicHashMap {
    using u32 = unsigned int;
    using u64 = unsigned long long;

    u32 LG = 0, N = 1;
    vector<u64> key;
    vector<T> val;
    vector<bool> vis, has;
    u32 siz = 0, load = 0;

   public:
    T &operator[](u64 k) {
        if (load + 1 == N) {
            relocate(LG + 1);
        }
        u32 i = index(k);
        if (!vis[i]) {
            vis[i] = true;
            key[i] = k;
            val[i] = T{};
            siz++;
            load++;
        }
        return val[i];
    }

    bool contains(u64 k) const { return contains_inner(index(k)); }

    int size() const { return static_cast<int>(siz); }
    bool empty() const { return siz == 0; }

    void reserve(u32 n) {
        u32 new_LG = LG;
        while ((1ul << new_LG) < n) {
            new_LG++;
        }
        relocate(new_LG);
    }

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

    bool contains_inner(u32 i) const { return vis[i]; }

    void relocate(u32 new_LG) {
        vector<pair<u64, T>> pair_old;
        pair_old.reserve(siz);
        for (auto e : key) {
            const u32 i = index(e);
            if (contains_inner(i)) {
                pair_old.emplace_back(e, val[i]);
            }
        }

        LG = new_LG;
        N = 1 << LG;

        key.reserve(N);
        val.reserve(N);
        vis.reserve(N);
        has.reserve(N);

        key.assign(N, 0);
        val.assign(N, 0);
        vis.assign(N, false);
        has.assign(N, false);

        load = 0;
        siz = 0;
        for (auto [k, v] : pair_old) {
            this->operator[](k) = v;
        }
    }
};