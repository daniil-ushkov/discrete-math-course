#include <bits/stdc++.h>

//#define DEBUG

#ifndef DEBUG
#define FILE_NAME "schedule"
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

template<typename T, typename INDEPENDENCY_CHECKER>
set<T> rado_edmonds(vector<T> X, INDEPENDENCY_CHECKER checker) {
    sort(X.begin(), X.end());
    set<T> B;
    for (const T &el : X) {
        if (checker(B, el)) {
            B.insert(el);
        }
    }
    return B;
}

struct task {
    bool operator<(const task &other) const {
        return w > other.w || (w == other.w && d < other.d);
    }
    ll d;
    ll w;
};

ll n;
vector<task> tasks;
set<ll> free_time;
ll sum_w = 0;

void read() {
    cin >> n;
    for (int i = 0; i < n; ++i) {
        ll d, w;
        cin >> d >> w;
        tasks.push_back({d, w});
        sum_w += w;
    }
    for (int i = 1; i <= n; ++i) {
        free_time.insert(i);
    }
}

int main() {
    init();
    read();
    ll opt_w = 0;
    auto checker = [](const set<task> &B, const task &el) {
        auto it = free_time.lower_bound(el.d);
        if (it != free_time.end() && *it == el.d) {
            free_time.erase(it);
            return true;
        }
        if (it == free_time.begin()) {
            return false;
        }
        free_time.erase(--it);
        return true;
    };
    auto optimized_set = rado_edmonds(tasks, checker);
    for (const task &el : optimized_set) {
        opt_w += el.w;
    }
    cout << sum_w - opt_w;
    return 0;
}
