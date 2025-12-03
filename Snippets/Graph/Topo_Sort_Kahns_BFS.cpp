// O((N + M)*logN). 0-indexed. Topological sort (Kahn BFS) con min-heap (lexicográficamente mínimo)
vector<int> topo_sort(int n, const vector<vector<int>>& adj) {
    vector<int> indeg(n, 0); // in-degree de cada nodo
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) {
            indeg[v]++;
        }
    }
    // min-heap para siempre sacar el nodo de menor índice
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int u = 0; u < n; u++) {
        if (indeg[u] == 0) pq.push(u);
    }

    vector<int> order;
    order.reserve(n);
    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (--indeg[v] == 0) {
                pq.push(v);
            }
        }
    }
    if ((int)order.size() != n) { return {}; }
    return order;
}
