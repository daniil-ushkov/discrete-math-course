#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

ifstream fin("automaton.in");
ofstream fout("automaton.out");
#define cin fin
#define  cout fout

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

#define TERM 26

void parse_prod(NFA& dfa) {
  string non_term;
  string prod;
  cin >> non_term;
  cin >> prod >> prod;
  if (prod.size() == 2) {
    dfa.link(non_term[0] - 'A', prod[1] - 'A', prod[0]);
  }
  if (prod.size() == 1) {
    dfa.link(non_term[0] - 'A', TERM, prod[0]);
  }
}

void test() {
  NFA dfa(27);
  dfa.set_term(TERM);
  size_t n;
  string start;
  cin >> n >> start;
  dfa.set_start(start[0] - 'A');
  while (n--) {
    parse_prod(dfa);
  }
  cin >> n;
  while (n--) {
    string word;
    cin >> word;
    cout << (dfa.accept(word) ? "yes\n" : "no\n");
  }
}

int main() {
  test();
  return 0;
}