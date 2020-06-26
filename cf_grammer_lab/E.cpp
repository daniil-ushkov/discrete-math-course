#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <sstream>

using namespace std;

#pragma GCC optimize("O3")

ifstream fin("cf.in");
ofstream fout("cf.out");
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

  rule(char from, string &to) : non_terms(26, false) {
    LEFT = from;
    RIGHT = to;
    for (char c : RIGHT) {
      if (non_term(c)) {
        non_terms[c - 'A'] = true;
      }
    }
  }

  bool single() const {
    return RIGHT.size() == 1 && term(RIGHT[0]);
  }

  bool doub() const {
    return RIGHT.size() == 2 && non_term(RIGHT[0]) && non_term(RIGHT[1]);
  }

  bool epsilon() const {
    return RIGHT.empty();
  }
};

istream &operator>>(istream &in, rule &r) {
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

#define MOD 1000000007

struct CFG {
  explicit CFG(char start, vector<rule> const &rules) : start(start), rules_(rules.begin(), rules.end()) {};

  //CYK modified
  bool accept(string &word) {
    size_t n = word.size();
    vector<vector<vector<bool>>> d(26,
                                   vector<vector<bool>>(n + 1, vector<bool>(n + 1, false)));
    vector<vector<vector<vector<bool>>>> h(
        rules_.size(),
        vector<vector<vector<bool>>>(
            n + 1, vector<vector<bool>>(n + 1, vector<bool>(10, false))));

    //epsilons
    for (rule const &r : rules_) {
      if (r.epsilon()) {
        for (size_t i = 0; i <= n; ++i) {
          d[r.LEFT - 'A'][i][i] = true;
        }
      }
    }

    //terminals
    for (rule const &r : rules_) {
      if (r.single()) {
        for (size_t i = 0; i < n; ++i) {
          if (r.RIGHT[0] == word[i]) {
            d[r.LEFT - 'A'][i][i + 1] = true;
          }
        }
      }
    }

    //additional dynamic
    for (size_t r = 0; r < rules_.size(); ++r) {
      for (size_t i = 0; i <= n; ++i) {
        h[r][i][i][0] = true;
      }
    }

    for (size_t l = 0; l <= n; ++l) {
      for (size_t it = 0; it < 5; ++it) {
        // additional dynamic
        for (size_t r = 0; r < rules_.size(); ++r) {
          for (size_t i = 0; i <= n; ++i) {
            if (i + l <= n) {
              for (size_t k = 1; k <= rules_[r].RIGHT.size(); ++k) {
                for (size_t m = i; m <= i + l; ++m) {
                  if (term(rules_[r].RIGHT[k - 1])) {
                    h[r][i][i + l][k] =
                        h[r][i][i + l][k] || (h[r][i][m][k - 1] && m + 1 == i + l && rules_[r].RIGHT[k - 1] == word[m]);
                  } else {
                    h[r][i][i + l][k] =
                        h[r][i][i + l][k] || (h[r][i][m][k - 1] && d[rules_[r].RIGHT[k - 1] - 'A'][m][i + l]);
                  }
                }
              }
            }
          }
        }
        //main dynamic
        for (size_t r = 0; r < rules_.size(); ++r) {
          for (size_t i = 0; i <= n; ++i) {
            if (i + l <= n) {
              d[rules_[r].LEFT - 'A'][i][i + l] =
                  d[rules_[r].LEFT - 'A'][i][i + l] || h[r][i][i + l][rules_[r].RIGHT.size()];
            }
          }
        }
      }
    }

    return d[start - 'A'][0][n];
  }

 private:
  vector<rule> rules_;
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
  CFG cfg(start[0], rules);
  string word;
  cin >> word;
  cout << (cfg.accept(word) ? "yes\n" : "no\n");

}

int main() {
  test();
  return 0;
}