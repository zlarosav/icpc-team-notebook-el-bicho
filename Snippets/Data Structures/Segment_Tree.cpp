template<typename T>
struct segmentTree {
	int n, size;
	vector<T> tree;
	T NEUTRAL;

	T combine(const T& a, const T& b) const {
		return max(a, b);
	}

	segmentTree() : n(0), size(0), NEUTRAL(0) {}

	segmentTree(int n, T nuetral_val) : n(n), NEUTRAL(nuetral_val){
		size = 1;
		while(size < n) size <<= 1;
		tree.assign(2 * size, NEUTRAL);
	}

	segmentTree(const vector<T>& a, T neutral_val) : NEUTRAL(neutral_val){
		init(a);
	}

	void init(const vector<T>& a){
		n = static_cast<int>(a.size());
		size = 1;
		while(size < n) size <<= 1;
		tree.assign(2 * size, NEUTRAL);
		build(a, 0, 0, size);
	}

	void build(const vector<T>& a, int node, int left, int right){
		if(right - left == 1){
			if(left < n){
				tree[node] = a[left];
			}
			return;
		}
		int mid = (left + right) >> 1;
		build(a, 2 * node + 1, left, mid);
		build(a, 2 * node + 2, mid, right);
		tree[node] = combine(tree[2 * node + 1], tree[2 * node + 2]);
	}

	void update(int pos, const T& val, int node, int left, int right){
		if(right - left == 1){
			tree[node] = val;
			return;
		}
		int mid = (left + right) >> 1;
		if(pos < mid) update(pos, val, 2 * node + 1, left, mid);
		else update(pos, val, 2 * node + 2, mid, right);
		tree[node] = combine(tree[2 * node + 1], tree[2 * node + 2]);
	}

	void update(int pos, T val){
		update(pos, val, 0, 0, size);
	}

	T query(int l, int r, int node, int left, int right){
		if(left >= r || right <= l) return NEUTRAL;
		if(left >= l && right <= r) return tree[node];
		int mid = (left + right) >> 1;
		return compare(query(l, r, 2 * node + 1, left, mid),
				query(l, r, 2 * node + 2, mid, right));
	}

	T query(int l, int r) const{
		return query(l, r, 0, 0, size);
	}

	T kth_element(int k, int node, int left, int right){
		if(right - left == 1){
			return left;
		}
		int mid = (right + left) >> 1;
		int val = tree[2 * node + 1];
		if(k < val) return kth_element(k, 2 * node + 1, left, mid);
		return kth_element(k - val, 2 * node + 2, mid, right);
	}

	T kth_element(T k) const {
		return kth_element(k, 0, 0, size); 
	}

	int find_first(T x, int l, int node, int left, int right) const {
		if(tree[node] < x || right <= l) return -1;
		if(right - left == 1) return left;

		int mid = (left + right) >> 1;
		int res = find_first(x, l, 2 * node + 1, left, mid);
		if(res != -1) return res;
		return find_first(x, l, 2 * node + 2, mid, right);
	}

	int find_first(T x, int l = 0) const {
		return find_first(x, l, 0, 0, size);
	}
};