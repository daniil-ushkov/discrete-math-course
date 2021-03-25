#include "bits/stdc++.h"

#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)
#pragma GCC optimize("O3")

using namespace std;

int n, m;
vector<vector<bool>> g;
vector<int> empty_g_ans;


void read() {
    cin >> n >> m;
    empty_g_ans.assign(15, 0);
    empty_g_ans[n] = 1;
    g.assign(n, vector<bool>(n, false));
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        g[u][v] = g[v][u] = true;
    }
}

vector<int> chr_polynom(vector<vector<bool>> &cur_g) {
    pair<int, int> e;
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < u; ++v) {
            if (cur_g[u][v]) {
                e = {u, v};
                goto found;
            }
        }
    }
    return empty_g_ans;

   found:
    cur_g[e.first][e.second] = cur_g[e.second][e.first] = false;
    auto p1 = chr_polynom(cur_g);

    auto saved_fst = cur_g[e.first];
    auto saved_snd = cur_g[e.second];

    for (int u = 0; u < n; ++u) {
        cur_g[e.first][u] = cur_g[e.first][u] || cur_g[e.second][u];
        cur_g[e.second][u] = false;
    }
    for (int u = 0; u < n; ++u) {
        cur_g[u][e.first] = cur_g[u][e.first] || cur_g[u][e.second];
        cur_g[u][e.second] = false;
    }
    auto p2 = chr_polynom(cur_g);

    cur_g[e.first] = saved_fst;
    cur_g[e.second] = saved_snd;
    for (int u = 0; u < n; ++u) {
        cur_g[u][e.first] = saved_fst[u];
        cur_g[u][e.second] = saved_snd[u];
    }
    cur_g[e.first][e.second] = cur_g[e.second][e.first] = true;

    for (int j = 0; j < 14; ++j) {
        p1[j] -= p2[j + 1];
    }

    return p1;
}

int main() {
    read();
    auto ans = chr_polynom(g);
    int zeros = 14;
    while (ans[zeros] == 0) {
        --zeros;
    }
    cout << zeros << '\n';
    for (int i = zeros; i >= 0; --i) {
        cout << ans[i] << ' ';
    }
    return 0;
}
