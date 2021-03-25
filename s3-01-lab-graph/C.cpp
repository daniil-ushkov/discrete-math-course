#include "bits/stdc++.h"

using namespace std;

bool ask(int from, int to) {
    cout << 1 << ' ' << from + 1 << ' ' << to + 1 << '\n';
    cout.flush();
    string ans;
    cin >> ans;
    return ans == "YES";
}

int main() {
    int n;
    cin >> n;
    vector<int> path;
    for (int u = 0; u < n; ++u) {
        int l = -1;
        int r = path.size();
        while (r - l != 1) {
            int m = l + (r - l) / 2;
            if (ask(path[m], u)) {
                l = m;
            } else {
                r = m;
            }
        }
        path.insert(path.begin() + r, u);
    }
    cout << 0 << ' ';
    for (int u : path) {
        cout << u + 1 << ' ';
    }
    cout << '\n';
    return 0;
}
