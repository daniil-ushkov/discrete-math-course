#include <fstream>
#include <vector>

using namespace std;

ifstream fin("problem1.in");
ofstream fout("problem1.out");

struct DFA {
  explicit DFA(size_t size)
      : start_(0),
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
 private:
  size_t start_;
  vector<vector<size_t>> go_;
  vector<bool> term_;
};

void test() {
  string str;
  fin >> str;
  size_t n, m, k;
  fin >> n >> m >> k;
  DFA dfa(n);
  size_t state;
  for (size_t i = 0; i < k; i++) {
    fin >> state;
    dfa.set_term(state - 1);
  }
  size_t s1, s2;
  char c;
  for (int i = 0; i < m; i++) {
    fin >> s1 >> s2 >> c;
    dfa.link(s1 - 1, s2 - 1, c);
  }
  if (dfa.accept(str)) {
    fout << "Accepts\n";
  } else {
    fout << "Rejects\n";
  }
}

int main() {
  test();
  return 0;
}