struct Trie {
  struct Node {
    vector<int> next;
    bool is_end;
    int count;
    Node() : next(26, -1), is_end(false), count(0) {}
  };
  vector<Node> nodes;
  Trie() { nodes.emplace_back(); }
  
  void insert(string& s) {
    int cur = 0;
    for (char c : s) {
      int idx = c - 'a';
      if (nodes[cur].next[idx] == -1) {
        nodes[cur].next[idx] = nodes.size();
        nodes.emplace_back();
      }
      cur = nodes[cur].next[idx];
      nodes[cur].count++;
    }
    nodes[cur].is_end = true;
  }
  
  bool search(string& s) {
    int cur = 0;
    for (char c : s) {
      int idx = c - 'a';
      if (nodes[cur].next[idx] == -1) return false;
      cur = nodes[cur].next[idx];
    }
    return nodes[cur].is_end;
  }
  
  int count_prefix(string& s) {
    int cur = 0;
    for (char c : s) {
      int idx = c - 'a';
      if (nodes[cur].next[idx] == -1) return 0;
      cur = nodes[cur].next[idx];
    }
    return nodes[cur].count;
  }
};

// uso:
// Trie trie;
// int n; cin >> n;
// for (int i = 0; i < n; i++) {
//   string s; cin >> s;
//   trie.insert(s);
// }
// string query; cin >> query;
// bool exists = trie.search(query); // true si query existe en el trie
// int count = trie.count_prefix(query); // cantidad de strings que tienen query como prefijo

Trie trie;
trie.insert("hello");
trie.insert("hell");
bool found = trie.search("hello"); // true
int count = trie.count_prefix("hel"); // 2 (hello y hell)