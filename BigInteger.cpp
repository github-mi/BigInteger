#include "BigInteger.h"
#include <algorithm>
#include <iterator>
#include <cctype>
using std::swap;
using std::cerr;

//------------------------------ Constructor
BigInteger::BigInteger(const string &str): number(str), sign(true) {
	number.erase(number.begin(),
				 find_if_not(number.begin(), number.end(), [](char c){return std::isspace(c);})
				 ); //remove the leading spaces
	number.erase(find_if_not(number.rbegin(), number.rend(), [](char c){return std::isspace(c);}).base(),
				 number.end()
				 ); //remove the trailing spaces
	if(number.empty()) {
		cerr << "Error: BigInteger is empty!\n";
		number = "0"; //set BigInteger to  0
	}
	else {
		if (number[0] == '-'){
			number.erase(number.begin());
			sign = false;
		}
		else {
			if(number[0] == '+'){
				number.erase(number.begin());
			}
		}
		if (number.empty()){
			cerr << "Error: BigInteger only have sign\n";
			number = "0"; //set BigInteger to  0
		}
		else {
			if (number[0] == '0' && number.size() > 1) {
				cerr << "Error: unexpected leading zero!\n";
				number = "0"; //set BigInteger to 0;
			}
			else{
				auto first = find_if_not(number.begin(), number.end(), [](char c) {return std::isdigit(c);});
				if (first != number.end()){
					cerr << "Error: BigInteger must be digits\n";
					number = "0"; //set BigInteger to  0
				}
				else{
					if (number[0] == '0') sign = true;
				}
			}
		}
	}
}
//------------------------------ operator<< 
ostream &operator<<(ostream &os, const BigInteger &bi){
	if (bi.sign) 
		os << bi.number;
	else
		os << "-" << bi.number;
	return os;
}
//------------------------------ operator>>
istream &operator>>(istream &is, BigInteger &bi){
	is >> bi.number; 
	if(!is)
		bi = BigInteger();
	else
		bi = BigInteger(bi.number);
	return is;
}
//------------------------------ operator== 
bool operator==(const BigInteger &lhs, const BigInteger &rhs) {
	return lhs.number == rhs.number && lhs.sign == rhs.sign;
}
	
//------------------------------  operator!=
bool operator!=(const BigInteger &lhs, const BigInteger &rhs) {
	return !(lhs == rhs);
}

