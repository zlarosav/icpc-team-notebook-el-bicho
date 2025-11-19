// ==================== Tipos base ====================
// - i64: reemplazo al long long
// - Real: para cálculos con flotantes de más precisión (distancias, intersecciones, etc.).
using i64  = long long;
using Real = long double;
constexpr Real EPS = 1e-9;

// ==================== sign / cmp ====================
//  - sign(x): devuelve -1, 0, 1 (según x < 0, x == 0, x > 0)
//  - cmp(a, b): compara a y b con tolerancia (para Real).
//    cmp(a, b) == 0 -> a "igual" a b, cmp(a, b) < 0 -> a < b, etc.
template <typename T>
int sign(T x) {
  return (x > 0) - (x < 0);
}
int sign(Real x) {
  return (x > EPS) - (x < -EPS);
}

template <typename T>
int cmp(T a, T b) {
  return sign(a - b);
}

// ==================== Punto y Línea ====================
//  - con T = i64 es usual en problemas de coordenadas enteras.
//  - P<T>: punto/vector en 2D con componentes de tipo T.
//  - L<T>: línea o segmento definido por dos puntos (l[0], l[1]).
template <typename T>
struct P {
  T x = 0, y = 0;
	P(T x = 0, T y = 0) : x(x), y(y) {}
	friend istream& operator>>(istream &is, P &p) { return is >> p.x >> p.y; }
	friend ostream& operator<<(ostream &os, P p) { return os << p.x << ' ' << p.y; }
	friend bool operator==(P a, P b) { return cmp(a.x, b.x) == 0 && cmp(a.y, b.y) == 0; }
	friend bool operator!=(P a, P b) { return !(a == b); }
  P operator-() const { return P(-x, -y); }
  P& operator+=(P a) {
    x += a.x; y += a.y;
		return *this;
	}
	P& operator-=(P a) {
		x -= a.x; y -= a.y;
		return *this;
	}
	P& operator*=(T d) {
		x *= d; y *= d;
		return *this;
	}
	P& operator/=(T d) {
		x /= d; y /= d;
		return *this;
	}
	friend P operator+(P a, P b) { return P(a) += b; }
	friend P operator-(P a, P b) { return P(a) -= b; }
	friend P operator*(P a, T d) { return P(a) *= d; }
	friend P operator/(P a, T d) { return P(a) /= d; }
  friend bool operator<(P a, P b) {
    int sx = cmp(a.x, b.x);
    return (sx != 0 ? sx == -1 : cmp(a.y, b.y) == -1);
  }
};

template <typename T>
struct L {
	array<P<T>, 2> l;
	L(P<T> a = {}, P<T> b = {}) : l{a, b} {}
};

// ==================== Operaciones vectoriales básicas ====================
//  - dot(a, b): producto escalar.
//  - cross(a, b): producto cruzado escalar (a.x * b.y - a.y * b.x).
//  - cross(p, a, b): cross(a - p, b - p) → orientación de p respecto a a→b.
//  - square(a): |a|^2.
//  - dist2(a, b): |a-b|^2, sin sqrt.
//  - length(a): |a|.
//  - dist(a, b): distancia euclidiana entre a y b.
template <typename T>
T dot(P<T> a, P<T> b) { return a.x * b.x + a.y * b.y; }
template <typename T>
T cross(P<T> a, P<T> b) { return a.x * b.y - a.y * b.x; }
template <typename T>
T cross(P<T> p, P<T> a, P<T> b) { return cross(a - p, b - p); }
template <typename T>
T square(P<T> a) { return dot(a, a); }
template <typename T>
T dist2(P<T> a, P<T> b) { return square(a - b); }
template <typename T>
Real length(P<T> a) { return sqrtl(square(a)); }
template <typename T>
Real dist(P<T> a, P<T> b) { return length(a - b); }

