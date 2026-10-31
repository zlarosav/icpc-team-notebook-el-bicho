vector<int> v;

vector<int> lis;

for(auto &x : v) {
	auto it = lower_bound(lis.begin(), lis.end(), x);
	if (it == lis.end()) {
		lis.push_back(x);
	} else {
		*it = x;
	}
}