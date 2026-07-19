template <typename T>
struct FenwickTree{
  vector<T> bit;
  int n;
  FenwickTree(int n) : bit(n, 0), n(n) {}

  FenwickTree(const vector<T>& a) : bit(int(a.size()), 0), n(a.size()){
    for(int i = 0; i < n; i++){
      update(i, a[i]);
    }
  }

  void update(int idx, T delta){
    for(; idx < n; idx = idx | (idx + 1))
      bit[idx] += delta;
  }

  T query(int r){ // summation in range of 0 to i
    T ret = 0;
    for(; r >= 0; r = (r & (r + 1)) - 1)
      ret += bit[r];
    return ret;
  }

  T query(int l, int r){
    return query(r) - query(l - 1);
  }
};

