// "These works to find a componente fuertemente conexa that it's in directed graph"
struct SCC{
	int N = 0, id;
	vector<vector<int>> adj;
	vector<int> ind, low;
	stack<int> s;
	vector<bool> in_stack;
	vector<vector<int>> components;
	vector<int> component_id;

	//1-indexed
	SCC(int n = 0){ N = n + 1, adj.assign(N, {}); }
	SCC(const vector<vector<int>> & _adj){ adj = _adj, N = adj.size(); }

	void add_edge(int from, int to){
		adj[from].push_back(to);
	}

	void dfs(int u){
		low[u] = ind[u] = id++;
		s.push(u);
		in_stack[u] = true;
		for(int v : adj[u]){
			if(ind[v] == -1){
				dfs(v);
				low[u] = min(low[u], low[v]);
			}else if(in_stack[v]){
				low[u] = min(low[u], ind[v]);
			}
		}
		if(low[u] == ind[u]){
			components.emplace_back();
			vector<int> & comp = components.back();
			while(true){
				assert(!s.empty());
				int x = s.top(); s.pop();
				in_stack[x] = false;
				component_id[x] = components.size() - 1;
				comp.push_back(x);
				if(x == u) break;
			}
		}
	}

	vector<vector<int>> get(){
		ind.assign(N, - 1); low.assign(N, -1); component_id.assign(N, -1);
		s = stack<int>();
		in_stack.assign(N, false);
		id = 0;
		components = {};
		for(int i = 1; i < N; i++)
			if(ind[i] == -1) dfs(i);

		// reverse(components.begin(), components.end()); return components; // SCC in topological order
		return components; // SCC in reverse topological order
	}
};