#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::vector;
using std::string;
	
const int kBase = 10;

class BigInteger {

private:
	static const int pw = 1; // 4;
	BigInteger mult(const int &a) const;

public:
	vector <int> vec;
	bool negative;
	BigInteger();
	BigInteger(string str);
	BigInteger(int a) : BigInteger(std::to_string(a)) {}
	BigInteger(vector<int> v, bool neg);

	bool isZero() const;
	BigInteger abs() const;

	friend BigInteger operator - (const BigInteger &t);

	friend bool operator == (const BigInteger &t, const BigInteger &a);
	friend bool operator != (const BigInteger &t, const BigInteger &a);
	friend bool operator > (const BigInteger &t, const BigInteger &a);
	friend bool operator < (const BigInteger &t, const BigInteger &a);
	friend bool operator >= (const BigInteger &t, const BigInteger &a);
	friend bool operator <= (const BigInteger &t, const BigInteger &a);

	friend BigInteger& operator ++ (BigInteger &t);    //prefix
	friend BigInteger& operator -- (BigInteger &t);
	friend BigInteger operator ++ (BigInteger &t, int); //postfix
	friend BigInteger operator -- (BigInteger &t, int);

	friend BigInteger operator + (const BigInteger &t, const BigInteger &a);
	friend BigInteger operator - (const BigInteger &t, const BigInteger &a);
	friend BigInteger operator += (BigInteger &t, const BigInteger &a);
	friend BigInteger operator -= (BigInteger &t, const BigInteger &a);

	friend BigInteger operator * (const BigInteger &t, const BigInteger &a);
	friend BigInteger operator / (const BigInteger &t, const BigInteger &a);
	friend BigInteger operator % (const BigInteger &t, const BigInteger &a);
	friend BigInteger operator *= (BigInteger &t, const BigInteger &a);
	friend BigInteger operator /= (BigInteger &t, const BigInteger &a);
	friend BigInteger operator %= (BigInteger &t, const BigInteger &a);

	explicit operator bool() const {
		return !isZero();
	}

	string toString() const;
};

BigInteger::BigInteger() {
	negative = false;
	vec = { 0 };
}

BigInteger::BigInteger(string s1) {

	string s;

	if (s1 == "" || s1 == "0" || s1 == "-0" || s1 == "-") {
		vec.push_back(0);
		negative = false;
		return;
	}

	if (s1[0] == '-') {
		negative = true;
		for (int i = 1; i < (int)s1.length(); i++) {
			s += s1[i];
		}
	}

	else {
		negative = false;
		s = s1;
	}

	for (int i = s.length() - 1; i >= 0; i -= pw) {

		int tmp = 0;
		int dec = 1;

		for (int j = 0; j < std::min(pw, i + 1); j++) {
			tmp += (s[i - j] - '0') * dec;
			dec *= 10;
		}

		vec.push_back(tmp);
	}

	while (!vec.empty() && vec.back() == 0) {
		vec.pop_back();
	}

	if (vec.size() == 0) {
		vec.push_back(0);
	}
}

std::ostream& operator<<(std::ostream& os, const BigInteger& a) {
	os << a.toString();
	return os;
}

std::istream& operator >>(std::istream& is, BigInteger& a) {
	string s;
	is >> s;
	a = BigInteger(s);
	return is;
}


BigInteger::BigInteger(vector<int> v, bool neg) : vec(v), negative(neg) {}

string BigInteger::toString() const {
	string res;

	if (isZero()) {
		return "0";
	}

	if (negative) {
		res += "-";
	}

	for (int i = this->vec.size() - 1; i >= 0; i--) {
		res += std::to_string(vec[i]);
	}

	return res;
}

bool BigInteger::isZero() const {

	if ((int)vec.size() == 0 || ((int)vec.size() == 1 && vec[0] == 0)) {
		return true;
	}

	return false;
}

BigInteger BigInteger::abs() const {

	BigInteger a(vec, false);
	return a;
}

