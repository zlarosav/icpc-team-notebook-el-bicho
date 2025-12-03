// O(N + M). 0-indexed. Retorna cualquier toposort válido (no necesariamente lexicográficamente mínima)
vector<int> top_sort(vector<vector<int>>& adj){
    int n = adj.size();
    bool cycle = false;
    vector<int> sorted, color(n); // 0 = no visitado, 1 = visitando, 2 = terminado

    function<void(int)> dfs = [&](int u){
        color[u] = 1;
        for (int v : adj[u]){
            if (color[v] == 0 && !cycle) dfs(v);
            else if (color[v] == 1) cycle = true; // ciclo detectado
        }
        color[u] = 2;
        sorted.push_back(u);
    };

    for (int i = 0; i < n; i++){
        if (color[i] == 0 && !cycle) dfs(i);
    }
    if (cycle) return {}; // no existe toposort
    reverse(sorted.begin(), sorted.end());
    return sorted;
}
