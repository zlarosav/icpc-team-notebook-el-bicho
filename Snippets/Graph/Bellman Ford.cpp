int n, m; cin >> n >> m;
vector<tuple<int, int, long long>> edges;
for (int i = 0; i < m; i++) {
  int u, v; cin >> u >> v;
  long long w; cin >> w;
  edges.emplace_back(u, v, w);
}
vector<long long> dist(n + 1, inf);
dist[1] = 0;
for (int i = 0; i < n - 1; i++) {
  for (auto [u, v, w] : edges) {
    if (dist[u] != inf) {
      dist[v] = min(dist[v], dist[u] + w);
    }
  }
}
bool has_negative_cycle = false;
for (auto [u, v, w] : edges) {
  if (dist[u] != inf && dist[u] + w < dist[v]) {
    has_negative_cycle = true;
    break;
  }
}
// dist[u] = distancia minima desde nodo 1 hasta u
// has_negative_cycle = true si hay ciclo negativo alcanzable desde nodo 1

