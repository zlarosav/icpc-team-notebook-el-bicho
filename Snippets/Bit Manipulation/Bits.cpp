using ull = unsigned long long;
const ull UNSIGNED_LL_MAX = 18'446'744'073'709'551'615;
// Verifica si S es potencia de dos (y distinto de cero)
#define isPowerOfTwo(S) ((S) && !((S) & ((S) - 1))) 
// Retorna la potencia de dos más cercana a S
#define nearestPowerOfTwo(S) (1LL << lround(log2(S))) 
// Calcula S % N cuando N es potencia de dos
#define modulo(S, N) ((S) & ((N) - 1)) 

// Verifica si el bit está encendido (bit en 1)
#define isOn(S, i) ((S) & (1LL<<(i))) 
// Enciende el bit (Lo pone en 1)
#define setBit(S, i) ((S) |= (1LL<<(i))) 
// Apaga el bit (Lo pone en 0)
#define clearBit(S, i) ((S) &= ~(1LL<<(i))) 
// Invierte el estado del bit (0 <-> 1)
#define toggleBit(S, i) ((S) ^= (1LL<<(i))) 
// Enciende los primeros 'n' bits (idx-0)
#define setAll(S, n) ((S) = ((n)>=64 ? ~0LL : (1LL << (n))-1)) 

// Extrae el bit menos significativo 0100 (Least Significant Bit)
#define lsb(S) ((S) & -(S)) 
// Número de ceros a la derecha (Posición del LSB, idx-0)
#define idxLastBit(x) __builtin_ctzll(x) 
// Extrae el bit màs significativo 0100 (Most Significant Bit)
#define msb(S) (1LL << (63 - __builtin_clzll(S))) 
// Posición del MSB (63 - ceros a la izquierda, idx-0)
#define idxFirstBit(x) (63 - __builtin_clzll(x)) 

#define countAllOnes(x) __builtin_popcountll(x)
// Apaga el último bit encendido (el menos significativo)
#define turnOffLastBit(S) ((S) & ((S) - 1)) 
// Enciende el último cero menos significativo
#define turnOnLastZero(S) ((S) | ((S) + 1)) 
// Apaga todos los bits encendidos más a la derecha consecutivos
#define turnOffLastConsecutiveBits(S) ((S) & ((S) + 1)) 
// Enciende los ceros consecutivos más a la derecha
#define turnOnLastConsecutiveZeroes(S) ((S) | ((S) - 1)) 

// Máscara de bits (mask -> subconjunto) O(2^N)
for (int mask = 0; mask < (1 << N); mask++)

// Recorrer subconjuntos de un superconjunto (menos el vacío)
int b = 0b1011; // Representación binaria de un decimal en int
for (int i = b; i; i = (i - 1) & b) {
  cout << bitset<4>(i) << "\n";
}

void printBin(ll x) {
  // 63 -> unsigned ll, 62 -> ll, 31 -> unsigned int, 30 -> int
  for (ll i = 63; i >= 0; i--)
    cout << ((x >> i) & 1);
  cout << '\n';
}