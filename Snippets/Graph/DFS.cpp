vector<bool> vis(n+1);
function<void(int)> dfs = [&](int u) {
  vis[u] = true;
  for (int& v : adj[u]) {
    if (vis[v]) continue;
    dfs(v);
  }
};

for (int u = 1; u <= n; u++) {
  if (vis[u]) continue;
  dfs(u);
}