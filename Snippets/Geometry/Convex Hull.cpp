struct Point {
  long long x, y;
  Point(long long x = 0, long long y = 0) : x(x), y(y) {}
  Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
  long long cross(const Point& p) const { return x * p.y - y * p.x; }
  long long cross(const Point& a, const Point& b) const { return (a - *this).cross(b - *this); }
  bool operator<(const Point& p) const { return x < p.x || (x == p.x && y < p.y); }
  bool operator==(const Point& p) const { return x == p.x && y == p.y; }
};

vector<Point> convex_hull(vector<Point>& points) {
  int n = points.size();
  if (n <= 1) return points;
  sort(points.begin(), points.end());
  vector<Point> hull;
  for (int i = 0; i < n; i++) {
    // Si quiero incluir coords colineales ( < 0), si no lo quiero ( <= 0)
    while (hull.size() >= 2 && hull[hull.size() - 2].cross(hull.back(), points[i]) <= 0) {
      hull.pop_back();
    }
    hull.push_back(points[i]);
  }
  int lower = hull.size();
  for (int i = n - 2; i >= 0; i--) {
    // Si quiero incluir coords colineales ( < 0), si no lo quiero ( <= 0)
    while (hull.size() > lower && hull[hull.size() - 2].cross(hull.back(), points[i]) <= 0) {
      hull.pop_back();
    }
    hull.push_back(points[i]);
  }
  hull.pop_back();
  return hull;
}

// uso:
// int n; cin >> n;
// vector<Point> points(n);
// for (int i = 0; i < n; i++) {
//   cin >> points[i].x >> points[i].y;
// }
// vector<Point> hull = convex_hull(points);
// hull contiene los puntos del convex hull en orden counter-clockwise (CCW / Antihorario)
// reverse(hull) para obtenerlo en orden CW / horario