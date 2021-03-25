#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <list>
#include <sstream>

using namespace std;

ifstream fin("useless.in");
ofstream fout("useless.out");
#define cin fin
#define  cout fout

bool non_term(char c) {
  return 'A' <= c && c <= 'Z';
}

bool term(char c) {
  return 'a' <= c && c <= 'z';
}

struct rule {
  char LEFT;
  string RIGHT;
  vector<bool> non_terms;

  rule() = default;

  rule(char from, string& to) : non_terms(26, false){
    LEFT = from;
    RIGHT = to;
    for (char c : RIGHT) {
      if (non_term(c)) {
        non_terms[c - 'A'] = true;
      }
    }
  }

  bool legal() const {
    bool res = 'A' <= LEFT && LEFT <= 'Z';
    for (char c : RIGHT) res = res && (non_term(c) || ('a' <= c && c <= 'z'));
    return res;
  }

  bool epsilon() const  {
    return RIGHT.empty();
  }

  bool generating() const {
    bool res = true;
    for (char c : RIGHT) res = res && term(c);
    return res;
  }

  bool contains(char non_term) const {
    return non_terms[non_term - 'A'] || non_term == LEFT;
  }
};

istream& operator>>(istream& in, rule& r) {
  in >> ws;
  string line;
  getline(in, line);
  stringstream str_stream(line);

  string FROM, arrow, TO;
  str_stream >> FROM >> arrow >> TO;
  bool legal = FROM.size() == 1 && 'A' <= FROM[0] && FROM[0] <= 'Z' && arrow == "->";
  for (char c : TO) legal = legal && (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'));
  if (!legal) {
    throw runtime_error("illegal rule");
  }
  r = rule(FROM[0], TO);
  return in;
}

struct CFG {
  explicit CFG(char start, vector<rule> const& rules) : start(start), rules_(rules.begin(), rules.end()) {};

  vector<bool> epsilon() {
    vector<bool> epsilon_non_term(26, false);
    for (rule& r : rules_) {
      if (r.epsilon()) {
        epsilon_non_term[r.LEFT - 'A'] = true;
      }
    }
    while (true) {
      bool changed = false;
      for (rule& r : rules_) {
        if (!epsilon_non_term[r.LEFT - 'A']) {
          bool eps = true;
          for (char c : r.RIGHT) eps = eps && (non_term(c) && epsilon_non_term[c - 'A']);
          if (eps) {
            changed = true;
            epsilon_non_term[r.LEFT - 'A'] = true;
          }
        }
      }
      if (!changed) break;
    }
    return epsilon_non_term;
  }

  vector<bool> generating() {
    vector<bool> gen_non_terms(26, false);
    for (rule const& r : rules_) {
      if (r.generating()) {
        gen_non_terms[r.LEFT - 'A'] = true;
      }
    }
    while (true) {
      bool changed = false;
      for (rule const& r : rules_) {
        if (!gen_non_terms[r.LEFT - 'A']) {
          bool gen = true;
          for (char c : r.RIGHT) gen = gen && (term(c) || gen_non_terms[c - 'A']);
          if (gen) {
            changed = true;
            gen_non_terms[r.LEFT - 'A'] = true;
          }
        }
      }
      if (!changed) break;
    }
    return gen_non_terms;
  }

  vector<bool> reachable() {
    vector<bool> reachable_non_terms(26, false);
    reachable_non_terms[start - 'A'] = true;
    while (true) {
      bool changed = false;
      for (rule const& r : rules_) {
        if (reachable_non_terms[r.LEFT - 'A']) {
          for (char non_term = 'A'; non_term <= 'Z'; ++non_term) {
            if (r.contains(non_term) && !reachable_non_terms[non_term - 'A']) {
              changed = true;
              reachable_non_terms[non_term - 'A'] = true;
            }
          }
        }
      }
      if (!changed) break;
    }
    return reachable_non_terms;
  }

  vector<bool> used_non_terms() {
    vector<bool> res(26, false);
    for (rule const& r : rules_) {
      res[r.LEFT - 'A'] = true;
      for (char c : r.RIGHT) {
        if (non_term(c)) {
          res[c - 'A'] = true;
        }
      }
    }
    return res;
  }

  void delete_containing(char non_term) {
    auto it = rules_.begin();
    while (it != rules_.end()) {
      auto cur = it;
      ++cur;
      if (it->contains(non_term)) rules_.erase(it);
      it = cur;
    }
  }

  vector<bool> delete_useless() {
    vector<bool> used_non_terms = CFG::used_non_terms();

    vector<bool> gen_non_terms = generating();
    for (char non_term = 'A'; non_term <= 'Z'; ++non_term) {
      if (!gen_non_terms[non_term - 'A']) {
        delete_containing(non_term);
      }
    }

    vector<bool> reachable_non_terms = reachable();
    for (char non_term = 'A'; non_term <= 'Z'; ++non_term) {
      if (!reachable_non_terms[non_term - 'A']) {
        delete_containing(non_term);
      }
    }

    vector<bool> useful_non_terms = CFG::used_non_terms();
    for (char non_term = 'A'; non_term <= 'Z'; ++non_term) {
      used_non_terms[non_term - 'A'] = used_non_terms[non_term - 'A'] && !useful_non_terms[non_term - 'A'];
    }

    if (!useful_non_terms[start - 'A']) {
      used_non_terms[start - 'A'] = true;
    }

    return used_non_terms;
  }

 private:
  list<rule> rules_;
  char start;
};

void test() {
  size_t n;
  string start;
  cin >> n >> start;
  vector<rule> rules;
  while (n--) {
    rule r;
    cin >> r;
    rules.push_back(r);
  }
  CFG g(start[0], rules);
  vector<bool> eps = g.delete_useless();
  for (size_t i = 0; i < eps.size(); ++i) {
    if (eps[i]) {
      cout << static_cast<char>(i + 'A') << ' ';
    }
  }
}

int main() {
  test();
  return 0;
}