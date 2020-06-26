#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

#pragma GCC optimize("O3")
ifstream fin("isomorphism.in");
ofstream fout("isomorphism.out");
#define cin fin
#define cout fout

const size_t INF = SIZE_MAX;

struct DFA {
  explicit DFA(size_t size)
      : start_(0),
        size_(size + 1),
        go_(size + 1, vector<size_t>(26, size)),
        term_(size + 1) {}

  void set_start(size_t state) {
    start_ = state;
  }

  void set_term(size_t state) {
    term_[state] = true;
  }

  void link(size_t s1, size_t s2, char c) {
    go_[s1][c - 'a'] = s2;
  }

  bool accept(string &str) {
    size_t state = start_;
    for (char c : str) {
      state = go_[state][c - 'a'];
    }
    return term_[state];
  }

  bool equals(DFA &dfa) {
    if (size_ != dfa.size_) {
      return false;
    }
    vector<bool> visited_(size_);
    vector<size_t> map(size_, INF);

    return equals_(dfa, map, start_, dfa.start_);
  }

 private:
  size_t start_;
  size_t size_;
  vector<vector<size_t>> go_;
  vector<bool> term_;

  bool equals_(DFA &dfa, vector<size_t> &map, size_t s1, size_t s2) {
    map[s1] = s2;
    if (term_[s1] != dfa.term_[s2]) {
      return false;
    }
    bool result = true;
    for (char c = 'a'; c <= 'z'; ++c) {
      if (map[go_[s1][c - 'a']] == INF) {
        result = result && equals_(dfa, map, go_[s1][c - 'a'], dfa.go_[s2][c - 'a']);
      } else {
        if (map[go_[s1][c - 'a']] != dfa.go_[s2][c - 'a']) {
          return false;
        }
      }
    }
    return result;
  }
};

istream &operator>>(istream &in, DFA &dfa) {
  size_t m, k;
  cin >> m >> k;
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
  return in;
}

void test() {
  size_t n;
  cin >> n;
  DFA dfa(n);
  cin >> dfa;
  cin >> n;
  DFA dfa1(n);
  cin >> dfa1;
  if (dfa.equals(dfa1)) {
    cout << "YES\n";
  } else {
    cout << "NO\n";
  }
}

int main() {
  test();
  return 0;
}