BigInteger BigInteger::mult(const int &a) const {

	if (a == 0 || this->isZero()) {
		return BigInteger(0);
	}

	int i;
	vector <int> res;
	int tail = 0;
	int mult;
	bool sign = a >= 0 ? 0 : 1;
	sign = sign ^ negative;

	for (i = 0; i < (int)vec.size(); i++) {
		mult = vec[i] * a + tail;
		res.push_back(mult % kBase);
		tail = mult / kBase;
	}

	if (tail) {
		res.push_back(tail);
	}

	return BigInteger(res, sign);
}


BigInteger operator - (const BigInteger& t) {
	BigInteger a(t.vec, !t.negative);
	return a;
}


bool operator == (const BigInteger &t, const BigInteger& a) {

	if (t.isZero() && a.isZero()) {
		return true;
	}

	if (t.isZero() != a.isZero()) {
		return false;
	}

	if (t.negative != a.negative) {
		return false;
	}

	if (t.vec.size() != a.vec.size()) {
		return false;
	}

	for (int i = 0; i < (int)t.vec.size(); i++) {

		if (t.vec[i] != a.vec[i]) {
			return false;
		}
	}

	return true;
}


bool operator != (const BigInteger &t, const BigInteger &a) {

	return !(t == a);
}


bool operator > (const BigInteger &t, const BigInteger &a) {

	if (!t.negative) {
		if (t.isZero()) {
			if (a.negative && !a.isZero()) {
				return true;
			}
			return false;
		}

		if (a.negative || a.isZero()) {
			return true;
		}

		else {

			if (t.vec.size() != a.vec.size()) {
				return t.vec.size() > a.vec.size();
			}

			else {
				int i = t.vec.size() - 1;
				while (i >= 0 && t.vec[i] == a.vec[i]) {
					i--;
				}

				if (i < 0) {
					return false;
				}

				return t.vec[i] > a.vec[i];

			}
		}
	}

	else {
		return !((-t > -a) && (-t != -a));
	}
}


bool operator < (const BigInteger &t, const BigInteger &a) {

	return (a > t);
}


bool operator >= (const BigInteger &t, const BigInteger &a) {

	return (t > a || t == a);
}


bool operator <= (const BigInteger &t, const BigInteger &a) {

	return (t < a || t == a);
}


BigInteger& operator ++ (BigInteger &t) {

	int i = 0;

	if (t.negative) {
		t = BigInteger(t.vec, !t.negative) - 1;
		t = -t;
		return t;
	}

	while (i < (int)t.vec.size() && t.vec[i] == kBase - 1) {
		t.vec[i] = 0;
		i++;
	}

	if (i != (int)t.vec.size()) {
		t.vec[i] ++;
		return t;
	}

	t.vec.push_back(1);
	return t;
}


BigInteger& operator -- (BigInteger &t) {

	return t = t - 1;
}


BigInteger operator ++ (BigInteger &t, int) {

	BigInteger old(t.vec, t.negative);
	++t;
	return old;
}


BigInteger operator -- (BigInteger &t, int) {

	BigInteger old(t.vec, t.negative);
	--t;
	return old;
}


BigInteger operator + (const BigInteger &t, const BigInteger &a) {

	if (a.abs() > t.abs()) {
		return (a + t);
	}

	if (a.isZero()) {
		return  t;
	}

	if (t.negative) {
		return -((-t) + (-a));
	}

	if (a.negative) {
		return t - (-a);
	}

	int tail = 0;
	int sum = 0;
	vector <int> res;
	int i;

	for (i = 0; i < (int)a.vec.size(); i++) {
		sum = t.vec[i] + a.vec[i] + tail;
		res.push_back(sum % kBase);
		tail = sum / kBase;
	}

	for (; i < (int)t.vec.size(); i++) {
		sum = t.vec[i] + tail;
		res.push_back(sum % kBase);
		tail = sum / kBase;
	}

	if (tail) {
		res.push_back(tail);
	}

	while (!res.empty() && res.back() == 0) {
		res.pop_back();
	}

	return BigInteger(res, false);
}


