#include <fstream>
#include <vector>

using namespace std;

ifstream fin("problem2.in");
ofstream fout("problem2.out");

struct NFA {
  explicit NFA(size_t size)
      : start_(0),
        size_(size),
        go_(size + 1, vector<vector<size_t>>(26, {size})),
        term_(size + 1) {}

  void set_start(size_t state) {
    start_ = state;
  }

  void set_term(size_t state) {
    term_[state] = true;
  }

  void link(size_t s1, size_t s2, char c) {
    go_[s1][c - 'a'].push_back(s2);
  }

  bool accept(string &str) {
    vector<vector<bool>> can(str.size() + 1, vector<bool>(size_));
    can[0][start_] = true;
    for (size_t d = 0; d < str.size(); ++d) {
      char c = str[d];
      for (size_t state = 0; state < size_; ++state) {
        if (can[d][state]) {
          for (size_t next : go_[state][c - 'a']) {
            can[d + 1][next] = true;
          }
        }
      }
    }
    for (size_t state = 0; state < size_; ++state) {
      if (can[str.size()][state] && term_[state]) {
        return true;
      }
    }
    return false;
  }
 private:
  size_t start_;
  size_t size_;
  vector<vector<vector<size_t>>> go_;
  vector<bool> term_;
};



void test() {
  string str;
  fin >> str;
  size_t n, m, k;
  fin >> n >> m >> k;
  NFA nfa(n);
  size_t state;
  for (size_t i = 0; i < k; i++) {
    fin >> state;
    nfa.set_term(state - 1);
  }
  size_t s1, s2;
  char c;
  for (int i = 0; i < m; i++) {
    fin >> s1 >> s2 >> c;
    nfa.link(s1 - 1, s2 - 1, c);
  }
  if (nfa.accept(str)) {
    fout << "Accepts\n";
  } else {
    fout << "Rejects\n";
  }
}

int main() {
  test();
  return 0;
}