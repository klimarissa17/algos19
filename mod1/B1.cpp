#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <random>
#include <set>

using std::vector;
using std::string;
using std::min;
using std::max;
using std::set;


vector<int> z_function(string s) {
	int l = 0;
	int r = 0;
	int n = s.length();
	vector<int> z(n);
	for(int i = 1; i < n; i++) {
		z[i] = max(0, min(z[i - l], r - i));
		while(s[i + z[i]] == s[z[i]] && i + z[i] < n) {
			z[i]++;
		}
		if (z[i] + i > r){
			l = i;
			r = z[i] + i;
		}
	}
	return z;
}

vector<int> pi_function(string s) {
	int n = s.length();
	vector<int> p(n);
	p[0] = 0;
	int j;
	for(int i = 1; i < n; i++) {
		j = p[i - 1];
		while (s[i] != s[j] && j > 0) {
			j = p[j - 1];
		}
		if (s[i] == s[j]) {
			j++;
		}
		p[i] = j;
	}
	return p;
}

string string_from_z(const vector<int> &z) {
	string s = "";
	char new_char = 'a';
	int prefix_len = 0;
	int n = z.size();
	int j;
	for(int i = 0; i < n; i++) {
		if (z[i] == 0 && prefix_len == 0) {
			s += new_char;
			if (i > 0 && z[i - 1] == 0)
				continue;
			new_char++;
		}
		if (z[i] > prefix_len) {
			prefix_len = z[i];
			j = 0;
		}
		if (prefix_len > 0) {
			s += s[j];
			j++;
			prefix_len--;
		}
	}
	return s;
}

string string_from_prefix(const vector<int> &p) {
	char new_char = 'b';
	string s = "a";
	
	set <int> f;
	for (int i = 1; i < p.size(); i++) {
		int j;
		f.clear();
		f.insert(0);
		if (p[i] == 0) {
			j = i;
			while(j>0) {
				f.insert(s[p[j-1]] - 'a');
				j = p[j - 1];
			}
			j = 0;
			while(f.count(j))
				j++;
			s += 'a' + j;
		}		
		else {
			s += s[p[i] - 1];
		}
	}
	return s;
}

string rand_string(){
	int n = rand() % 10;
	string s = "";
	for(int i = 0; i < n; i++) {
		s += char('a' + rand() % 3);
	}
	return s;
}

void solve(string pattern, string str) {

	int s_len = str.length();
	int p_len = pattern.length();

	str = pattern + "$" + str;
	vector<int> z = z_function(str);

	for(int i = 0; i < s_len; i++) {
		if (z[p_len + 1 + i] == p_len) {
			std::cout << i << " ";
		}
	}
}

int main() {

	srand(42);
	vector<int> pf;
	string s;
	int n, tmp;
	
	std::getline(std::cin, s);
	int i = 0;
	int k = 0;
	while(i < s.size()) {
		while(s[i] != ' ' && s[i] != '\n' && i < s.size()) {
			k *= 10;
			k += s[i] - '0';
			i++;
		}
		pf.push_back(k);
		k = 0;
		i++;
	}

	std::cout << string_from_prefix(pf);
	
	return 0;
}

