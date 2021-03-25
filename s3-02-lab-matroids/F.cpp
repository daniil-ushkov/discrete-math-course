#include <bits/stdc++.h>

#define DEBUG

#ifndef DEBUG
#define FILE_NAME "rainbow"
#define FAST_IO std::ios_base::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0)
#pragma GCC optimize("O3")
#endif

void init() {
#ifndef DEBUG
    freopen(FILE_NAME ".in", "r", stdin);
    freopen(FILE_NAME ".out", "w", stdout);
    FAST_IO;
#endif
}

using namespace std;

struct edge {
    bool operator<(const edge &other) const {
        return id < other.id;
    }
    int from;
    int to;
    int color;
    int id;
};

struct rainbow_tree {
    rainbow_tree(int n, int m, vector<edge> &edges)
    : n(n),
      m(m),
      edges(edges),
      tree(n, set<edge>()),
      in_tree(m, false),
      color_cnt(102, 0) {
        for (int id = 0; id < m; ++id) {
            edges_out.insert(id);
        }
    }

    bool can_swap_in_graphic(int lid, int rid) {
        if (!(in_tree[lid] && !in_tree[rid])) {
            return false;
        }

        auto e1 = edges[lid];
        auto e2 = edges[rid];

        remove_edge(e1.id);
        add_edge(e2.id);
        vector<int> color(n, 0);
        bool cyclic = dfs(e2.from, -1, color);
        remove_edge(e2.id);
        add_edge(e1.id);
        return !cyclic;
    }

    bool can_swap_in_rainbow(int lid, int rid) {
        if (!(in_tree[lid] && !in_tree[rid])) {
            return false;
        }

        auto e1 = edges[lid];
        auto e2 = edges[rid];
        return color_cnt[e2.color] == 0 || (e1.color == e2.color);
    }

    void add_edge(int id) {
        auto e = edges[id];
        tree[e.from].insert(e);
        tree[e.to].insert({e.to, e.from, e.color, e.id});
        in_tree[id] = true;
        color_cnt[e.color]++;

        edges_in.insert(id);
        edges_out.erase(id);
    }

    void remove_edge(int id) {
        auto e = edges[id];
        tree[e.from].erase(e);
        tree[e.to].erase({e.to, e.from, e.color, e.id});
        in_tree[id] = false;
        color_cnt[e.color]--;

        edges_in.erase(id);
        edges_out.insert(id);
    }

    bool dfs(int u, int prev_id, vector<int> &color) {
        bool cyclic = false;
        color[u] = 1;
        for (const edge &e : tree[u]) {
            if (color[e.to] == 0) {
                cyclic |= dfs(e.to, e.id, color);
                if (cyclic) {
                    break;
                }
            } else if (color[e.to] == 1 && e.id != prev_id) {
                cyclic = true;
                break;
            }
        }
        color[u] = 2;
        return cyclic;
    }

    int n, m;
    vector<edge> edges;
    vector<set<edge>> tree;
    vector<bool> in_tree;
    vector<int> color_cnt;

    set<int> edges_in, edges_out;
};

int n, m, s, t;
rainbow_tree *rt;

void read() {
    cin >> n >> m;
    vector<edge> edges;
    for (int id = 0; id < m; ++id) {
        int u, v, c;
        cin >> u >> v >> c;
        u--, v--, c--;
        edges.push_back({u, v, c, id});
    }
    n += 4;
    m += 2;
    s = m - 2;
    t = m - 1;
    edges.push_back({n - 4, n - 3, 100, s});
    edges.push_back({n - 2, n - 1, 101, t});
    rt = new rainbow_tree(n, m, edges);
    rt->add_edge(s);
    rt->add_edge(t);
}

void solve() {
    while (true) {
        auto edges_out_copy1 = rt->edges_out;
        for (int rid : edges_out_copy1) {
            if (rt->can_swap_in_graphic(s, rid) && rt->can_swap_in_rainbow(s, rid)) {
                rt->add_edge(rid);
            }
        }

        vector<bool> used(m, false);
        queue<int> q;
        vector<int> prev(m, -1);
        used[s] = true;
        q.push(s);

        while (!q.empty()) {
            if (prev[t] != -1) {
                break;
            }
            int id = q.front();
            q.pop();
            if (rt->in_tree[id]) {
                auto edges_out_copy2 = rt->edges_out;
                for (int rid : edges_out_copy2) {
                    if (!used[rid] && rt->can_swap_in_graphic(id, rid)) {
                        used[rid] = true;
                        q.push(rid);
                        prev[rid] = id;
                    }
                }
            } else {
                auto edges_out_copy2 = rt->edges_in;
                for (int lid : edges_out_copy2) {
                    if (!used[lid] && rt->can_swap_in_rainbow(lid, id)) {
                        used[lid] = true;
                        q.push(lid);
                        prev[lid] = id;
                        if (lid == t) {
                            break;
                        }
                    }
                }
            }
        }

        if (prev[t] == -1) {
            break;
        }

        int cur = prev[t];
        while (prev[cur] != s) {
            rt->remove_edge(prev[cur]);
            rt->add_edge(cur);
        }
        rt->add_edge(cur);
    }
}

void write() {
    int k = 0;
    for (int id = 0; id < rt->m - 2; ++id) {
        if (rt->in_tree[id]) {
            k++;
        }
    }
    cout << k << '\n';
    for (int id = 0; id < rt->m - 2; ++id) {
        if (rt->in_tree[id]) {
            cout << id + 1 << ' ';
        }
    }
}

int main() {
    init();
    read();
    solve();
    write();
    return 0;
}

