int n, k; cin >> n >> k;
vector<int> a(n);
for (int i = 0; i < n; i++) cin >> a[i];

// ventana deslizante de tamaño k
deque<int> dq;
for (int i = 0; i < n; i++) {
  while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
  while (!dq.empty() && a[dq.back()] <= a[i]) dq.pop_back();
  dq.push_back(i);
  if (i >= k - 1) {
    // a[dq.front()] es el máximo en ventana [i-k+1, i]
  }
}

// mínimo en ventana de tamaño k
dq.clear();
for (int i = 0; i < n; i++) {
  while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
  while (!dq.empty() && a[dq.back()] >= a[i]) dq.pop_back();
  dq.push_back(i);
  if (i >= k - 1) {
    // a[dq.front()] es el mínimo en ventana [i-k+1, i]
  }
}

