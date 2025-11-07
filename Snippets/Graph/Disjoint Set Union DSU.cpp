struct DSU {
	vector<int> p, size;
	DSU(int n){
		p.resize(n + 1), size.resize(n + 1,1);
		for(int i = 1; i <= n; i++) p[i] = i;
	}

	int find(int x){
		if(p[x] != x) p[x] = find(p[x]);
		return p[x];
	}

	void merge(int x, int y){
		x = find(x), y = find(y);
		if(x == y) return;
		if(size[x] < size[y]) swap(x, y);
		size[x] += size[y];
		p[y] = x;
	}
};