//------------------------------  operator >
bool operator>(const BigInteger &lhs, const BigInteger &rhs) {
	if (lhs.sign > rhs.sign)
		return true;
	else{
		if (lhs.sign < rhs.sign)
			return false;
		else
			if (lhs.sign){
				if (lhs.number.size() > rhs.number.size())
					return true;
				else {
					if (lhs.number.size() < rhs.number.size())
						return false;
					else {
						return lhs.number > rhs.number;
					}
				}
			}
			else
				return !(~lhs > ~rhs);
	}
}
//------------------------------  operator >=
bool operator>=(const BigInteger &lhs, const BigInteger &rhs) {
	return lhs == rhs || lhs > rhs;
}
//------------------------------  operator <
bool operator<(const BigInteger &lhs, const BigInteger &rhs) {
	return !(lhs >= rhs);
}
//------------------------------  operator <=
bool operator<=(const BigInteger &lhs, const BigInteger &rhs) {
	return !(lhs > rhs);
}
//------------------------------  operator ~
BigInteger BigInteger::operator~() const {
	if (number[0] == '0')
		return *this;
	else{
		BigInteger temp = *this;
		temp.sign = !sign;
		return temp;
	}
}
//------------------------------  operator !
bool BigInteger::operator!() const {
	if (number[0] != '0')
		return false;
	else
		return true;
}
//------------------------------  operator +
BigInteger operator+(const BigInteger &lhs, const BigInteger &rhs){
	if (lhs.sign != rhs.sign)
		return lhs - ~rhs;
	else {
		if (lhs.number.size() >= rhs.number.size()){
			BigInteger sum = lhs;
			for (string::size_type i = 0; i != rhs.number.size(); ++i)
				sum.number[sum.number.size()-i-1] +=
						rhs.number[rhs.number.size()-i-1] - '0';
			int carry = 0, temp = 0;
			for (string::size_type i = 0; i != sum.number.size(); ++i){
				temp = sum.number[sum.number.size()-i-1] - '0' + carry;  
				sum.number[sum.number.size()-i-1] = temp % 10 + '0';
				carry = temp / 10;
			}
			if (carry == 1)
				sum.number.insert(sum.number.begin(), '1');
			return sum;
		}
		else 
			return rhs + lhs;
	}
}
//------------------------------  operator +=
BigInteger& BigInteger::operator+=(const BigInteger &rhs){
	*this = *this + rhs;
	return *this;
}
//------------------------------  operator -
BigInteger operator-(const BigInteger &lhs, const BigInteger &rhs){
	if (lhs.sign != rhs.sign)
		return lhs + ~rhs;
	else {
		if (lhs.sign) {
			if (lhs >= rhs) {
				BigInteger diff = lhs;
				for (string::size_type i = 0; i != rhs.number.size(); ++i)
					diff.number[diff.number.size()-i-1] -=
						(rhs.number[rhs.number.size()-i-1] - '0');
				int carry = 0, temp =0;
				for (string::size_type i = 0; i != diff.number.size(); ++i){
					temp = diff.number[diff.number.size()-i-1] - '0' - carry;
					diff.number[diff.number.size()-i-1] = (temp + 10) % 10 + '0';
					carry = temp < 0 ? 1 : 0;
				}
				auto first = find_if(diff.number.begin(), diff.number.end(),
									[](char c) {return c != '0';}
									);
				if (first == diff.number.end())
					diff.number = "0";
				else
					diff.number.erase(diff.number.begin(), first);
				return diff;
			}
			else
				return ~(rhs - lhs);
		}
		else
			return ~(~lhs - ~rhs);
	}
}
//------------------------------  operator -=
BigInteger& BigInteger::operator-=(const BigInteger &rhs){
	*this = *this - rhs;
	return *this;
}
//------------------------------  operator *
BigInteger operator*(const BigInteger &lhs, const BigInteger &rhs){
	if (lhs.number.size() >= rhs.number.size()) {
		BigInteger prod;
		for (string::size_type i = 0; i != rhs.number.size(); ++i) {
			if (rhs.number[rhs.number.size()-i-1] == '0')
				continue;
			BigInteger prod_temp = lhs;
			int carry = 0, temp = 0;
			for (string::size_type k =0; k != lhs.number.size(); ++k) {
				temp =  (lhs.number[lhs.number.size()-k-1] - '0') *
						(rhs.number[rhs.number.size()-i-1] - '0') + carry;
				prod_temp.number[prod_temp.number.size()-k-1] = temp % 10 + '0';
				carry = temp / 10;
			}
			if (carry > 0)
				prod_temp.number.insert(prod_temp.number.begin(), carry + '0');
			prod_temp.number.append(i, '0');
			prod += prod_temp;
		}
		prod.sign = !(lhs.sign ^ rhs.sign);
		return prod;
	}
	else
		return rhs * lhs;
}
//------------------------------  operator *=
BigInteger& BigInteger::operator*=(const BigInteger &rhs){
	*this = *this * rhs;
	return *this;
}
//------------------------------  operator /
BigInteger operator/(const BigInteger &lhs, const BigInteger &rhs){
	BigInteger lhs_temp = lhs.abs();
	BigInteger quot;
	if (lhs.number.size() >= rhs.number.size()) {
		auto gap = lhs.number.size() - rhs.number.size();
		for (int i = gap; i != -1; --i) {
			BigInteger rhs_temp = rhs.abs() * BigInteger("1"+string(i, '0'));
			while (lhs_temp >= rhs_temp) {
				lhs_temp -= rhs_temp;
				quot += BigInteger("1"+string(i, '0'));
			}
		}
	}
	quot.sign = !(lhs.sign ^ rhs.sign);
	return quot;
}
//------------------------------  operator /=
BigInteger& BigInteger::operator/=(const BigInteger &rhs){
	*this = *this / rhs;
	return *this;
}
//------------------------------  operator abs()
BigInteger BigInteger::abs() const {
		BigInteger temp = *this;
		temp.sign = true;
		return temp;
}
