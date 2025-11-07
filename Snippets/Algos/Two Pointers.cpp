int n, target; cin >> n >> target;
vector<int> a(n);
for (int i = 0; i < n; i++) cin >> a[i];

// encontrar subarray con suma = target
int l = 0, sum = 0;
for (int r = 0; r < n; r++) {
  sum += a[r];
  while (sum > target && l <= r) {
    sum -= a[l++];
  }
  if (sum == target) {
    // subarray [l, r] tiene suma = target
  }
}

// encontrar número de subarrays con suma <= target
l = 0, sum = 0;
long long count = 0;
for (int r = 0; r < n; r++) {
  sum += a[r];
  while (sum > target && l <= r) {
    sum -= a[l++];
  }
  count += r - l + 1;
}
// count = número de subarrays con suma <= target

