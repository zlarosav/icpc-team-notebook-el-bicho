string s; cin >> s; // número como string (puede ser muy grande, tipo 10^100)
int n = s.size();
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
  int limit = tight ? (s[pos] - '0') : 9;
  for (int d = 0; d <= limit; d++) {
    bool new_tight = tight && (d == limit);
    bool new_started = started || (d > 0);
    int new_rem = (rem * 10 + d) % k;
    res += solve(pos + 1, new_rem, new_tight, new_started);
  }
  if (started && !tight) {
    dp[pos][rem][tight ? 1 : 0] = res;
  }
  return res;
};

long long result = solve(0, 0, true, false);
// result = cantidad de números <= s que son divisibles por k
// para contar en rango [a, b]: result_b - result_a-1
// ejemplo: s = "1000000", k = 7 -> contar números de 0 a 1000000 divisibles por 7

