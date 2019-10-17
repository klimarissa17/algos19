#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::min;
using std::max;

vector<int> z_function(string s) {
	int l = 0;
	int r = 0;
	int n = s.length();
	vector<int> z(n);
	for (int i = 1; i < n; i++) {
		z[i] = max(0, min(z[i - l], r - i));
		while (s[i + z[i]] == s[z[i]] && i + z[i] < n) {
			z[i]++;
		}
		if (z[i] + i > r){
			l = i;
			r = z[i] + i;
		}
	}
	return z;
}

void solve(string pattern, string str) {

	int s_len = str.length();
	int p_len = pattern.length();

	str = pattern + "$" + str;
	vector<int> z = z_function(str);

	for (int i = 0; i < s_len; i++) {
		if (z[p_len + 1 + i] == p_len) {
			std::cout << i << " ";
		}
	}
}

int main() {

	string pattern, str;
	std::cin >> pattern >> str;
	int s_len = str.length();
	int p_len = pattern.length();

	solve(pattern, str);

	return 0;
}

