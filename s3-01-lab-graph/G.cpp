#include "bits/stdc++.h"

#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)
#pragma GCC optimize("O3")

using namespace std;

int n, m, k = 0, w;
vector<vector<int>> g;
vector<int> color;

void read() {
    cin >> n >> m;
    g.assign(n, {});
    color.assign(n, -1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (auto &u : g) {
        k = max(k, (int) u.size());
    }
    for (int u = 0; u < n; ++u) {
        if (g[u].size() < k) {
            w = u;
            break;
        }
    }
    if (k % 2 == 0) {
        k++;
    }
}

void bfs(vector<int> &sorted) {
    vector<bool> used(n, false);
    queue<int> q;
    sorted.push_back(w);
    q.push(w);
    used[w] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (!used[v]) {
                sorted.push_back(v);
                used[v] = true;
                q.push(v);
            }
        }
    }
    reverse(sorted.begin(), sorted.end());
}

void solve() {
    vector<int> sorted;
    bfs(sorted);
    for (int u : sorted) {
        vector<bool> color_used(k, false);
        for (int v : g[u]) {
            if (color[v] != -1) {
                color_used[color[v]] = true;
            }
        }
        for (int i = 0; i < k; ++i) {
            if (!color_used[i]) {
                color[u] = i;
                break;
            }
        }
    }
}

void write() {
    cout << k << '\n';
    for (int u = 0; u < n; ++u) {
        cout << color[u] + 1 << '\n';
    }
}

int main() {
    read();
    solve();
    write();
    return 0;
}