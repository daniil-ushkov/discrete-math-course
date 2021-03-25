#include "bits/stdc++.h"

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<vector<int>> tree(n);
    vector<int> deg(n, 0);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        deg[u - 1]++;
        deg[v - 1]++;
        tree[u - 1].push_back(v - 1);
        tree[v - 1].push_back(u - 1);
    }
    set<int> leaves;
    for (int u = 0; u < n; ++u) {
        if (deg[u] == 1) {
            leaves.insert(u);
        }
    }
    for (int i = 0; i < n - 2; ++i) {
        int u = *leaves.begin();
        leaves.erase(leaves.begin());
        deg[u]--;
        int v = -1;
        for (int w : tree[u]) {
            if (deg[w] != 0) {
                v = w;
            }
        }
        deg[v]--;
        if (deg[v] == 1) {
            leaves.insert(v);
        }
        cout << v + 1 << ' ';
    }
    return 0;
}