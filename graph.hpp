#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

struct Edge {
    int to;
    long long cost;

    Edge() = default;
    constexpr Edge(int to, long long cost) noexcept : to(to), cost(cost) {}

    constexpr bool operator<(const Edge& rhs) const noexcept { return cost < rhs.cost; }
    constexpr bool operator>(const Edge& rhs) const noexcept { return cost > rhs.cost; }
};

class Graph : private vector<vector<Edge>> {
   public:
    using vector<vector<Edge>>::vector;
    using vector<vector<Edge>>::operator[];
    using vector<vector<Edge>>::operator=;
    using vector<vector<Edge>>::size;
    using vector<vector<Edge>>::resize;
    using vector<vector<Edge>>::clear;
    using vector<vector<Edge>>::push_back;
    using vector<vector<Edge>>::emplace_back;
    using vector<vector<Edge>>::begin;
    using vector<vector<Edge>>::end;
    using vector<vector<Edge>>::front;
    using vector<vector<Edge>>::back;

    static constexpr long long INF = 1'000'000'000'000'000'010LL;

   private:
    vector<pair<int, int>> edges;
    vector<tuple<int, int, long long>> weighted_edges;

    bool is_weighted = false;

    template <typename T>
    bool chmin(T& a, T b) const {
        return a > b ? a = b, true : false;
    }
    template <typename T>
    bool chmax(T& a, T b) const {
        return a < b ? a = b, true : false;
    }

   public:
    void read(int num_edges = -1, bool weighted = false, bool directed = false, int idx = 1) {
        if (num_edges == -1) num_edges = size() - 1;

        is_weighted |= weighted;

        while (num_edges--) {
            int from, to;
            long long cost = 1;

            cin >> from >> to;
            if (weighted) cin >> cost;

            from -= idx;
            to -= idx;

            operator[](from).emplace_back(to, cost);

            edges.emplace_back(from, to);
            weighted_edges.emplace_back(from, to, cost);

            if (!directed) {
                operator[](to).emplace_back(from, cost);
            }
        }
    }

    void add_edge(int from, int to, long long cost = -1, bool directed = false, int idx = 0) {
        if (cost != -1)
            is_weighted = true;
        else
            cost = 1;

        from -= idx;
        to -= idx;

        operator[](from).emplace_back(to, cost);

        edges.emplace_back(from, to);
        weighted_edges.emplace_back(from, to, cost);

        if (!directed) {
            operator[](to).emplace_back(from, cost);
        }
    }

    vector<long long> dijkstra(int start) const {
        vector<long long> dist(size(), INF);

        if (is_weighted) {
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> que;

            dist[start] = 0;
            que.push({0, start});

            while (!que.empty()) {
                auto p = que.top();
                que.pop();
                int v = p.second;

                if (dist[v] < p.first) continue;

                for (const Edge& edge : operator[](v)) {
                    if (chmin(dist[edge.to], dist[v] + edge.cost)) {
                        que.push({dist[edge.to], edge.to});
                    }
                }
            }
        } else {
            queue<int> que;

            dist[start] = 0;
            que.push(start);

            while (!que.empty()) {
                int v = que.front();
                que.pop();

                for (const Edge& edge : operator[](v)) {
                    if (dist[edge.to] != INF) continue;
                    dist[edge.to] = dist[v] + 1;

                    que.push(edge.to);
                }
            }
        }

        return dist;
    }

    vector<long long> bellman_ford(int start) const {
        size_t n = size();

        vector<long long> dist(n, INF);
        dist[start] = 0;

        for (size_t loop = 0; loop + 1 < n; ++loop) {
            for (size_t v = 0; v < n; ++v) {
                if (dist[v] == INF) continue;

                for (const Edge& edge : operator[](v)) {
                    chmin(dist[edge.to], dist[v] + edge.cost);
                }
            }
        }

        queue<int> que;
        vector<bool> check(n);

        for (size_t v = 0; v < n; ++v) {
            if (dist[v] == INF) continue;

            for (const Edge& edge : operator[](v)) {
                if (chmin(dist[edge.to], dist[v] + edge.cost)) {
                    if (!check[edge.to]) {
                        que.push(edge.to);
                        check[edge.to] = true;
                    }
                }
            }
        }

        while (!que.empty()) {
            int now = que.front();
            que.pop();

            for (const Edge& edge : (*this)[now]) {
                if (!check[edge.to]) {
                    check[edge.to] = true;
                    que.push(edge.to);
                }
            }
        }

        for (size_t i = 0; i < n; ++i) {
            if (check[i]) dist[i] = -INF;
        }

        return dist;
    }

