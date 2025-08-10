template <class T> using pq = priority_queue<T>;
template <class T> using pqg = priority_queue<T, vector<T>, greater<T>>;

void solve() {
  int n, m; cin >> n >> m;
  vector<vector<pair<int, ll>>> adj(n+1);
  while (m--) {
    int u, v; ll w; cin >> u >> v >> w;
    adj[u].push_back({v, w});
  }

  vector<ll> dist(n+1, MX);
  pqg<pair<ll, int>> q;
  q.push({0LL, 1});
  dist[1] = 0LL;
  while (!q.empty()) {
    auto [d, u] = q.top();
    q.pop();
    if (dist[u] < d) continue;
    for (auto [v, w] : adj[u]) {
      ll new_d = d + w;
      if (new_d < dist[v]) {
        dist[v] = new_d;
        q.push({dist[v], v});
      }
    }
  }

  for (int u = 1; u <= n; u++) cout << dist[u] << ' ';
  cout << '\n';
}