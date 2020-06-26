#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <list>
#include <sstream>

using namespace std;

ifstream fin("nfc.in");
ofstream fout("nfc.out");
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

  bool single() const {
    return RIGHT.size() == 1 && term(RIGHT[0]);
  }

  bool doub() const {
    return RIGHT.size() == 2 && non_term(RIGHT[0]) && non_term(RIGHT[1]);
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

#define MOD 1000000007

struct CFG {
  explicit CFG(char start, vector<rule> const& rules) : start(start), rules_(rules.begin(), rules.end()) {};

  uint32_t count(string& word) {
    size_t n = word.size();
    vector<vector<vector<uint32_t>>> dp(26,
        vector<vector<uint32_t>>(n + 1, vector<uint32_t>(n + 1)));
    for (rule const& r : rules_) {
      for (size_t i = 0; i < n; ++i) {
        if (r.single() && r.RIGHT[0] == word[i]) {
          dp[r.LEFT - 'A'][i][i + 1] = 1;
        }
      }
    }
    for (size_t d = 1; d <= n; ++d) {
      for (rule const& r : rules_) {
        if (r.doub()) {
          for (size_t l = 0; l < n; ++l) {
            if (l + d <= n) {
              for (size_t m = l + 1; m < l + d; ++m) {
                dp[r.LEFT - 'A'][l][l + d] =
                    (dp[r.LEFT - 'A'][l][l + d] + (uint64_t) dp[r.RIGHT[0] - 'A'][l][m] * dp[r.RIGHT[1] - 'A'][m][l + d] % MOD) % MOD;
              }
            }
          }
        }
      }
    }
    return dp[start - 'A'][0][n];
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
  CFG cfg(start[0], rules);
  string word;
  cin >> word;
  cout << cfg.count(word);
}

int main() {
  test();
  return 0;
}