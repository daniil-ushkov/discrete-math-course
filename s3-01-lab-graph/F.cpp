#include "bits/stdc++.h"

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> cnt(n, 0);
    vector<int> code(n - 2);
    for (int i = 0; i < n - 2; ++i) {
        cin >> code[i];
        cnt[--code[i]]++;
    }
    set<int> leaves;
    for (int u = 0; u < n; ++u) {
        if (cnt[u] == 0) {
            leaves.insert(u);
        }
    }
    for (int u : code) {
        int v = *leaves.begin();
        leaves.erase(v);
        cout << u + 1 << ' ' << v + 1 << '\n';
        if (--cnt[u] == 0) {
            leaves.insert(u);
        }
    }
    int v = *leaves.begin();
    cout << n << ' ' << v + 1 << '\n';
    return 0;
}