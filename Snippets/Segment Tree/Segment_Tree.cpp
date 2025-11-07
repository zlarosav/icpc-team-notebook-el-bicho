// "This segment_tree I understand better how it works"
template<typename T>
struct seg_tree {
    int N;
    T Z = 0;
    vector<T> tree;
    
    seg_tree(int N) : N(N) {
        tree.resize(4 * N); 
    }
    
    seg_tree(vector<T>& A) {
        N = (int)A.size();
        tree.resize(4 * N);
        build(A, 1, 0, N-1);
    }
    
private:
    T op(T a, T b) {
        return a + b;
    }
    
    void build(vector<T>& a, int node, int left, int right) {
        if(left == right) {
            tree[node] = a[left];
            return;
        }
        int mid = (left + right) >> 1;
        build(a, 2 * node, left, mid);
        build(a, 2 * node + 1, mid + 1, right);
        tree[node] = op(tree[2 * node], tree[2 * node + 1]);
    }
    
    void modify(int pos, T value, int node, int left, int right) {
        if(left == right) {
            tree[node] = value;
            return;
        }
        int mid = (left + right) >> 1;
        if(pos <= mid) 
            modify(pos, value, 2 * node, left, mid);
        else 
            modify(pos, value, 2 * node + 1, mid + 1, right);
        tree[node] = op(tree[2 * node], tree[2 * node + 1]);
    }
    
    T query(int l, int r, int node, int left, int right) {
        if(r < left || l > right) return Z;
        if(l <= left && right <= r) return tree[node];
        int mid = (left + right) >> 1;
        T leftSum = query(l, r, 2 * node, left, mid);
        T rightSum = query(l, r, 2 * node + 1, mid + 1, right);
        return op(leftSum, rightSum);
    }
    
public:
    void build(vector<T>& a) { build(a, 1, 0, N-1); }
    void modify(int pos, T value) { modify(pos, value, 1, 0, N-1); }
    T query(int l, int r) { return query(l, r, 1, 0, N-1); }
};