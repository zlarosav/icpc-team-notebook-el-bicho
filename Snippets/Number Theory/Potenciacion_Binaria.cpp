using ll = long long;
const int MAXN = 1e6;   // límite superior de n
const ll MOD = 1e9 + 7; // primo grande

// Potenciacion binaria modular a^b mod p
ll binpow(ll a, ll b, ll m = MOD) {
    a %= m;
    ll res = 1;
    while (b > 0) {
        if (b & 1)
            res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}