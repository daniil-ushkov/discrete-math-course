#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>

using namespace std;

#pragma GCC optimize("O3")
ifstream fin("minimization.in");
ofstream fout("minimization.out");
#define cin fin
#define cout fout

#define FOR_C for (char c = 'a'; c <= 'z'; ++c)
#define TRASH 0

struct DFA {
  explicit DFA(size_t start, size_t trash, vector<vector<size_t>> &go, vector<bool> &term)
      : start_(start),
        trash_(trash),
        size_(go.size()),
        go_(go),
        term_(term) {}
  explicit DFA(size_t size)
      : start_(1),
        trash_(0),
        size_(size + 1),
        go_(size_, vector<size_t>(26, TRASH)),
        term_(size_) {}

  void set_term(size_t state) {
    term_[state] = true;
  }

  void link(size_t s1, size_t s2, char c) {
    go_[s1][c - 'a'] = s2;
  }

  DFA minimize() {
    //Searching of reachable states
    vector<bool> is_reach(size_);
    vector<size_t> reachable;
    queue<size_t> reach_q;

    reach_q.push(start_);
    is_reach[start_] = true;
    reachable.push_back(start_);

    while (!reach_q.empty()) {
      size_t cur = reach_q.front();
      reach_q.pop();
      FOR_C {
        auto next = go_[cur][c - 'a'];
        if (!is_reach[next]) {
          is_reach[next] = true;
          reachable.push_back(next);
          reach_q.push(next);
        }
      }
    }

    //Creating of reversed go_ on set of reachable states
    vector<vector<vector<size_t>>> go_rev(size_, vector<vector<size_t>>(26));
    for (auto state : reachable) {
      FOR_C {
        auto next = go_[state][c - 'a'];
        if (is_reach[next]) {
          go_rev[next][c - 'a'].push_back(state);
        }
      };
    }

    //Creating of equivalent states table
    vector<vector<bool>> diff(size_, vector<bool>(size_));
    queue<pair<size_t, size_t>> diff_q;
    for (auto state1 : reachable) {
      for (auto state2 : reachable) {
        if (!diff[state1][state2] && term_[state1] != term_[state2]) {
          diff[state1][state2] = diff[state2][state1] = true;
          diff_q.push({state1, state2});
        }
      }
    }

    while (!diff_q.empty()) {
      auto cur = diff_q.front();
      diff_q.pop();
      FOR_C {
        for (auto state1 : go_rev[cur.first][c - 'a']) {
          for (auto state2 : go_rev[cur.second][c - 'a']) {
            if (!diff[state1][state2]) {
              diff[state1][state2] = diff[state2][state1] = true;
              diff_q.push({state1, state2});
            }
          }
        }
      }
    }

    vector<size_t> component(size_, SIZE_MAX);
    size_t comp_num = 0;
    for (auto state1 : reachable) {
      if (component[state1] == SIZE_MAX) {
        for (auto state2 : reachable) {
          if (!diff[state1][state2]) {
            component[state2] = comp_num;
          }
        }
        comp_num++;
      }
    }

    //DFA creating
    vector<vector<size_t>> go(comp_num, vector<size_t>(26, component[trash_]));
    vector<bool> term(comp_num);

    for (size_t comp = 0; comp < comp_num; ++comp) {
      for (auto state : reachable) {
        if (component[state] == comp) {
          FOR_C {
            for (auto prev : go_rev[state][c - 'a']) {
              go[component[prev]][c - 'a'] = component[state];
            }
          }
        }
      }
    }

    for (auto state : reachable) {
      if (term_[state]) {
        term[component[state]] = true;
      }
    }

    return DFA(component[start_], component[trash_], go, term);
  }

 private:
  size_t start_;
  size_t trash_;
  size_t size_;
  vector<vector<size_t>> go_;
  vector<bool> term_;

  friend ostream &operator<<(ostream &, DFA);
};

istream &operator>>(istream &in, DFA &dfa) {
  size_t links_num, terms_num;
  in >> links_num >> terms_num;
  for (size_t i = 0; i < terms_num; i++) {
    size_t t;
    in >> t;
    dfa.set_term(t);
  }
  for (size_t i = 0; i < links_num; i++) {
    size_t s1, s2;
    char c;
    in >> s1 >> s2 >> c;
    dfa.link(s1, s2, c);
  }
  return in;
}

ostream &operator<<(ostream &out, DFA dfa) {
  vector<tuple<size_t, size_t, char>> links;
  for (size_t state = 0; state < dfa.size_; ++state) {
    FOR_C {
      if (dfa.go_[state][c - 'a'] != dfa.trash_) {
        links.emplace_back(state, dfa.go_[state][c - 'a'], c);
      }
    }
  }
  vector<size_t> terms;
  for (size_t state = 0; state < dfa.size_; ++state) {
    if (dfa.term_[state]) {
      terms.push_back(state);
    }
  }
  out << dfa.size_ - 1 << " " << links.size() << " " << terms.size() << "\n";
  for (auto term : terms) {
    out << term << " ";
  }
  out << "\n";
  for (auto link : links) {
    out << get<0>(link) << " " << get<1>(link) << " " << get<2>(link) << "\n";
  }
  return out;
}

void run() {
  size_t size = 0;
  cin >> size;
  DFA dfa(size);
  cin >> dfa;
  cout << dfa.minimize();
}

int main() {
  run();
  return 0;
}