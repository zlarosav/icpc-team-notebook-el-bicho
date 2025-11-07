string s; cin >> s;
int n = s.size();
vector<int> z(n);
int l = 0, r = 0;
for (int i = 1; i < n; i++) {
  if (i <= r) {
    z[i] = min(r - i + 1, z[i - l]);
  }
  while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
    z[i]++;
  }
  if (i + z[i] - 1 > r) {
    l = i;
    r = i + z[i] - 1;
  }
}

string pattern, text;
cin >> pattern >> text;
string combined = pattern + "#" + text;
vector<int> z_combined(combined.size());
int l_combined = 0, r_combined = 0;
for (int i = 1; i < combined.size(); i++) {
  if (i <= r_combined) {
    z_combined[i] = min(r_combined - i + 1, z_combined[i - l_combined]);
  }
  while (i + z_combined[i] < combined.size() && combined[z_combined[i]] == combined[i + z_combined[i]]) {
    z_combined[i]++;
  }
  if (i + z_combined[i] - 1 > r_combined) {
    l_combined = i;
    r_combined = i + z_combined[i] - 1;
  }
}
vector<int> occurrences;
for (int i = pattern.size() + 1; i < combined.size(); i++) {
  if (z_combined[i] == pattern.size()) {
    occurrences.push_back(i - pattern.size() - 1);
  }
}
// z[i] = longitud del substring mas largo que empieza en i y es prefijo de s
// occurrences contiene las posiciones donde pattern aparece en text

