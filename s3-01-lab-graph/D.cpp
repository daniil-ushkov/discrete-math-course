#include "bits/stdc++.h"

#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)
//#pragma GCC optimize("O3")

using namespace std;

int n;
vector<vector<bool>> g;
list<int> path;

void assert_path() {
    for (auto it = ++path.begin(); it != path.end(); ++it) {
        auto it1 = it;
        --it1;
        assert(g[*it1][*it]);
    }
}

void assert_cycle() {
    assert_path();
    auto it = --path.end();
    assert(g[*it][*path.begin()]);
}

void read() {
    fastio;
    cin >> n;
    g.assign(n, vector<bool>(n));
    for (int u = 1; u < n; ++u) {
        string line;
        cin >> line;
        for (int v = 0; v < u; ++v) {
            g[u][v] = (line[v] == '1');
            g[v][u] = !g[u][v];
        }
    }
}

void find_path() {
    for (int u = 0; u < n; ++u) {
        auto it = path.begin();
        while (g[*it][u]) {
            ++it;
        }
        path.insert(it, u);
    }
    assert_path();
}

void rotate(_List_iterator<int> &itl) {
    auto it = itl;
    ++it;
    if (it != path.end()) {
        while (!g[*itl][*it]) {
            path.push_front(*itl);
            path.erase(itl);
            itl = it;
            --itl;
        }
    }
}

void find_cycle() {
    auto itl = path.begin();
    advance(itl, 2);
    while (!g[*itl][*path.begin()]) {
        ++itl;
    }
    ++itl;
    auto itr = itl;
    while (itr != path.end()) {
        auto it = path.begin();
        while (it != itl && !g[*itr][*it]) {
            ++it;
        }
        ++itr;
        if (it != itl) {
            path.insert(it, itl, itr);
            auto tmp = itl;
            --tmp;
            path.erase(itl, itr);
            rotate(tmp);
            ++tmp;
            itl = itr = tmp;
        }
    }
    assert_cycle();
}

int main() {
    read();
    if (n == 1) {
        cout << 1 << '\n';
        return 0;
    }
    find_path();
    find_cycle();
    for (int u : path) {
        cout << u + 1 << ' ';
    }
    return 0;
}
