struct DSU {
  vector<int> p, size;
  DSU(int n) {
    p.resize(n + 1), size.resize(n + 1, 1);
    for (int i = 1; i <= n; i++) p[i] = i;
  }
  int find(int x) {
    if (p[x] != x) p[x] = find(p[x]);
    return p[x];
  }
  void merge(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return;
    if (size[x] < size[y]) swap(x, y);
    size[x] += size[y];
    p[y] = x;
  }
};

int n, m; cin >> n >> m;
vector<tuple<long long, int, int>> edges;
for (int i = 0; i < m; i++) {
  int u, v; cin >> u >> v;
  long long w; cin >> w;
  edges.emplace_back(w, u, v);
}
sort(edges.begin(), edges.end());
DSU dsu(n);
long long mst_cost = 0;
for (auto [w, u, v] : edges) {
  if (dsu.find(u) != dsu.find(v)) {
    dsu.merge(u, v);
    mst_cost += w;
  }
}
// mst_cost = costo del MST (Minimum Spanning Tree)

