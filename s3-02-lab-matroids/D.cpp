#include <bits/stdc++.h>

//#define DEBUG

#ifndef DEBUG
#define FILE_NAME "check"
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

struct matroid {
    explicit matroid(uint32_t size) : size(size), ind(1u << size, false) {};

    void add_ind(vector<uint32_t> &set) {
        uint32_t mask = 0;
        for (uint32_t el : set) {
            mask |= 1u << el;
        }
        ind[mask] = true;
    }

    bool fst_prop() {
        return ind[0];
    }

    bool snd_prop() {
        for (uint32_t A = 0; A < ind.size(); ++A) {
            if (ind[A]) {
                for (uint32_t B = A; B != 0; B = (B - 1) & A) {
                    if (!ind[B]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool thd_prop() {
        for (uint32_t A = 0; A < ind.size(); ++A) {
            for (uint32_t B = 0; B < ind.size(); ++B) {
                if (ind[A] && ind[B] && __builtin_popcount(A) > __builtin_popcount(B)) {
                    uint32_t A_without_B = A & (~B);
                    bool exists = false;
                    for (uint32_t i = 0; i < size; ++i) {
                        if ((A_without_B & (1u << i)) != 0) {
                            exists |= ind[B | (1u << i)];
                        }
                    }
                    if (!exists) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool valid() {
        return fst_prop() && snd_prop() && thd_prop();
    }

    uint32_t size;
    vector<bool> ind;
};

int main() {
    init();
    uint32_t n, m;
    cin >> n >> m;
    matroid matroid(n);
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t cnt;
        cin >> cnt;
        vector<uint32_t> set(cnt);
        for (uint32_t j = 0; j < cnt; ++j) {
            cin >> set[j];
            --set[j];
        }
        matroid.add_ind(set);
    }
    cout << (matroid.valid() ? "YES\n" : "NO\n");
}