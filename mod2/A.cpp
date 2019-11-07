#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <set>

using std::vector;
using std::string;
using std::min;
using std::max;
using std::cin;
using std::cout;

class SufArray {
private:
	string s;
	int n;
	vector<int> c; //классы эквивалентности подстрок
	vector <int> p;
	const int ALPHABET_SIZE = 256;

	void _calc_positions(int num_of_classes, vector<int> &count);
	int _sort_chars();
	void _sort_substrings(int classes, int k);
	void _build_array();
	vector<int> _build_lcp();

public:
	SufArray(string s);
	vector<int> print_array();
	int solve();
};

SufArray::SufArray(string str) {
	s = str + '$';
	n = s.length();
	c.resize(n);
	p.resize(n);
	_build_array();
}

void SufArray::_calc_positions(int num_of_classes, vector<int> &count) {

	for(int i = 1; i < num_of_classes; i++) {
		count[i] += count[i - 1];
	}

	for(int i = num_of_classes - 1; i > 0; i--) {
		count[i] = count[i - 1];
	}
	count[0] = 0;
}

int SufArray::_sort_chars() {
	vector<int> count(ALPHABET_SIZE);
	for(int i = 0; i < n; i++) {
		count[s[i]] ++;
	}

	_calc_positions(ALPHABET_SIZE, count);

	for(int i = 0; i < n; i++) {
		p[count[s[i]]++] = i;
	}

	int classes = 1;
	for(int i = 1; i < n; i++) {
		if(s[p[i]] != s[p[i - 1]]) {
			classes++;
		}
		c[p[i]] = classes - 1;
	}
	return classes;
}


void SufArray::_sort_substrings(int classes, int k) {
	vector <int> new_c(n);
	vector <int> new_p(n);
	vector <int> count(classes);

	for(int i = 0; i < n; i++) {
		new_p[i] = (p[i] - (1 << (k-1)) + n) % n;
 	}

	for(int i = 0; i < n; i++) {
		count[c[new_p[i]]] ++;
	}

	_calc_positions(classes, count);

	for(int i = 0; i < n; i++) {
		p[count[c[new_p[i]]]++] = new_p[i];
	}

	int new_classes = 1;
	int m1, m2;

	for(int i = 1; i < n; i++) {
		m1 = (p[i]  + (1<<(k-1))) % n;
		m2 = (p[i - 1] + (1<<(k-1))) % n;

		if(c[p[i]] != c[p[i - 1]] || c[m1] != c[m2]) {
			new_classes++;
		}

	new_c[p[i]] = new_classes - 1;
	}

	for(int i = 0; i < n; i++) {
		c[i] = new_c[i];
	}

	if(1 << k <= n) {
		_sort_substrings(new_classes, ++k);
	}
}

void SufArray::_build_array() {
	int classes = _sort_chars();
	_sort_substrings(classes, 1);
}

vector<int> SufArray::print_array() {
	
	vector<int> lcp = _build_lcp();
	vector<int> res(n - 1);

	for(int i = 1; i < n; i++) {
		res[i - 1] = p[i];
	}

	for(int i = 0; i < res.size(); i++) {
		cout << res[i] << " ";
	}

	cout << "\n";
	
    for(int i = 0; i < res.size(); i++) {
		cout << lcp[i] << " ";
	}
	return res;
}

vector<int> SufArray::_build_lcp() {
	vector<int> inverse(n);
	vector<int> lcp(n);

	for(int i = 0; i < n; i++) {
		inverse[p[i]] = i;
	}

	int k = 0;
	for(int i = 0; i < n; i++) {
		if(k > 0) {
			k--;
		}
        
		if(inverse[i] == n - 1) {
			lcp[n - 1] = -1;
			k = 0;
		} else {
			int j = p[inverse[i] + 1];
            
			while(max(i + k, j + k) < n && s[i + k] == s[j + k]) {
				k++;
			}
			lcp[inverse[i]] = k;
		}
	}
	return lcp;
}

int SufArray::solve() {
	int s1 = 0;
	vector<int> lcp = _build_lcp();
	for(int i = 0; i < n; i++) {
		s1 += lcp[i];
	}
	n--;
	return (n * (n + 1) / 2 - s1);
}

int main() {

	string s;
	cin >> s;
	
	SufArray *arr = new SufArray(s);
	
	cout << arr->solve() - 1;
	return 0;
}
