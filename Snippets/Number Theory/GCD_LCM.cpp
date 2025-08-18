// Maximo comun divisor (GCD): Algoritmo de Euclides
int gcd(int a, int b) { 
    if (a > b) swap(a, b);
    if (a == 0) return b;
    return gcd(b % a, a);
}
  
// Minimo comun multiplo (LCM): Calculado con GCD
    int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}