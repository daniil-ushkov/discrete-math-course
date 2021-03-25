#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

#define FASTIO ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);

using namespace std;

#pragma GCC optimize("O3")
ifstream fin("problem3.in");
ofstream fout("problem3.out");
#define cin fin
#define cout fout

const uint64_t INF = 0xFFFFFFFF;
const uint64_t MOD = 1e9 + 7;

struct DFA {

  explicit DFA(size_t size)
      : start_(0),
        size_(size + 1),
        term_(size_),
        go_(size_, vector<size_t>(26, size_ - 1)) {

  }

  void set_term(size_t state) {
    term_[state] = true;
  }

  void link(size_t s1, size_t s2, char c) {
    go_[s1][c - 'a'] = s2;
  }


  int64_t count_words() {
    analyse_();
    if (!term_reachable_) {
      return 0;
    }
    if (inf_) {
      return -1;
    }
    calc_cnt_(max_depth_);

    return res;
  }

 private:
  //Base
  size_t start_;
  size_t size_;
  vector<vector<size_t>> go_;
  vector<bool> term_;

  uint64_t count(size_t s1, size_t s2) {
    uint64_t result = 0;
    for (char c = 'a'; c <= 'z'; ++c) {
      if (go_[s1][c - 'a'] == s2) {
        result++;
      }
    }
    return result;
  }

  //Dynamic for counting words
  uint64_t res = 0;
  vector<vector<uint64_t>> cnt_;
  void calc_cnt_(uint64_t length) {
    cnt_.assign(2, vector<uint64_t>(size_));
    cnt_[1][start_] = 1;
    if (term_[start_]) res = 1;
    for (size_t d = 0; d < length + 100; ++d) {
      cnt_[d % 2].assign(size_, 0);
      for (size_t state = 0; state < size_; ++state) {
        set<size_t> was;
        for (char c = 'a'; c <= 'z'; ++c) {
          auto next = go_[state][c - 'a'];
          if (find(was.begin(), was.end(), next) == was.end()) {
            was.insert(next);

            cnt_[d % 2][next] =
                (cnt_[d % 2][next] + ((uint64_t) count(state, next) * cnt_[(d + 1) % 2][state]) % MOD) % MOD;
            if (term_[next]) {
              res = (res + ((uint64_t) count(state, next) * cnt_[(d + 1) % 2][state]) % MOD) % MOD;
            }
          }
        }
      }
    }
  }

  //Checking infinity and emptiness of DFA language
  bool term_reachable_;
  bool inf_;

  uint64_t max_depth_;
  uint64_t depth_;

  vector<bool> valid;

  void analyse_() {
    term_reachable_ = false;
    inf_ = false;

    max_depth_ = 0;
    depth_ = 0;

    vector<vector<vector<size_t>>> go_rev(size_, vector<vector<size_t>>(26));
    for (size_t state = 0; state < size_; ++state) {
      for (char c = 'a'; c <= 'z'; ++c) {
        go_rev[go_[state][c - 'a']][c - 'a'].push_back(state);
      }
    }

    valid.assign(size_, false);
    dfs_(start_, valid);

    vector<bool> visited(size_);
    for (size_t state = 0; state < size_; ++state) {
      if (term_[state]) {
        dfs_rev(state, visited, go_rev);
      }
    }

    for (size_t state = 0; state < size_; ++state) {
      valid[state] = valid[state] && visited[state];
    }

    visited.assign(size_, false);

    inf_ = find_cycle_(start_, visited);
  }

  void dfs_(size_t curr, vector<bool> &visited) {
    max_depth_ = max(max_depth_, depth_++);
    visited[curr] = true;
    if (term_[curr]) {
      term_reachable_ = true;
    }
    set<size_t> was;
    for (char c = 'a'; c <= 'z'; ++c) {
      auto next = go_[curr][c - 'a'];
      if (find(was.begin(), was.end(), next) == was.end()) {
        was.insert(next);

        if (!visited[next]) {
          dfs_(next, visited);
        }

      }
    }
    depth_--;
  }

  static void dfs_rev(size_t curr, vector<bool> &visited, vector<vector<vector<size_t>>> &go_rev) {
    visited[curr] = true;
    set<size_t> was;
    for (char c = 'a'; c <= 'z'; ++c) {
      for (auto prev : go_rev[curr][c - 'a']) {
        if (find(was.begin(), was.end(), prev) == was.end()) {
          was.insert(prev);

          if (!visited[prev]) {
            dfs_rev(prev, visited, go_rev);
          }

        }
      }
    }
  }

  bool find_cycle_(size_t curr, vector<bool> &visited) {
    visited[curr] = true;

    set<size_t> was;
    for (char c = 'a'; c <= 'z'; ++c) {
      auto next = go_[curr][c - 'a'];
      if (find(was.begin(), was.end(), next) == was.end()) {
        was.insert(next);

        if (valid[next]) {
          if (!visited[next]) {
            if (find_cycle_(next, visited)) {
              return true;
            }
          } else {
            return true;
          }
        }


      }
    }
    return false;
  }
};

void test() {
  size_t n, m, k;
  cin >> n >> m >> k;
  DFA dfa(n);
  size_t state;
  for (size_t i = 0; i < k; i++) {
    cin >> state;
    dfa.set_term(state - 1);
  }
  size_t s1, s2;
  char c;
  for (size_t i = 0; i < m; i++) {
    cin >> s1 >> s2 >> c;
    dfa.link(s1 - 1, s2 - 1, c);
  }
  cout << dfa.count_words();
}

int main() {
  FASTIO
  test();
  return 0;
}
