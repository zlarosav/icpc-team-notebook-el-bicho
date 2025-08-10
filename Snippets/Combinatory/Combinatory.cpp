vector<mint> inverse, fact, inv_fact;

void generateBC(int N = 1e5){
	const int mod = mint().MOD;

	inverse.resize(N + 1); fact.resize(N + 1); inv_fact.resize(N + 1);
	inverse[1] = 1;

	for(int i = 2; i <= N; i++)
		inverse[i] = mod - (mod / i * inverse[mod % i]);

	fact[0] = inv_fact[0] = 1;
	for(int i = 1; i <= N; i++){
		fact[i] = fact[i - 1] * mint(i);
		inv_fact[i] = inv_fact[i - 1] * inverse[i];
	}
};

mint C(int n, int k){
	if(k > n) return mint(0);
	assert(n < fact.size() && k < fact.size());
	return fact[n] * inv_fact[k] * inv_fact[n - k];
}