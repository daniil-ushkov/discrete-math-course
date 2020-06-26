#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

ifstream fin("problem4.in");
ofstream fout("problem4.out");

typedef vector<vector<size_t>> matrix;
matrix* dot(matrix* m1, matrix* m2) {
  size_t size = m1->size();
  auto* result = new matrix(size, vector<size_t>(size));
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      for (size_t k = 0; k < size; k++) {
        (*result)[i][j] += (*m1)[i][k] * (*m2)[k][j];
      }
    }
  }
  return result;
}

matrix* one(size_t size) {
  auto* result = new matrix(size, vector<size_t>(size));
  for (size_t i = 0; i < size; i++) {
    (*result)[i][i] = 1;
  }
  return result;
}

matrix* power_(matrix* m, uint32_t n) {
  if (n == 0) {
    return one(m->size());
  }
  if (n % 2 == 0) {
    auto* m1 = power_(m, n / 2);
    return dot(m1, m1);
  } else {
    return dot(power_(m, n - 1), m);
  }
}

matrix* transpose(matrix* m) {
  auto* result = new matrix(m->size(), vector<size_t>(m->size()));
}

struct DFA {
  explicit DFA(size_t size)
      : start_(0),
        go_(size + 1, vector<size_t>(26, size)),
        term_(size + 1),
        graph_matrix_(size + 1, vector<size_t>(size + 1)) {
    for (size_t state = 0; state <= size; ++state) {
      graph_matrix_[state][size] = 1;
    }
  }

  void set_start(size_t state) {
    start_ = state;
  }

  void set_term(size_t state) {
    term_[state] = true;
  }

  void link(size_t s1, size_t s2, char c) {
    go_[s1][c - 'a'] = s2;
    graph_matrix_[s1][s2] = 1;
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

  vector<vector<size_t>> graph_matrix_;
};

void test() {
  string str;
  cin >> str;
  size_t n, m, k;
  cin >> n >> m >> k;
  DFA dfa(n);
  size_t state;
  for (size_t i = 0; i < k; ++i) {
    cin >> state;
    dfa.set_term(state - 1);
  }
  size_t s1, s2;
  char c;
  for (int i = 0; i < m; ++i) {
    cin >> s1 >> s2 >> c;
    dfa.link(s1 - 1, s2 - 1, c);
  }
  if (dfa.accept(str)) {
    cout << "Accepts\n";
  } else {
    cout << "Rejects\n";
  }
}

int main() {
//  test();
  matrix m1 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  matrix m2 = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
  matrix m3 = *power(&m1, 10);


  return 0;
}