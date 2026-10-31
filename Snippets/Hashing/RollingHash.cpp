vector<long long> pref1,pref2;
vector<long long> pot1,pot2;
const long long MOD1 = 1e9 + 7;
const long long MOD2 = 1e9 + 9;
const long long B = 31;
int n; string s;
 
void precompute() {
	pref1.assign(n+1,0);
	pref2.assign(n+1,0);
	pot1.assign(n+1,1);
	pot2.assign(n+1,1);
 
	for(int i = 1; i < n; i++) {
		pot1[i] = (pot1[i-1]*B) % MOD1;
		pot2[i] = (pot2[i-1]*B) % MOD2;
	}
	
 
	for(int i = 0; i < n; i++) {
		long long val = s[i] - 'a' + 1;
		pref1[i+1] = (pref1[i]*B + val) % MOD1; 
		pref2[i+1] = (pref2[i]*B + val) % MOD2; 
	}
}
 
pair<long long, long long> getHash(int l, int r) {
	long long h1 = (pref1[r] - pref1[l]*pot1[r-l] % MOD1 + MOD1) % MOD1;
	long long h2 = (pref2[r] - pref2[l]*pot2[r-l] % MOD2 + MOD2) % MOD2;
	return {h1,h2};
}


//Ejemplo de uso para hallar todos los substrings para precesarlos en O(1)
void solve() {
	cin>>s; n = s.size();
	precompute();

	for(int l = 2; l <= n; l++) {
		map<pair<long long, long long>, vector<int>> mp;
 
		for(int i = 0; i + l <= n; i++) {
			mp[getHash(i,i + l)].push_back(i);
		}
	}
}