// ==================== Direcciones, ángulos, normales ====================
//  - normal(a): vector unitario en dirección de a.
//  - up(a): true si el vector está en el semiplano "de arriba" (para ordenar por ángulo).
//  - polar(a, b): criterio de orden por ángulo polar (para sort).
//  - parallel(a, b): vectores paralelos.
//  - sameDirection(a, b): vectores paralelos y apuntando en misma dirección.
//  - angle(p): atan2(y, x).
//  - rotate90(p): rota 90° CCW (útil para una normal rápida).
//  - rotate(p, ang): rota un vector por un ángulo ang en radianes.
template <typename T>
P<Real> normal(P<T> a) {
	Real len = length(a);
	return P<Real>(a.x / len, a.y / len);
}
template <typename T>
bool up(P<T> a) {
	return sign(a.y) > 0 || (sign(a.y) == 0 && sign(a.x) > 0);
}
// 3 colinear? recuerda remover (0,0) si lo usas en ordenamientos polares
template <typename T>
bool polar(P<T> a, P<T> b) {
	bool ua = up(a), ub = up(b);
	return ua != ub ? ua : sign(cross(a, b)) == 1;
}
template <typename T>
bool parallel(P<T> a, P<T> b) {
	return sign(cross(a, b)) == 0;
}
template <typename T>
bool sameDirection(P<T> a, P<T> b) {
  return sign(cross(a, b)) == 0 && sign(dot(a, b)) == 1;
}
template <typename T>
Real angle(P<T> p) {
  return atan2((Real)p.y, (Real)p.x);
}
template <typename T>
P<T> rotate90(P<T> p) {
  return P<T>(-p.y, p.x);
}
P<Real> rotate(P<Real> p, Real ang) {
  return P<Real>(p.x * cosl(ang) - p.y * sinl(ang),
                p.x * sinl(ang) + p.y * cosl(ang));
}

// ==================== Dirección de una línea ====================
//  - direction(l): vector l.l[1] - l.l[0] (dirección del segmento/recta).
//  - parallel(l1, l2) / sameDirection(l1, l2): igual que para vectores pero con líneas.
template <typename T>
P<T> direction(L<T> l) {
  return l.l[1] - l.l[0];
}
template <typename T>
bool parallel(L<T> l1, L<T> l2) {
  return sameDirection(direction(l1), direction(l2));
}
template <typename T>
bool sameDirection(L<T> l1, L<T> l2) {
  return sameDirection(direction(l1), direction(l2));
}

// ==================== Proyección, reflexión, distancias a recta ====================
//  - projection(p, l): proyección ortogonal de p sobre la recta (infinita) l.
//  - reflection(p, l): reflejo de p respecto a la recta l.
//  - pointToLineDist(p, l): distancia mínima de p a la recta infinita que pasa por l.
P<Real> projection(P<Real> p, L<Real> l) {
	auto d = direction(l);
	return l.l[0] + d * (dot(p - l.l[0], d) / (Real)square(d));
}
P<Real> reflection(P<Real> p, L<Real> l) {
  return projection(p, l) * 2 - p;
}
template <typename T>
Real pointToLineDist(P<T> p, L<T> l) {
	if (l.l[0] == l.l[1]) return dist(p, l.l[0]);
	return fabsl(cross(l.l[0] - l.l[1], l.l[0] - p)) / length(direction(l));
}

// ==================== Intersección de líneas (rectas infinitas) ====================
//  - lineIntersection(l1, l2): punto de intersección de las rectas infinitas
//    definidas por l1 y l2.
//  - OJO: no chequea si son paralelas; debes verificar antes que cross != 0.
template <typename T>
P<Real> lineIntersection(L<T> l1, L<T> l2) {
	auto d1 = direction(l1);
	auto d2 = direction(l2);
	auto num = (Real)cross(d2, l1.l[0] - l2.l[0]);
	auto den = (Real)cross(d2, d1);
	return P<Real>(l1.l[0]) - d1 * (num / den);
}

// ==================== Side / Between ====================
//  - side(p, a, b): orientación de p respecto al vector a→b.
//      > 0: izquierda (CCW), < 0: derecha (CW), 0: colineal.
//  - side(p, l): igual que antes pero con línea l.
//  - between(m, l, r): true si m está entre l y r (incluyendo bordes).
template <typename T>
int side(P<T> p, P<T> a, P<T> b) {
  return sign(cross(p, a, b));
}
template <typename T>
int side(P<T> p, L<T> l) {
  return side(p, l.l[0], l.l[1]);
}
template <typename T>
bool between(T m, T l, T r) {
  return cmp(l, m) == 0 || cmp(m, r) == 0 || (l < m) != (r < m);
}

// ==================== Puntos sobre segmento ====================
//  - pointOnSeg(p, l): true si p está sobre el segmento l (incluye endpoints).
//  - pointStrictlyOnSeg(p, l): true si p está sobre el segmento pero no en los endpoints.
template <typename T>
bool pointOnSeg(P<T> p, L<T> l) {
  return side(p, l) == 0 &&
				between(p.x, l.l[0].x, l.l[1].x) &&
				between(p.y, l.l[0].y, l.l[1].y);
}
template <typename T>
bool pointStrictlyOnSeg(P<T> p, L<T> l) {
  if (side(p, l) != 0) return false;
  auto d = direction(l);
  return sign(dot(p - l.l[0], d)) * sign(dot(p - l.l[1], d)) < 0;
}

