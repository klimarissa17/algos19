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
	string _s;
	int _n;
	vector<int> _c; //классы эквивалентности подстрок
	vector <int> _p;
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
	_s = str + '$';
	_n = _s.length();
	_c.resize(_n);
	_p.resize(_n);
	_build_array();
}

void SufArray::_calc_positions(int num_of_classes, vector<int> &count) {

	for (int i = 1; i < num_of_classes; i++) {
		count[i] += count[i - 1];
	}

	for (int i = num_of_classes - 1; i > 0; i--) {
		count[i] = count[i - 1];
	}
	count[0] = 0;
}

int SufArray::_sort_chars() {
	vector<int> count(ALPHABET_SIZE);
	for (int i = 0; i < _n; i++) {
		count[_s[i]] ++;
	}

	_calc_positions(ALPHABET_SIZE, count);

	for (int i = 0; i < _n; i++) {
		_p[count[_s[i]]++] = i;
	}

	int classes = 1;
	for (int i = 1; i < _n; i++) {
		if (_s[_p[i]] != _s[_p[i - 1]]) {
			classes++;
		}
		_c[_p[i]] = classes - 1;
	}
	return classes;
}


void SufArray::_sort_substrings(int classes, int k) {
	vector <int> new_c(_n);
	vector <int> new_p(_n);
	vector <int> count(classes);

	for (int i = 0; i < _n; i++) {
		new_p[i] = (_p[i] - (1 << (k-1)) + _n) % _n;
 	}

	for (int i = 0; i < _n; i++) {
		count[_c[new_p[i]]] ++;
	}

	_calc_positions(classes, count);

	for (int i = 0; i < _n; i++) {
		_p[count[_c[new_p[i]]]++] = new_p[i];
	}

	int new_classes = 1;
	int m1, m2;

	for (int i = 1; i < _n; i++) {
		m1 = (_p[i]  + (1 << (k - 1))) % _n;
		m2 = (_p[i - 1] + (1 << (k - 1))) % _n;

		if (_c[_p[i]] != _c[_p[i - 1]] || _c[m1] != _c[m2]) {
			new_classes++;
		}

	new_c[_p[i]] = new_classes - 1;
	}

	for (int i = 0; i < _n; i++) {
		_c[i] = new_c[i];
	}

	if (1 << k <= _n) {
		_sort_substrings(new_classes, ++k);
	}
}

void SufArray::_build_array() {
	int classes = _sort_chars();
	_sort_substrings(classes, 1);
}

vector<int> SufArray::print_array() {
	
	vector<int> lcp = _build_lcp();
	vector<int> res(_n - 1);

	for (int i = 1; i < _n; i++) {
		res[i - 1] = _p[i];
	}

	for (int i = 0; i < res.size(); i++) {
		cout << res[i] << " ";
	}

    cout << "\n";
	
    for (int i = 0; i < res.size(); i++) {
		cout << lcp[i] << " ";
	}
	return res;
}

vector<int> SufArray::_build_lcp() {
	vector<int> inverse(_n);
	vector<int> lcp(_n);

	for (int i = 0; i < _n; i++) {
		inverse[_p[i]] = i;
	}

	int k = 0;
	for (int i = 0; i < _n; i++) {
		if (k > 0) {
			k--;
		}
        
		if (inverse[i] == _n - 1) {
			lcp[_n - 1] = -1;
			k = 0;
		} else {
			int j = _p[inverse[i] + 1];
            
			while (max(i + k, j + k) < _n && _s[i + k] == _s[j + k]) {
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
	for (int i = 0; i < _n; i++) {
		s1 += lcp[i];
	}
	_n--;
	return (_n * (_n + 1) / 2 - s1);
}

int main() {

	string s;
	cin >> s;
	
	SufArray arr = SufArray(s);
	
	cout << arr.solve() - 1;
	return 0;
}
