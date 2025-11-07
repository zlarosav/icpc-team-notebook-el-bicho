int n, capacity; cin >> n >> capacity;
vector<int> weight(n), value(n);
for (int i = 0; i < n; i++) {
  cin >> weight[i] >> value[i];
}
vector<long long> dp(capacity + 1, 0);
for (int i = 0; i < n; i++) {
  for (int w = capacity; w >= weight[i]; w--) {
    dp[w] = max(dp[w], dp[w - weight[i]] + value[i]);
  }
}
// dp[capacity] = valor máximo que se puede obtener con capacidad máxima
// para version con items ilimitados, cambiar el loop: for (int w = weight[i]; w <= capacity; w++)