// ==================== Solapamiento de intervalos ====================
//  - overlap(l1, r1, l2, r2): true si [l1, r1] y [l2, r2] se solapan (1D).
template <typename T>
bool overlap(T l1, T r1, T l2, T r2) {
  if (l1 > r1) swap(l1, r1);
  if (l2 > r2) swap(l2, r2);
  return cmp(r1, l2) != -1 && cmp(r2, l1) != -1;
}

// ==================== Intersección de segmentos / rayos ====================
//  - segIntersect(l1, l2): true si los segmentos se tocan o cortan
//    (incluye colineales solapados y tocar en vértices).
//  - segStrictlyIntersect(l1, l2): true si se cortan estrictamente
//    (no cuenta tocar solo en un endpoint).
//  - rayIntersect(l1, l2): considera l1 y l2 como rayos, intersectan "hacia adelante"
//    (no cuenta si solo coincide el origen).
template <typename T>
bool segIntersect(L<T> l1, L<T> l2) {
	auto [p1, p2] = l1.l;
	auto [q1, q2] = l2.l;
	return overlap(p1.x, p2.x, q1.x, q2.x) &&
				overlap(p1.y, p2.y, q1.y, q2.y) &&
				side(p1, l2) * side(p2, l2) <= 0 &&
				side(q1, l1) * side(q2, l1) <= 0;
}
template <typename T>
bool segStrictlyIntersect(L<T> l1, L<T> l2) {
	auto [p1, p2] = l1.l;
	auto [q1, q2] = l2.l;
	return side(p1, l2) * side(p2, l2) < 0 &&
				side(q1, l1) * side(q2, l1) < 0;
}
template <typename T>
bool rayIntersect(L<T> l1, L<T> l2) {
	auto v1 = direction(l1);
	auto v2 = direction(l2);
	int x = sign(cross(v1, v2));
	return x != 0 && side(l1.l[0], l2) == x && side(l2.l[0], l1) == -x;
}

// ==================== Distancias punto-segmento / segmento-segmento ====================
//  - pointToSegDist(p, l): distancia mínima de p al segmento l.
//  - segDist(l1, l2): distancia mínima entre dos segmentos (0 si se intersectan).
template <typename T>
Real pointToSegDist(P<T> p, L<T> l) {
	auto d = direction(l);
	if (sign(dot(p - l.l[0], d)) >= 0 && sign(dot(p - l.l[1], d)) <= 0) {
		return pointToLineDist(p, l);
	} else {
		return min(dist(p, l.l[0]), dist(p, l.l[1]));
	}
}
template <typename T>
Real segDist(L<T> l1, L<T> l2) {
	if (segIntersect(l1, l2)) return 0;
	return min({
		pointToSegDist(l1.l[0], l2),
		pointToSegDist(l1.l[1], l2),
		pointToSegDist(l2.l[0], l1),
		pointToSegDist(l2.l[1], l1)
	});
}

// ==================== Área de polígono y punto en polígono ====================
//  - area(a): devuelve 2 * área con signo del polígono a (ordenado).
//      >0 → CCW, <0 → CW, abs(area)/2.0 → área real.
//  - pointInPoly(p, a): true si p está dentro o sobre el borde del polígono a
//      (no necesariamente convexo).
template <typename T>
T area(vector<P<T>> a) {
	T res = 0;
	int n = (int)a.size();
	for (int i = 0; i < n; i++) {
		res += cross(a[i], a[(i + 1) % n]);
	}
	return res;
}
template <typename T>
bool pointInPoly(P<T> p, vector<P<T>> a) {
	int n = (int)a.size(), res = 0;
	for (int i = 0; i < n; i++) {
		P<T> u = a[i], v = a[(i + 1) % n];
		if (pointOnSeg(p, L<T>(u, v))) return true;
		if (cmp(u.y, v.y) <= 0) swap(u, v);
		if (cmp(p.y, u.y) > 0 || cmp(p.y, v.y) <= 0) continue;
		res ^= cross(p, u, v) > 0;
	}
	return res;
}

// ==================== Aliases finales ====================
//  - Point  = P<i64>   → puntos con coordenadas enteras.
//  - Line   = L<i64>   → segmentos/líneas con endpoints enteros.
//  - Usa Real (long double) para distancias si necesitas precisión extra.
using Point = P<i64>;
using Line  = L<i64>;

// ejemplo de uso rápido:
// Point a, b; cin >> a >> b;
// Line seg(a, b);
// if (segIntersect(seg, Line(Point(0,0), Point(10,0)))) { ... }