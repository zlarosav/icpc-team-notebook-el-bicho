int n; cin >> n;
vector<vector<long long>> dist(n + 1, vector<long long>(n + 1, inf));
for (int i = 1; i <= n; i++) dist[i][i] = 0;
int m; cin >> m;
for (int i = 0; i < m; i++) {
  int u, v; cin >> u >> v;
  long long w; cin >> w;
  dist[u][v] = min(dist[u][v], w);
  dist[v][u] = min(dist[v][u], w);
}
for (int k = 1; k <= n; k++) {
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      if (dist[i][k] != inf && dist[k][j] != inf) {
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
      }
    }
  }
}
// dist[i][j] = distancia minima entre nodo i y nodo j

