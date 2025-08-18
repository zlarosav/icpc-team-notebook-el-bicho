// nCk brute force sin MOD n <= 20
long long nCk_bruteforce(long long n, long long k) {
    if (k < 0 || k > n) return 0;
    long long res = 1;
    for (long long i = 1; i <= k; i++) {
        res = res * (n - i + 1) / i; // aquí la división es exacta
    }
    return res;
}

// nPk brute force sin MOD n <= 20
long long nPk_bruteforce(long long n, long long k) {
    if (k < 0 || k > n) return 0;
    long long res = 1;
    for (long long i = 0; i < k; i++) {
        res *= (n - i);
    }
    return res;
}