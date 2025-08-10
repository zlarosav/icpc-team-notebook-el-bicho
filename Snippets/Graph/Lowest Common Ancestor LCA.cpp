struct LCA{
	int n, l, timer = 0;
	vector<vector<int>> up, adj;
	vector<int> depth, in, out;
 
	LCA(int _n) {
		n = _n + 1;
		l = ceil(log2(n));
		up.resize(n, vector<int>(l + 1));
		adj.resize(n);
		depth.resize(n);
		in.resize(n);
		out.resize(n);
	}
 
	void add_edge(int p, int u){
		adj[p].push_back(u);
		adj[u].push_back(p);
	}
 
	void dfs(int u = 1, int p = 1){
		up[u][0] = p;
		depth[u] = depth[p] + 1;
		in[u] = ++timer;
		for(int level = 1; level <= l; level++){
			up[u][level] = up[up[u][level - 1]][level - 1];
		}
		for(int v : adj[u]){
			if(v == p) continue;
			dfs(v, u);
		}
		out[u] = ++timer;
	}
 
	bool is_ancestor(int p, int u){
		return in[p] <= in[u] && out[p] >= out[u];
	}

	int query(int u, int v){
		if(is_ancestor(u, v)) return u;
		if(is_ancestor(v, u)) return v;

		for(int bit = l; bit >= 0; bit--){
			if(is_ancestor(up[u][bit], v)) continue;
			u = up[u][bit];
		}
		return up[u][0];
	}

	int ancestor(int u, int k){
		if(depth[u] <= k) return -1;
		for(int bit = 0; bit <= l; bit++){
			if(k >> bit & 1) u = up[u][bit];
		}
		return u;
	}

	int distance(int u, int v){
		return depth[u] + depth[v] - 2 * depth[query(u, v)];
	}
};