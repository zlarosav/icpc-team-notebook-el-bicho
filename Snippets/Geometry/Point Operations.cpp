struct Point {
  long long x, y;
  Point(long long x = 0, long long y = 0) : x(x), y(y) {}
  Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
  Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
  Point operator*(long long k) const { return Point(x * k, y * k); }
  long long dot(const Point& p) const { return x * p.x + y * p.y; }
  long long cross(const Point& p) const { return x * p.y - y * p.x; }
  long long cross(const Point& a, const Point& b) const { return (a - *this).cross(b - *this); }
  long long norm2() const { return x * x + y * y; }
  double norm() const { return sqrt(norm2()); }
  bool operator<(const Point& p) const { return x < p.x || (x == p.x && y < p.y); }
  bool operator==(const Point& p) const { return x == p.x && y == p.y; }
};

long long orientation(Point a, Point b, Point c) {
  return (b - a).cross(c - a);
}

bool collinear(Point a, Point b, Point c) {
  return orientation(a, b, c) == 0;
}

bool cw(Point a, Point b, Point c) {
  return orientation(a, b, c) < 0;
}

bool ccw(Point a, Point b, Point c) {
  return orientation(a, b, c) > 0;
}

// uso:
// Point p1(1, 2), p2(3, 4), p3(5, 6);
// long long area = abs((p2 - p1).cross(p3 - p1)) / 2; // area del triangulo
// bool clockwise = cw(p1, p2, p3); // true si p1->p2->p3 es clockwise
// bool counter_clockwise = ccw(p1, p2, p3); // true si p1->p2->p3 es counter-clockwise

