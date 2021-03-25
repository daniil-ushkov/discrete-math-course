#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

//#pragma GCC optimize("O3")
//ifstream fin("problem4.in");
//ofstream fout("problem4.out");
//#define cin fin
//#define cout fout
#define FASTIO ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);

const uint32_t INF = 0xFFFFFFFF;
const uint32_t MOD = 1e9 + 7;

struct DFA {


  explicit DFA(size_t size)
      : start_(0),
        size_(size + 1),
        go_(size_, vector<size_t>(26, size_ - 1)),
        term_(size_),
        graph_matrix_(size_, vector<uint32_t>(size_)),
        cnt_(2001, vector<uint32_t>(size_)),
        valid_cnt_(false) {
    for (size_t state = 0; state < size_; ++state) {
      graph_matrix_[state][size] = 26;
    }
  }



  //Init
  void set_start(size_t state) {
    start_ = state;
  }
  void set_term(size_t state) {
    term_[state] = true;
  }
  void link(size_t s1, size_t s2, char c) {
    graph_matrix_[s1][s2]++;
    graph_matrix_[s1][go_[s1][c - 'a']]--;
    go_[s1][c - 'a'] = s2;
  }



  bool accept(string &str) {
    size_t state = start_;
    for (char c : str) {
      state = go_[state][c - 'a'];
    }
    return term_[state];
  }



  uint32_t count_words_of_length(size_t len) {
    if (!valid_cnt_) {
      calc_cnt_();
    }
    uint32_t result = 0;
    for (size_t state = 0; state < size_; ++state) {
      if (term_[state]) {
        result = (result + cnt_[len][state]) % MOD;
      }
    }
    return result;
  }



 private:
  //Base
  size_t start_;
  size_t size_;
  vector<vector<size_t>> go_;
  vector<bool> term_;


  //Counting words of length
  vector<vector<uint32_t>> graph_matrix_;
  vector<vector<uint32_t>> cnt_;
  bool valid_cnt_;
  void calc_cnt_() {
    cnt_.assign(2001, vector<uint32_t>(size_));
    cnt_[0][start_] = 1;
    for (size_t d = 0; d < 2000; ++d) {
      for (size_t state = 0; state < size_; ++state) {
        for (size_t next = 0; next < size_; ++next) {
          cnt_[d + 1][next] = (cnt_[d + 1][next] + ((uint64_t) graph_matrix_[state][next] * cnt_[d][state]) % MOD) % MOD;
        }
      }
    }
  }
};

void test() {
  size_t n, m, k, l;
  cin >> n >> m >> k >> l;
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
  cout << dfa.count_words_of_length(l);
}

int main() {
  FASTIO
  test();
  return 0;
}
