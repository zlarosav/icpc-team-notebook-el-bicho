template<typename T>
struct segment_tree{
	int N;
	T Z = 0;
	vector<T> tree;
	segment_tree(int N) : N(N) {
		tree.resize(2 * N);
	}

	segment_tree(vector<T>& A){
		N = (int) A.size();
		tree.resize(2 * N);
		build(A, 1, 0, N - 1);
	}

	auto& operator[](size_t i) { return tree[i]; } // this funciton works for get element int this position
private:

	T op(T& a, T& b){ return a + b; }
	// O (n)
	void build(vector<T>& values, int node, int l, int r){
		// if l and r are equal both are leaf node
		// left node = [l, m]
		// m = (l + r) / 2
		// left and right are nodes
		// left interval = [l, m], right intervla = [m + 1, r]
		// after complete fill nodes of left and right, we need to fill the [l, r] node
		if(l == r){
			tree[node] = values[l];
			return;
		}
		int m = (l + r) >> 1;
		int left = node + 1;
		int right = node + 2 * (m - l + 1);

		build(values, left, l, m);
		build(values, right, m + 1, r);
		
		tree[node] = op(tree[left], tree[right]);
	}
	
	// O (log N)
	void modify(int pos, T value, int node,  int l,  int r){
		// if l and r are equal, we found our node and update it
		if(l == r){
			tree[node] = value;
			return;
		}
		int m = (l + r) >> 1; // we get the mid
		int left = node + 1;
		int right = node + 2 * (m - l + 1);

		if(pos <= m) modify(pos, value, left, l, m);
		else modify(pos, value, right, m + 1, r);

		tree[node] = op(tree[left], tree[right]);
	}

	void update(int pos, T value, int node,  int l,  int r){
		// if l and r are equal, we found our node and update it
		if(l == r){
			tree[node] = op(tree[node], value);
			return;
		}
		int m = (l + r) >> 1; // we get the mid
		int left = node + 1;
		int right = node + 2 * (m - l + 1);

		if(pos <= m) update(pos, value, left, l, m);
		else update(pos, value, right, m + 1, r);

		tree[node] = op(tree[left], tree[right]);
	}

	// O(log N)
	T query(int ql, int qr, int node, int l, int r){
		if(r < ql || l > qr) return Z; // CHECK
		if(ql <= l && r <= qr) return tree[node];
		int m = (l + r) >> 1;
		int left = node + 1;
		int right = node + 2 * (m - l + 1);
		T ansL = query(ql, qr, left, l, m);
		T ansR = query(ql, qr, right, m + 1, r);
		return op(ansL, ansR);
	}
public:
	void build(vector<T>& values){ build(values, 1, 0, N - 1); }

	void modify(int pos, T value){ modify(pos, value, 1, 0, N - 1); }

	void update(int pos, T value){ update(pos, value, 1, 0, N - 1); }

	T query(int ql, int qr){ return query(ql, qr, 1, 0, N - 1); }
};