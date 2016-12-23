#include <string>
#include <iostream>

using std::string;
using std::to_string;
using std::ostream;
using std::istream;

class BigInteger{
	friend ostream& operator<<(ostream&, const BigInteger&);
	friend istream& operator>>(istream&, BigInteger&); 
	friend bool operator==(const BigInteger&, const BigInteger&);
	friend bool operator!=(const BigInteger&, const BigInteger&);
	friend bool operator>(const BigInteger&, const BigInteger&);
	friend bool operator>=(const BigInteger&, const BigInteger&);
	friend bool operator<(const BigInteger&, const BigInteger&);
	friend bool operator<=(const BigInteger&, const BigInteger&);
	friend BigInteger operator+(const BigInteger&, const BigInteger&);
	friend BigInteger operator-(const BigInteger&, const BigInteger&);
	friend BigInteger operator*(const BigInteger&, const BigInteger&);
	friend BigInteger operator/(const BigInteger&, const BigInteger&);
public:
	BigInteger(const string &str = string("0")); 
	BigInteger(const char *c): BigInteger(string(c)) { }
	BigInteger(const long long &l):
			BigInteger(to_string(l)) { }
	BigInteger(const int &i):
			BigInteger(to_string(i)) { }
	BigInteger operator~() const;
	bool operator!() const;
	BigInteger& operator+=(const BigInteger&);
	BigInteger& operator-=(const BigInteger&);
	BigInteger& operator*=(const BigInteger&);
	BigInteger& operator/=(const BigInteger&);
	BigInteger abs() const;
private:
	string number;
	bool sign;
};
