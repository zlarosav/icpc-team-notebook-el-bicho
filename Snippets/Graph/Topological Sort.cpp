vector<int> top_sort(vector<vector<int>>& adj){
  int n = adj.size();
  bool cycle = false;
  vector<int> sorted, color(n);
  function<void(int)> dfs = [&](int u){
    color[u] = 1;
    for(int v : adj[u]){
      if(color[v] == 0 && !cycle) dfs(v);
      else if(color[v] == 1) cycle = true;
    }
    color[u] = 2;
    sorted.push_back(u);
  };
  for(int i = 1; i < n; i++){
    if(color[i] == 0 && !cycle) dfs(i);
  }
  if(cycle){return {};}
  reverse(sorted.begin(), sorted.end());
  return sorted;
}