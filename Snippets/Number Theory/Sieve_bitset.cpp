// Hasta N = 10^8 aprox en 1s
const int MAX_V = 1e7 + 5;
bitset<MAX_V> composite;
void sieve() {
    composite[0] = composite[1] = true;
    for (int i = 2; i * i < MAX_V; i++) {
        if (composite[i]) continue;
        for (int j = i * i; j < MAX_V; j += i) {
            composite[j] = true;
        }
    }
}

int main() {
    sieve();
    for (int i = 2; i < 100; i++) {
        cout << i << " is_primes : " << !composite[i] << '\n';
    }
}