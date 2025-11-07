// binary search en array ordenado
int n, target; cin >> n >> target;
vector<int> a(n);
for (int i = 0; i < n; i++) cin >> a[i];

// encontrar primera posición >= target
int l = 0, r = n - 1, first_pos = n;
while (l <= r) {
  int mid = (l + r) / 2;
  if (a[mid] >= target) {
    first_pos = mid;
    r = mid - 1;
  } else {
    l = mid + 1;
  }
}

// encontrar última posición <= target
l = 0, r = n - 1;
int last_pos = -1;
while (l <= r) {
  int mid = (l + r) / 2;
  if (a[mid] <= target) {
    last_pos = mid;
    l = mid + 1;
  } else {
    r = mid - 1;
  }
}

// binary search en función monótona
function<bool(int)> check = [&](int x) {
  return true; // condición
};
l = 0, r = 1e9;
int ans = -1;
while (l <= r) {
  int mid = (l + r) / 2;
  if (check(mid)) {
    ans = mid;
    l = mid + 1; // o r = mid - 1 dependiendo del problema
  } else {
    r = mid - 1; // o l = mid + 1
  }
}

