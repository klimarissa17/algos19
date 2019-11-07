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
	long long n1, n2;
	long long n;
	vector<long long> c; //классы эквивалентности подстрок
	vector <long long> p;
	vector<long long> lcp;
	const long long ALPHABET_SIZE = 256;

	void _calc_positions(long long num_of_classes, vector<long long> &count);
	long long _sort_chars();
	void _sort_substrings(long long classes, long long k);
	void _build_array();
	void _build_lcp();
	pair<long long, int> kth_common_substring(long long k, long long f);

public:
	SufArray(string s);
	vector<long long> print_array();
	void solve(long long k, long long f);
};

SufArray::SufArray(string str) {
	s = str + '#';
	n = s.length();
	c.resize(n);
	p.resize(n);
	_build_array();
}

void SufArray::_calc_positions(long long num_of_classes, vector<long long> &count) {

	for(int i = 1; i < num_of_classes; i++) {
		count[i] += count[i - 1];
	}

	for(int i = num_of_classes - 1; i > 0; i--) {
		count[i] = count[i - 1];
	}
	count[0] = 0;
}

long long SufArray::_sort_chars() {
	vector<long long> count(ALPHABET_SIZE);
	for(int i = 0; i < n; i++) {
		count[s[i]] ++;
	}

	_calc_positions(ALPHABET_SIZE, count);

	for(int i = 0; i < n; i++) {
		p[count[s[i]]++] = i;
	}

	long long classes = 1;
	for(int i = 1; i < n; i++) {
		if(s[p[i]] != s[p[i - 1]]) {
			classes++;
		}
		c[p[i]] = classes - 1;
	}
	return classes;
}


void SufArray::_sort_substrings(long long classes, long long k) {
	vector <long long> new_c(n);
	vector <long long> new_p(n);
	vector <long long> count(classes);

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

	long long new_classes = 1;
	long long m1, m2;
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
	long long classes = _sort_chars();
	_sort_substrings(classes, 1);
}

vector<long long> SufArray::print_array() {
	
	vector<long long> res(n - 1);
	for(int i = 1; i < n; i++) {
		res[i - 1] = p[i];
	}

	for(int i = 0; i < res.size(); i++) {
		cout << p[i] << " ";
	}
	cout << "\n";
	for(int i = 0; i < res.size(); i++) {
		cout << lcp[i] << " ";
	}
	return res;
}

void SufArray::_build_lcp() {

	lcp.resize(n);
	vector<long long> inverse(n);
		
	for(int i = 0; i < n; i++) {
		inverse[p[i]] = i;
	}

	long long k = 0;
	for(int i = 0; i < n; i++) {
		if(k > 0) {
			k--;
		}
		if(inverse[i] == n - 1) {
			lcp[n - 1] = 0;
			k = 0;
		} else {
			long long j = p[inverse[i] + 1];
			while(max(i + k, j + k) < n && s[i + k] == s[j + k]) {
				k++;
			}
			lcp[inverse[i]] = k;
		}
	}
}
pair<long long, int> SufArray::kth_common_substring(long long k, long long f) {
	vector<bool> cat(n);
	pair <long long, int> answer;

	for(int i = 0; i<n; i++){
		if(p[i]<f) {
			cat[i] = true;
		}
	}
	long long mn = 0;
	long long curr = 0;

	for(int i = 2; i < n-1; i++) {
		if((cat[i] || cat[i + 1]) && !(cat[i] && cat[i + 1])) {
			if(lcp[i] > mn) {
				curr += lcp[i] - mn;
				if(curr >= k) {
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
			if(lcp[i] < mn) {
				mn = lcp[i];
			}
		}
	}
	if(curr < k) {
		answer.first = -1;
	}
	
	return answer;

}

void SufArray::solve(long long k, long long f) {
	_build_lcp();
	pair <long long, int> kth = kth_common_substring(k, f);

	if(kth.first != -1) {
		string res = "";
		for(int i = kth.first; i < kth.first + kth.second; i++) {
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
	long long k;
	cin >> k;
	
	SufArray *arr = new SufArray(s1 + "$" + s2);
	
	arr->solve(k, s1.length());
	return 0;
}
