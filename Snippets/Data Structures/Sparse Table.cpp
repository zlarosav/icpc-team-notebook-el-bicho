int n; cin >> n;
vector<long long> a(n);
for (int i = 0; i < n; i++) cin >> a[i];
int k = log2(n) + 1;
vector<vector<long long>> st(n, vector<long long>(k));
for (int i = 0; i < n; i++) st[i][0] = a[i];
for (int j = 1; j < k; j++) {
  for (int i = 0; i + (1 << j) <= n; i++) {
    st[i][j] = min(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
  }
}
function<long long(int, int)> query = [&](int l, int r) {
  int j = log2(r - l + 1);
  return min(st[l][j], st[r - (1 << j) + 1][j]);
};
// query(l, r) = mínimo en rango [l, r] en O(1)
// cambiar min por max para máximo
// cambiar min por gcd para GCD en rango

