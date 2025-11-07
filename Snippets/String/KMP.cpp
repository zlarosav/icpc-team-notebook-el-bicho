string s; cin >> s;
int n = s.size();
vector<int> pi(n);
for (int i = 1; i < n; i++) {
  int j = pi[i - 1];
  while (j > 0 && s[i] != s[j]) {
    j = pi[j - 1];
  }
  if (s[i] == s[j]) j++;
  pi[i] = j;
}

string pattern, text;
cin >> pattern >> text;
string combined = pattern + "#" + text;
vector<int> pi_combined(combined.size());
for (int i = 1; i < combined.size(); i++) {
  int j = pi_combined[i - 1];
  while (j > 0 && combined[i] != combined[j]) {
    j = pi_combined[j - 1];
  }
  if (combined[i] == combined[j]) j++;
  pi_combined[i] = j;
}
vector<int> occurrences;
for (int i = pattern.size() + 1; i < combined.size(); i++) {
  if (pi_combined[i] == pattern.size()) {
    occurrences.push_back(i - 2 * pattern.size());
  }
}
// pi[i] = longitud del prefijo mas largo que es sufijo en s[0..i]
// occurrences contiene las posiciones donde pattern aparece en text