    optional<vector<vector<long long>>> warshall_floyd() const {
        const size_t n = size();

        vector<vector<long long>> dist(n, vector<long long>(n, INF));

        for (size_t i = 0; i < n; ++i) dist[i][i] = 0;

        for (size_t i = 0; i < n; ++i) {
            for (const Edge& edge : operator[](i)) {
                chmin(dist[i][edge.to], edge.cost);
            }
        }

        for (size_t k = 0; k < n; ++k) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    if (dist[i][k] != INF && dist[k][j] != INF) {
                        chmin(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }

        bool has_negative_cycle = false;

        for (size_t i = 0; i < n; ++i) {
            if (dist[i][i] < 0) {
                has_negative_cycle = true;
                break;
            }
        }

        if (has_negative_cycle) return nullopt;

        return dist;
    }

    vector<int> topological_sort() const {
        int v = size();

        vector<int> indegree(v);

        for (const auto& [from, to] : edges) {
            ++indegree[to];
        }

        queue<int> que;

        for (int i = 0; i < v; ++i) {
            if (indegree[i] == 0) {
                que.push(i);
            }
        }

        vector<int> res;

        while (!que.empty()) {
            int p = que.front();
            que.pop();

            for (const Edge& edge : operator[](p)) {
                int u = edge.to;

                --indegree[u];
                if (indegree[u] == 0) que.push(u);
            }

            res.push_back(p);
        }

        return res;
    }

    long long prim() const {
        long long res = 0;

        priority_queue<Edge, vector<Edge>, greater<Edge>> que;

        for (const Edge& edge : operator[](0)) {
            que.push(edge);
        }

        vector<bool> used(size(), false);
        used[0] = true;

        while (!que.empty()) {
            Edge edge = que.top();
            que.pop();

            if (used[edge.to]) continue;
            used[edge.to] = true;

            res += edge.cost;

            for (const Edge& e : operator[](edge.to)) {
                que.push(e);
            }
        }

        return res;
    }

    // { articulation points, bridges }
    pair<vector<int>, vector<pair<int, int>>> low_link() const {
        int n = size();

        vector<bool> used(n, false);
        vector<int> ord(n), low(n), articulation_points;
        vector<pair<int, int>> bridges;

        function<int(int, int, int)> dfs = [&](int id, int k, int par) {
            used[id] = true;
            ord[id] = k++;
            low[id] = ord[id];

            bool is_articulation_point = false;
            int count = 0;

            for (const Edge& edge : operator[](id)) {
                if (!used[edge.to]) {
                    ++count;

                    k = dfs(edge.to, k, id);

                    chmin(low[id], low[edge.to]);

                    if (par != -1 && ord[id] <= low[edge.to]) {
                        is_articulation_point = true;
                    }
                    if (ord[id] < low[edge.to]) {
                        bridges.emplace_back(min(id, edge.to), max(id, edge.to));
                    }
                } else if (edge.to != par) {
                    chmin(low[id], ord[edge.to]);
                }
            }

            if (par == -1 && count >= 2) {
                is_articulation_point = true;
            }

            if (is_articulation_point) {
                articulation_points.push_back(id);
            }

            return k;
        };

        int k = 0;

        for (int i = 0; i < n; ++i) {
            if (!used[i]) {
                k = dfs(i, k, -1);
            }
        }

        return make_pair(articulation_points, bridges);
    }

    // { { u, v }, dist }
    pair<pair<int, int>, long long> diameter() const {
        int v1 = -1, v2 = -1;
        long long dist = -1;

        const int n = size();

        const vector<long long> dist1 = dijkstra(0);

        for (int i = 0; i < n; ++i) {
            if (chmax(dist, dist1[i])) {
                v1 = i;
            }
        }

        dist = -1;

        const vector<long long> dist2 = dijkstra(v1);

        for (int i = 0; i < n; ++i) {
            if (chmax(dist, dist2[i])) {
                v2 = i;
            }
        }

        return {{v1, v2}, dist};
    }
};

class LowestCommonAncestor {
   private:
    vector<vector<int>> par;
    vector<int> dist;

    void dfs(const Graph& g, int v, int p, int d) {
        par[0][v] = p;
        dist[v] = d;

        for (const Edge& edge : g[v]) {
            if (edge.to != p) dfs(g, edge.to, v, d + 1);
        }
    }

   public:
    LowestCommonAncestor(const Graph& g, int root = 0) { init(g, root); }

    void init(const Graph& g, int root = 0) {
        int V = g.size(), K = 1;

        while ((1 << K) < V) ++K;

        par.assign(K, vector<int>(V, -1));
        dist.assign(V, -1);

        dfs(g, root, -1, 0);

        for (int k = 0; k + 1 < K; ++k) {
            for (int v = 0; v < V; ++v) {
                if (par[k][v] < 0) {
                    par[k + 1][v] = -1;
                } else {
                    par[k + 1][v] = par[k][par[k][v]];
                }
            }
        }
    }

    int query(int u, int v) const {
        if (dist[u] < dist[v]) swap(u, v);

        int K = par.size();

        for (int k = 0; k < K; ++k) {
            if ((dist[u] - dist[v]) >> k & 1) {
                u = par[k][u];
            }
        }

        if (u == v) return u;

        for (int k = K - 1; k >= 0; --k) {
            if (par[k][u] != par[k][v]) {
                u = par[k][u];
                v = par[k][v];
            }
        }

        return par[0][u];
    }

    int parent(int x, int d) const {
        int K = par.size();

        for (int k = 0; k < K; ++k) {
            if (d >> k & 1) {
                x = par[k][x];
            }
        }

        return x;
    }

    int distance(int u, int v) const { return dist[u] + dist[v] - 2 * dist[query(u, v)]; }
};