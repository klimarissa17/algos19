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
using std::pair;

class SufArray {
private:
	string s;
	string s1, s2;
	int64_t n1, n2;
	int64_t n;
	vector<int64_t> c; //классы эквивалентности подстрок
	vector <int64_t> p;
	vector<int64_t> lcp;
	const int64_t ALPHABET_SIZE = 256;

	void _calc_positions(int64_t num_of_classes, vector<int64_t> &count);
	int64_t _sort_chars();
	void _sort_substrings(int64_t classes, int64_t k);
	void _build_array();
	void _build_lcp();
	pair<int64_t, int> kth_common_substring(int64_t k, int64_t f);

public:
	SufArray(string s);
	vector<int64_t> print_array();
	void solve(int64_t k, int64_t f);
};

SufArray::SufArray(string str) {
	s = str + '#';
	n = s.length();
	c.resize(n);
	p.resize(n);
	_build_array();
}

void SufArray::_calc_positions(int64_t num_of_classes, vector<int64_t> &count) {

	for (int i = 1; i < num_of_classes; i++) {
		count[i] += count[i - 1];
	}

	for (int i = num_of_classes - 1; i > 0; i--) {
		count[i] = count[i - 1];
	}
	count[0] = 0;
}

int64_t SufArray::_sort_chars() {
	vector<int64_t> count(ALPHABET_SIZE);
	for (int i = 0; i < n; i++) {
		count[s[i]] ++;
	}

	_calc_positions(ALPHABET_SIZE, count);

	for (int i = 0; i < n; i++) {
		p[count[s[i]]++] = i;
	}

	int64_t classes = 1;
	for (int i = 1; i < n; i++) {
		if (s[p[i]] != s[p[i - 1]]) {
			classes++;
		}
		c[p[i]] = classes - 1;
	}
	return classes;
}


void SufArray::_sort_substrings(int64_t classes, int64_t k) {
	vector <int64_t> new_c(n);
	vector <int64_t> new_p(n);
	vector <int64_t> count(classes);

	for (int i = 0; i < n; i++) {
		new_p[i] = (p[i] - (1 << (k-1)) + n) % n;
 	}

	for (int i = 0; i < n; i++) {
		count[c[new_p[i]]] ++;
	}

	_calc_positions(classes, count);

	for (int i = 0; i < n; i++) {
		p[count[c[new_p[i]]]++] = new_p[i];
	}

	int64_t new_classes = 1;
	int64_t m1, m2;
	for (int i = 1; i < n; i++) {
		m1 = (p[i]  + (1<<(k-1))) % n;
		m2 = (p[i - 1] + (1<<(k-1))) % n;

		if (c[p[i]] != c[p[i - 1]] || c[m1] != c[m2]) {
			new_classes++;
		}

	new_c[p[i]] = new_classes - 1;
	}

	for (int i = 0; i < n; i++) {
		c[i] = new_c[i];
	}

	if (1 << k <= n) {
		_sort_substrings(new_classes, ++k);
	}
}

void SufArray::_build_array() {
	int64_t classes = _sort_chars();
	_sort_substrings(classes, 1);
}

vector<int64_t> SufArray::print_array() {
	
	vector<int64_t> res(n - 1);
	for (int i = 1; i < n; i++) {
		res[i - 1] = p[i];
	}

	for (int i = 0; i < res.size(); i++) {
		cout << p[i] << " ";
	}
	cout << "\n";
	for (int i = 0; i < res.size(); i++) {
		cout << lcp[i] << " ";
	}
	return res;
}

void SufArray::_build_lcp() {

	lcp.resize(n);
	vector<int64_t> inverse(n);
		
	for (int i = 0; i < n; i++) {
		inverse[p[i]] = i;
	}

	int64_t k = 0;
	for (int i = 0; i < n; i++) {
		if (k > 0) {
			k--;
		}
		if (inverse[i] == n - 1) {
			lcp[n - 1] = 0;
			k = 0;
		} else {
			int64_t j = p[inverse[i] + 1];
			while (max(i + k, j + k) < n && s[i + k] == s[j + k]) {
				k++;
			}
			lcp[inverse[i]] = k;
		}
	}
}
pair<int64_t, int> SufArray::kth_common_substring(int64_t k, int64_t f) {
	vector<bool> cat(n);
	pair <int64_t, int> answer;

	for (int i = 0; i<n; i++){
		if (p[i]<f) {
			cat[i] = true;
		}
	}
	int64_t mn = 0;
	int64_t curr = 0;

	for (int i = 2; i < n-1; i++) {
		if ((cat[i] || cat[i + 1]) && !(cat[i] && cat[i + 1])) {
			if (lcp[i] > mn) {
				curr += lcp[i] - mn;
				if (curr >= k) {
					answer.first = p[i];
					answer.second = lcp[i] - curr + k;
					return answer;
				}
				mn = lcp[i];
			}
			else {
                mn = lcp[i];
			}

		} else {
			if (lcp[i] < mn) {
				mn = lcp[i];
			}
		}
	}
	if (curr < k) {
		answer.first = -1;
	}
	
	return answer;

}

void SufArray::solve(int64_t k, int64_t f) {
	_build_lcp();
	pair <int64_t, int> kth = kth_common_substring(k, f);

	if (kth.first != -1) {
		string res = "";
		for (int i = kth.first; i < kth.first + kth.second; i++) {
			res += s[i];
		}
		cout << res;
	} else{
		cout << -1;
	}
}

int main() { 

	string s1, s2;
	cin >> s1 >> s2;
	int64_t k;
	cin >> k;
	
	SufArray arr = SufArray(s1 + "$" + s2);
	
	arr.solve(k, s1.length());
	return 0;
}
