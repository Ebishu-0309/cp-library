class DynamicHashSet {
    using u32 = unsigned int;
    using u64 = unsigned long long;

    u32 LG = 0, N = 1;
    vector<u64> key;
    vector<bool> vis, has;
    u32 siz = 0, load = 0;

   public:
    void insert(u64 x) {
        if (load + 1 == N) {
            relocate(LG + 1);
        }
        const u32 i = index(x);
        if (!vis[i]) {
            vis[i] = true;
            has[i] = true;
            key[i] = x;
            siz++;
            load++;
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

    void relocate(u32 new_LG) {
        vector<u64> key_old;
        key_old.reserve(siz);
        for (auto e : key) {
            if (contains(e)) {
                key_old.emplace_back(e);
            }
        }

        LG = new_LG;
        N = 1 << LG;

        key.reserve(N);
        vis.reserve(N);
        has.reserve(N);
        key.assign(N, 0);
        vis.assign(N, false);
        has.assign(N, false);
        load = 0;
        siz = 0;
        for (auto e : key_old) {
            insert(e);
        }
    }
};