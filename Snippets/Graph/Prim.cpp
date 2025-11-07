int n, m; cin >> n >> m;
vector<vector<pair<int, long long>>> adj(n + 1);
for (int i = 0; i < m; i++) {
  int u, v; cin >> u >> v;
  long long w; cin >> w;
  adj[u].emplace_back(v, w);
  adj[v].emplace_back(u, w);
}
vector<bool> vis(n + 1);
pqg<pair<long long, int>> pq;
pq.push({0LL, 1});
long long mst_cost = 0;
while (!pq.empty()) {
  auto [w, u] = pq.top(); pq.pop();
  if (vis[u]) continue;
  vis[u] = true;
  mst_cost += w;
  for (auto [v, weight] : adj[u]) {
    if (!vis[v]) {
      pq.push({weight, v});
    }
  }
}
// mst_cost = costo del MST (Minimum Spanning Tree)

