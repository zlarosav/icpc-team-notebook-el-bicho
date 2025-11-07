struct FenwickTree {
  vector<long long> tree;
  int n;
  
  FenwickTree(int size) : n(size + 1) {
    tree.assign(n, 0);
  }
  
  void update(int idx, long long delta) {
    for (idx++; idx < n; idx += idx & -idx) {
      tree[idx] += delta;
    }
  }
  
  long long query(int idx) {
    long long sum = 0;
    for (idx++; idx > 0; idx -= idx & -idx) {
      sum += tree[idx];
    }
    return sum;
  }
  
  long long range_query(int l, int r) {
    return query(r) - query(l - 1);
  }
};

// uso:
// int n; cin >> n;
// FenwickTree ft(n);
// for (int i = 0; i < n; i++) {
//   long long x; cin >> x;
//   ft.update(i, x);
// }
// ft.update(idx, delta); // actualizar elemento en idx
// long long sum = ft.range_query(l, r); // suma en rango [l, r]

