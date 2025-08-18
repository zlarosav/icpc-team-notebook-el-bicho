// Devuelve el inverso modular de a mod MOD
// Usa el Teorema Pequeño de Fermat: a^(MOD-2) === a^(-1) (mod MOD)
// (válido solo si MOD es primo)
ll inv(ll a, ll p = MOD) {
    return binpow(a, p - 2, p);
}

// Factoriales e inversos factoriales precomputados
// fact[n]  = n! mod MOD
// invf[n]  = (n!)^(-1) mod MOD
// Precomputa en O(n)
vector<ll> fact(MAXN + 1), invf(MAXN + 1);

void precompute_factorials() {
    fact[0] = 1;
    for (int i = 1; i <= MAXN; i++) {
        fact[i] = fact[i - 1] * i % MOD;
    }
    invf[MAXN] = inv(fact[MAXN]);
    for (int i = MAXN; i > 0; i--) {
        invf[i - 1] = invf[i] * i % MOD;
    }
}

// Combinatoria de n en k: nCk(n, k) para n <= 10^6
// "n choose k" = n! / (k! * (n-k)!) mod MOD
// Retorna 0 si k > n
ll nCk(ll n, ll k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * invf[k] % MOD * invf[n - k] % MOD;
}

// Permutación de n en k: nPk(n, k) para n <= 10^6
// Calcula permutaciones: "n permute k" = n! / (n-k)! mod MOD
// Retorna 0 si k > n
ll nPk(ll n, ll k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * invf[n - k] % MOD;
}