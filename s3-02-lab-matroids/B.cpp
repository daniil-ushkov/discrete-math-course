#include <bits/stdc++.h>

//#define DEBUG

#ifndef DEBUG
#define FILE_NAME "destroy"
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

typedef long long ll;

class dsu {
public:
    explicit dsu(int size) : size_(size), parent_(size), rank_(size) {
        for (int i = 0; i < size_; i++) {
            parent_[i] = i;
            rank_[i] = 0;
        }
    }

    bool equal(int a, int b) {
        return find_leader_(a) == find_leader_(b);
    }

    void merge(int a, int b) {
        a = find_leader_(a);
        b = find_leader_(b);
        if (a == b) {
            return;
        }
        if (rank_[a] > rank_[b]) {
            swap(a, b);
        }
        parent_[a] = b;
        if (rank_[a] == rank_[b]) {
            rank_[b]++;
        }
    }

private:
    int find_leader_(int x) {
        if (parent_[x] == x) {
            return x;
        }
        return parent_[x] = find_leader_(parent_[x]);
    }

    int size_;
    vector<int> parent_;
    vector<int> rank_;
};

struct edge {
    int from;
    int to;
    ll w;
    int id;
};

int n, m;
ll s;
vector<edge> edges;

void read() {
    cin >> n >> m >> s;
    for (int i = 0; i < m; ++i) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        --u, --v;
        edges.push_back({u, v, w, i + 1});
    }
}

void solve() {
    sort(edges.begin(), edges.end(), [](const auto &el1, const auto &el2) { return el1.w > el2.w; });
    dsu dsu(n);
    vector<edge> useful;
    for (const edge &e : edges) {
        if (dsu.equal(e.from, e.to)) {
            useful.push_back(e);
        } else {
            dsu.merge(e.from, e.to);
        }
    }
    reverse(useful.begin(), useful.end());
    ll w = 0;
    vector<int> ans;
    for (const edge &e : useful) {
        if (w + e.w <= s) {
            w += e.w;
            ans.push_back(e.id);
        }
    }
    sort(ans.begin(), ans.end());
    cout << ans.size() << '\n';
    for (int id : ans) {
        cout << id << ' ';
    }
}

int main() {
    init();
    read();
    solve();
    return 0;
}
