#include <bits/stdc++.h>

//#define DEBUG

#ifndef DEBUG
#define FILE_NAME "matching"
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

int n;
vector<vector<int>> l;
vector<ll> w;
vector<int> r;

void read() {
    cin >> n;
    l.assign(n, {});
    w.assign(n, 0);
    r.assign(n, -1);
    for (int i = 0; i < n; ++i) {
        cin >> w[i];
    }
    for (int u = 0; u < n; ++u) {
        int m;
        cin >> m;
        for (int i = 0; i < m; ++i) {
            int v;
            cin >> v;
            --v;
            l[u].push_back(v);
        }
    }
}

bool dfs(int u, vector<bool> &used) {
    if (used[u]) {
        return false;
    }
    used[u] = true;
    for (int v : l[u]) {
        if (r[v] == -1 || dfs(r[v], used)) {
            r[v] = u;
            return true;
        }
    }
    return false;
}

void solve() {
    vector<int> sorted_vs(n);
    for (int i = 0; i < n; ++i) {
        sorted_vs[i] = i;
    }
    sort(sorted_vs.begin(), sorted_vs.end(), [&](int u, int v) { return w[u] * w[u] > w[v] * w[v]; });
    for (int u : sorted_vs) {
        vector<bool> used(n, false);
        dfs(u, used);
    }
    vector<int> ans(n, 0);
    for (int u = 0; u < n; ++u) {
        if (r[u] != -1) {
            ans[r[u]] = u + 1;
        }
    }
    for (int u : ans) {
        cout << u << ' ';
    }
}

int main() {
    init();
    read();
    solve();
    return 0;
}