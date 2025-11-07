string pattern; cin >> pattern; // ejemplo: "xxxxx3xxxx" donde x = dígito libre
int n = pattern.size();
long long k; cin >> k; // modulo

vector<vector<vector<long long>>> dp(n, vector<vector<long long>>(k, vector<long long>(2, -1)));

function<long long(int, int, bool, bool)> solve = [&](int pos, int rem, bool tight, bool started) {
  if (pos == n) {
    return (started && rem == 0) ? 1LL : 0LL;
  }
  if (started && !tight && dp[pos][rem][tight ? 1 : 0] != -1) {
    return dp[pos][rem][tight ? 1 : 0];
  }
  long long res = 0;
  if (pattern[pos] != 'x' && pattern[pos] != 'X') {
    int fixed_digit = pattern[pos] - '0';
    bool new_tight = tight && (fixed_digit == 9);
    bool new_started = started || (fixed_digit > 0);
    int new_rem = (rem * 10 + fixed_digit) % k;
    res += solve(pos + 1, new_rem, new_tight, new_started);
  } else {
    int limit = tight ? 9 : 9;
    int start_digit = (pos == 0) ? 1 : 0; // primer dígito no puede ser 0
    for (int d = start_digit; d <= limit; d++) {
      bool new_tight = tight && (d == limit);
      bool new_started = started || (d > 0);
      int new_rem = (rem * 10 + d) % k;
      res += solve(pos + 1, new_rem, new_tight, new_started);
    }
  }
  if (started && !tight) {
    dp[pos][rem][tight ? 1 : 0] = res;
  }
  return res;
};

long long result = solve(0, 0, true, false);
// result = cantidad de números que siguen el patrón y son divisibles por k
// ejemplo: pattern = "xxxxx3xxxx", k = 7
// cuenta números tipo 1234534567 que son divisibles por 7
// x o X = dígito libre, cualquier otro carácter = dígito fijo

