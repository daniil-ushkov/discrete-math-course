#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

ifstream fin("epsilon.in");
ofstream fout("epsilon.out");
#define cin fin
#define  cout fout

struct rule {
  char LEFT;
  string RIGHT;

  bool legal() {
    bool res = 'A' <= LEFT && LEFT <= 'Z';
    for (char c : RIGHT) res = res && (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'));
    return res;
  }

  bool epsilon() {
    return RIGHT.empty();
  }

  bool check_all(bool (*p)(char)) {
    bool res = true;
    for (char c : RIGHT) res = res && p(c);
    return res;
  }
};
istream& operator>>(istream& in, rule& r) {
  in >> ws;
  string str;
  getline(cin, str);
  stringstream str_stream(str);

  string FROM, arrow, TO;
  str_stream >> FROM >> arrow >> TO;
  bool legal = FROM.size() == 1 && 'A' <= FROM[0] && FROM[0] <= 'Z' && arrow == "->";
  for (char c : TO) legal = legal && (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'));
  if (!legal) {
    throw runtime_error("illegal rule");
  }
  r = {FROM[0], TO};
  return in;
}

bool non_term(char c) {
  return 'A' <= c && c <= 'Z';
}

struct CFG {
  explicit CFG(vector<rule> const& rules) : rules_(rules) {};

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
 private:
  vector<rule> rules_;
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
  CFG g(rules);
  vector<bool> eps = g.epsilon();
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