BigInteger operator - (const BigInteger &t, const BigInteger &a) {

	if (a == t) {
		return BigInteger(0);
	}

	if (a.abs() > t.abs()) {
		return -(a - t);
	}

	if (a.isZero()) {
		return t;
	}

	if (a.negative) {
		return (t + (-a));
	}

	if (t.negative) {
		return -((-t) + a);
	}

	int tail = 0;
	int sum = 0;
	vector <int> res;
	int i;

	for (i = 0; i < (int)a.vec.size(); i++) {
		sum = t.vec[i] - a.vec[i] - tail;
		res.push_back((kBase + sum) % kBase);
		tail = sum < 0 ? 1 : 0;
	}

	for (; i < (int)t.vec.size(); i++) {
		sum = t.vec[i] - tail;
		res.push_back((kBase + sum) % kBase);
		tail = sum < 0 ? 1 : 0;
	}

	if (tail) {
		if (!res.empty()) {
			if (res.back() == 1) {
				res.pop_back();
			}

			else {
				res.back()--;
			}
		}
	}

	while (!res.empty() && res.back() == 0) {
		res.pop_back();
	}

	return BigInteger(res, false);
}


BigInteger operator += (BigInteger &t, const BigInteger &a) {
	return t = t + a;
}


BigInteger operator -= (BigInteger &t, const BigInteger &a) {
	return t = t - a;
}


BigInteger operator * (const BigInteger &t, const BigInteger &a) {

	if (a.abs() > t.abs()) {
		return (a * t);
	}

	if (a.isZero() || t.isZero()) {
		return BigInteger(0);
	}

	bool sign = a.negative ^ t.negative;
	BigInteger a1 = a.abs();
	BigInteger t1 = t.abs();
	vector <int> res;
	BigInteger sum(0);
	BigInteger tmp(0);

	for (int i = 0; i < (int)a1.vec.size(); i++) {

		tmp = t1.mult(a1.vec[i]);

		for (int j = 0; j < i; j++) {
			tmp = tmp.mult(kBase);
		}

		sum += tmp;
	}

	sum.negative = sign;
	return sum;
}


BigInteger operator / (const BigInteger &t, const BigInteger &a) {

	if (a.abs() > t.abs()) {
		return BigInteger(0);
	}

	vector <int> tmp_vec;
	vector <int> res;
	bool sign = a.negative ^ t.negative;
	BigInteger a1 = a.abs();
	BigInteger t1 = t.abs();

	vector <int> num_piece;
	int i = 0;
	BigInteger tail(0);

	for (i = (int)a1.vec.size(); i >= 1; i--) {
		num_piece.push_back(t1.vec[t1.vec.size() - i]);
	}

	i = a1.vec.size();

	if (BigInteger(num_piece, false) < a1) {
		num_piece.push_back(0);

		for (int j = num_piece.size() - 2; j >= 0; j--) {
			num_piece[j + 1] = num_piece[j];
		}

		num_piece[0] = t1.vec[t1.vec.size() - a1.vec.size() - 1];
		i++;
	};

	int digit;

	while (i < (int)t1.vec.size()) {
		BigInteger subint(num_piece, false);
		digit = 0;

		while (a1.mult(digit) <= subint) {
			digit++;
		}

		digit = std::max(digit - 1, 0);
		res.push_back(digit);
		tail = subint - a1.mult(digit);
		num_piece.clear();
		num_piece.resize(tail.vec.size() + 1);
		num_piece[0] = t1.vec[t1.vec.size() - 1 - i];

		for (int j = 0; j < (int)tail.vec.size(); j++) {
			num_piece[j + 1] = tail.vec[j];
		}

		while (!num_piece.empty() && num_piece.back() == 0) {
			num_piece.pop_back();
		}

		i++;
	}

	BigInteger subint(num_piece, false);
	digit = 0;

	while (a1.mult(digit) <= subint) {
		digit++;
	}

	digit = std::max(digit - 1, 0);
	res.push_back(digit);
	std::reverse(res.begin(), res.end());
	return BigInteger(res, sign);
}


BigInteger operator % (const BigInteger &t, const BigInteger &a) {
	return t - (a * (t / a));
}

BigInteger operator *= (BigInteger &t, const BigInteger &a) {
	return t = t * a;
}


BigInteger operator /= (BigInteger &t, const BigInteger &a) {
	return t = t / a;
}


BigInteger operator %= (BigInteger &t, const BigInteger &a) {
	return t = t % a;
}

