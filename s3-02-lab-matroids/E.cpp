#include <bits/stdc++.h>

//#define DEBUG

#ifndef DEBUG
#define FILE_NAME "cycles"
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

int n, m;
vector<int> w;
vector<uint32_t> cycles;

void read() {
    cin >> n >> m;
    w.assign(n, 0);
    for (int i = 0; i < n; ++i) {
        cin >> w[i];
    }
    for (int i = 0; i < m; ++i) {
        int cnt;
        cin >> cnt;
        uint32_t C = 0;
        for (int j = 0; j < cnt; ++j) {
            int el;
            cin >> el;
            C |= (1u << (el - 1));
        }
        cycles.push_back(C);
    }
}

bool contains_cycle(uint32_t B) {
    for (uint32_t C : cycles) {
        if ((B & C) == C) {
            return true;
        }
    }
    return false;
}

void solve() {
    vector<int> sorted_by_w(n);
    for (int i = 0; i < n; ++i) {
        sorted_by_w[i] = i;
    }
    sort(sorted_by_w.begin(), sorted_by_w.end(),
         [&](uint32_t lhs, uint32_t rhs) { return w[lhs] > w[rhs]; });
    uint32_t B = 0;
    int ans = 0;
    for (int i : sorted_by_w) {
        if (!contains_cycle(B | (1u << i))) {
            B |= (1u << i);
            ans += w[i];
        }
    }
    cout << ans << '\n';
}


int main() {
    init();
    read();
    solve();
    return 0;
}