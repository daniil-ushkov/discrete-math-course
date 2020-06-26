#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <queue>
#include <map>
#include <tuple>
#include <set>

using namespace std;

//#pragma GCC optimize("O3")
//ifstream fin("problem5.in");
//ofstream fout("problem5.out");
//#define cin fin
//#define cout fout

const uint32_t MOD = 1e9 + 7;

struct FSA {
  explicit FSA(size_t size)
      : start_(0), size_(size + 1), term_(size + 1) {}

  void set_start(size_t state) {
    start_ = state;
  }

  void set_term(size_t state) {
    term_[state] = true;
  }

  virtual void link(size_t s1, size_t s2, char c) = 0;
  virtual bool accept(string &str) = 0;

 protected:
  size_t start_;
  size_t size_;
  vector<bool> term_;
};

istream &operator>>(istream &in, FSA &dfa) {
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

struct DFA : FSA {
  explicit DFA(size_t size)
      : FSA(size),
        go_(size_, vector<size_t>(26, size)),
        graph_matrix_(size_, vector<uint32_t>(size_)) {}

  void link(size_t s1, size_t s2, char c) override {
    graph_matrix_[s1][s2]++;
    graph_matrix_[s1][go_[s1][c - 'a']]--;
    go_[s1][c - 'a'] = s2;
  }

  bool accept(string &str) override {
    size_t state = start_;
    for (char c : str) {
      state = go_[state][c - 'a'];
    }
    return term_[state];
  }

  uint32_t count_words_of_length(size_t len) {
    calc_cnt_();
    uint32_t result = 0;
    for (size_t state = 0; state < size_; ++state) {
      if (term_[state]) {
        result = (result + cnt_[len][state]) % MOD;
      }
    }
    return result;
  }

 private:
  friend ostream &operator<<(ostream &, DFA);

  vector<vector<size_t>> go_;

  vector<vector<uint32_t>> graph_matrix_;
  vector<vector<uint32_t>> cnt_;
  void calc_cnt_() {
    cnt_.assign(2001, vector<uint32_t>(size_));
    cnt_[0][start_] = 1;
    for (size_t d = 0; d < 2000; ++d) {
      for (size_t state = 0; state < size_; ++state) {
        for (size_t next = 0; next < size_; ++next) {
          cnt_[d + 1][next] = (cnt_[d + 1][next] +
              ((uint64_t) graph_matrix_[state][next] * cnt_[d][state]) % MOD) % MOD;
        }
      }
    }
  }
};

struct NFA : FSA {
  explicit NFA(size_t size)
      : FSA(size),
        go_(size + 1, vector<set<size_t>>(26, {size})) {}

  void link(size_t s1, size_t s2, char c) override {
    go_[s1][c - 'a'].insert(s2);
  }

  bool accept(string &str) override {
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

  //Thompson's algorithm
  DFA convert() {
    typedef vector<bool> mask;

    map<mask, size_t> state_id;
    vector<tuple<size_t, size_t, char>> links;
    vector<size_t> terms;

    queue<mask> q;

    mask start(size_ - 1);
    start[start_] = true;

    state_id.insert({start, state_id.size()});
    if (term_[start_]) {
      terms.push_back(state_id[start]);
    }
    q.push(start);

    while (!q.empty()) {
      mask cur = q.front();
      q.pop();

      for (char c = 'a'; c <= 'z'; ++c) {
        mask next(size_ - 1);
        bool actual = false;
        bool term = false;

        for (size_t state = 0; state < size_ - 1; ++state) {
          if (cur[state]) {
            for (size_t next_state : go_[state][c - 'a']) {
              if (next_state != size_ - 1) {
                next[next_state] = true;
                actual = true;
                if (term_[next_state]) {
                  term = true;
                }
              }
            }
          }
        }

        if (!actual) {
          continue;
        }

        if (state_id.find(next) == state_id.end()) {
          state_id.insert({next, state_id.size()});
          if (term) {
            terms.push_back(state_id[next]);
          }
          q.push(next);
        }

        links.emplace_back(state_id[cur], state_id[next], c);
      }
    }

    DFA dfa(state_id.size());
    for (auto link : links) {
      dfa.link(get<0>(link), get<1>(link), get<2>(link));
    }
    for (size_t term : terms) {
      dfa.set_term(term);
    }

    return dfa;
  }

 private:
  vector<vector<set<size_t>>> go_;
};

#define FOR_C for (char c = 'a'; c <= 'z'; ++c)

ostream &operator<<(ostream &out, DFA dfa) {
  vector<tuple<size_t, size_t, char>> links;
  for (size_t state = 0; state < dfa.go_.size() - 1; ++state) {
    FOR_C {
        if (dfa.go_[state][c - 'a'] != dfa.go_.size() - 1) {
          links.emplace_back(state + 1, dfa.go_[state][c - 'a'] + 1, c);
        }
    }
  }

  vector<size_t> terms;
  for (size_t state = 0; state < dfa.go_.size(); ++state) {
    if (dfa.term_[state]) {
      terms.push_back(state + 1);
    }
  }

  out << dfa.go_.size() - 1 << " " << links.size() << " " << terms.size() << "\n";
  for (auto term : terms) {
    out << term << " ";
  }
  out << "\n";
  for (auto link : links) {
    out << get<0>(link) << " " << get<1>(link) << " " << get<2>(link) << "\n";
  }
  return out;
}

void test() {
  size_t n, m, k;
  cin >> n >> m >> k;
  NFA nfa(n);
  size_t state;
  for (size_t i = 0; i < k; i++) {
    cin >> state;
    nfa.set_term(state - 1);
  }
  size_t s1, s2;
  char c;
  for (size_t i = 0; i < m; i++) {
    cin >> s1 >> s2 >> c;
    nfa.link(s1 - 1, s2 - 1, c);
  }
  cout << nfa.convert();
}

int main() {
  test();
  return 0;
}