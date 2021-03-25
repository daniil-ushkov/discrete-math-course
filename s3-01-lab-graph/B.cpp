#include "bits/stdc++.h"

#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)
#pragma GCC optimize("O3")

using namespace std;

void reverse(deque<int> &q, int begin, int end) {
    for (int i = 0; i < (end - begin) / 2; ++i) {
        swap(q[begin + i], q[end - 1 - i]);
    }
}

int main() {
    fastio;
    int n;
    cin >> n;
    vector<vector<bool>> g(n, vector<bool>(n));
    for (int u = 1; u < n; ++u) {
        string line;
        cin >> line;
        for (int v = 0; v < u; ++v) {
            g[u][v] = g[v][u] = (line[v] == '1');
        }
    }
    deque<int> q;
    for (int i = 0; i < n; ++i) {
        q.push_back(i);
    }
    for (int i = 0; i < n * (n - 1); ++i) {
        if (!g[q[0]][q[1]]) {
            int j = 2;
            while (j + 1 < q.size() && !(g[q[0]][q[j]] && g[q[1]][q[j + 1]])) {
                j++;
            }
            if (j + 1 == q.size()) {
                j = 2;
                while (!g[q[0]][q[j]]) {
                    j++;
                }
            }
            reverse(q, 1, j + 1);
        }
        q.push_back(q.front());
        q.pop_front();
    }
    for (int u : q) {
        cout << u + 1 << ' ';
    }
    return 0;
}