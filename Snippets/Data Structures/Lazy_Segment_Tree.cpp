template <typename T, typename L, typename MergeOp, typename ApplyOp, typename MergeLazy>
class lazy_segment_tree{
private:
	int n;
	vector<T> tree;
	vector<L> lazy;
	MergeOp merge_op;
	ApplyOp apply_op;
	MergeLazy merge_lazy;
	T identity_value;
	L identity_lazy;

	void build(const vector<T>& a, int node, int l, int r){
		if(l == r){ tree[node] = a[l]; return; }

		int m = (l + r) >> 1;
		int x = node + 1, y = node + 2 * (m - l + 1);
		build(a, x, l, m);
		build(a, y, m + 1, r);
		tree[node] = merge_op(tree[x], tree[y]);
	}

	void push(int node, int l, int r){
		if(lazy[node] != identity_lazy){
			tree[node] = apply_op(tree[node], lazy[node], l, r);
			if(l != r){
				int m = (l + r) >> 1;
				int x = node + 1, y = node + 2 * (m - l + 1);
				lazy[x] = merge_lazy(lazy[x], lazy[node]);
				lazy[y] = merge_lazy(lazy[y], lazy[node]);
			}
			lazy[node] = identity_lazy;
		}
	}

	T query(int node, int l, int r, int ql, int qr){
		push(node, l, r);
		if(qr < l || r < ql) return identity_value;
		if(ql <= l && r <= qr) return tree[node];

		int m = (l + r) >> 1;
		int x = node + 1, y = node + 2 * (m - l + 1);
		T left = query(x, l, m, ql, qr);
		T right = query(y, m + 1, r, ql, qr);
		return merge_op(left, right);
	}

	void update(int node, int l, int r, int ql, int qr, L val){
		push(node, l, r);
		if(qr < l || r < ql) return;
		if(ql <= l && r <= qr){
			lazy[node] = merge_lazy(lazy[node], val);
			push(node, l, r);
			return;
		}

		int m = (l + r) >> 1;
		int x = node + 1, y = node + 2 * (m - l + 1);
		update(x, l, m, ql, qr, val);
		update(y, m + 1, r, ql, qr, val);
		tree[node] = merge_op(tree[x], tree[y]);
	}

public:
	lazy_segment_tree(const vector<T>& a, MergeOp merge_op, ApplyOp apply_op, MergeLazy merge_lazy, T identity_value, L identity_lazy)
	: n((int) a.size()), merge_op(merge_op), apply_op(apply_op), merge_lazy(merge_lazy), identity_value(identity_value), identity_lazy(identity_lazy) {
		tree.resize(2 * n, identity_value);
		lazy.resize(2 * n, identity_lazy);
		build(a, 1, 0, n - 1);
	}

	T query(int l, int r){
		return query(1, 0, n - 1, l, r);
	}

	void update(int l, int r, L val){
		update(1, 0, n - 1, l, r, val);
	}
};

/*
Definir las variables dentro de la funcion solve o main:
	using T = ;
	using L = ;

	auto merge_op = [](T a, T b) { return ; };
	auto apply_op = [](T a, L b, int l, int r) { return ; };
	auto merge_lazy = [](L a, L b) { return ; };
	T identity_value = ;
	L identity_lazy = ;

	vector<T> values;
	// ...

	lazy_segment_tree<T, L, decltype(merge_op), decltype(apply_op), decltype(merge_lazy)> 
		lst(values, merge_op, apply_op, merge_lazy, identity_value, identity_lazy);
*/ 