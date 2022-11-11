#pragma once
#include <iostream>
#include<optional>
#include<vector>
// here you can include whatever you want :)
#include <string>
#include <stdint.h>
#include <exception>


#define SUPPORT_DIVISION 0 // define as 1 when you have implemented the division
#define SUPPORT_IFSTREAM 0 // define as 1 when you have implemented the input >>

// if you do not plan to implement bonus, you can delete those lines
// or just keep them as is and do not define the macro to 1
class BigNum;
bool bigger_abs(BigNum& a, const BigNum& b);
bool equal_abs(BigNum& a, const BigNum& b);
class BigNum final
{
public:
	// constructors
	
	BigNum() {
		digits.push_back(0);
	};
	BigNum(int64_t n) {
		if (n < 0) {
			negative = true;
			n *= -1;
		}
		if (n == 0) {
			digits.push_back(0);
		}
		else {
			while (n > 0) {
				digits.push_back(static_cast<uint8_t>(n % 10));
				n = n / 10;
			}
		}
	}
	explicit BigNum(const std::string& str) {
		bool not_zero{ false };
		uint8_t digit{ 0 };
		for (size_t i = 0; i < str.size(); i++) {
			if (std::isdigit(str[i]) == 0) { // check if character is not a number
				if (i == 0) {
					if (str[i] == '-')
						negative = true;
					else if (str[i] == '+')
						negative = false;
					else
						throw std::invalid_argument("String must contain only numeric values");
				}
				else {
					throw std::invalid_argument("String must contain only numeric values");
				}
			}
			else {
				digit = static_cast<uint8_t>(str[i] - '0'); // TODO toto je zle treba pouzit atoi alebo take daco
				if (digit == 0 && not_zero == false) { // ignore 0 at the begining
					continue;
				}
				else {
					if (digit != 0 && not_zero == false) {
						not_zero = true;
					}
					//std::cout << "Vkladam " << str[i] << "\n";
					digits.insert(digits.begin(), digit);
				}
			}
		}
		if (not_zero == false) { // input was something like 000000 or -00000 or +00000
			//std::cout << "Boli same nuly \n";
			digits.insert(digits.begin(), 0);
			negative = false;
		}
	}
	// TODO copy tu vraj staci dat default
	BigNum(const BigNum& other) {
		negative = other.negative;
		for (size_t i = 0; i < other.digits.size(); i++) {
			digits.push_back(other.digits[i]);
		}
	}
	BigNum& operator=(const BigNum& rhs) {
		digits.clear();
		negative = rhs.negative;
		for (size_t i = 0; i < rhs.digits.size(); i++) {
			digits.push_back(rhs.digits[i]);
		}
		return *this;
	}
	// unary operators
	const BigNum& operator+() const { // TODO neviem to
		return *this;

	}
	BigNum operator-() const {
		BigNum b = *this;
		if (negative == true)
			b.negative = false;
		else
			b.negative = true;
		return b;
	};
	// binary arithmetics operators
	BigNum& operator+=(const BigNum& rhs) {
		BigNum a = BigNum();
		a = *this;
		a = a + rhs;
		this->digits.resize(a.digits.size());
		for (size_t i = 0; i < a.digits.size();i++) {
			digits[i] = a.digits[i];
		}
		this->negative = a.negative;
		return *this;
	}
	BigNum& operator-=(const BigNum& rhs) {
		BigNum a = BigNum();
		a = *this;
		a = a - rhs;
		this->digits.resize(a.digits.size());
		for (size_t i = 0; i < a.digits.size(); i++) {
			digits[i] = a.digits[i];
		}
		this->negative = a.negative;
		return *this;
	}
	BigNum& operator*=(const BigNum& rhs) {
		BigNum a = BigNum();
		a = *this;
		a = a * rhs;
		this->digits.resize(a.digits.size());
		for (size_t i = 0; i < a.digits.size(); i++) {
			digits[i] = a.digits[i];
		}
		this->negative = a.negative;
		return *this;
	}
#if SUPPORT_DIVISION == 1
	BigNum& operator/=(const BigNum& rhs); // bonus
	BigNum& operator%=(const BigNum& rhs); // bonus
#endif
private:
	std::vector<uint8_t> digits;
	bool negative{ false };
	// here you can add private data and members, but do not add stuff to 
	// public interface, also you can declare friends here if you want
	friend BigNum operator+(BigNum lhs, const BigNum& rhs);
	friend BigNum operator-(BigNum lhs, const BigNum& rhs);
	friend BigNum operator*(BigNum lhs, const BigNum& rhs);
	friend bool operator<(const BigNum& lhs, const BigNum& rhs);
	friend bool operator==(const BigNum& lhs, const BigNum& rhs);
	friend std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs);
	friend bool bigger_abs(BigNum& a, const BigNum& b);
	friend bool equal_abs(BigNum& a, const BigNum& b);
};
BigNum operator+(BigNum lhs, const BigNum& rhs) {
	BigNum a = BigNum();
	size_t n1 = lhs.digits.size();
	size_t n2 = rhs.digits.size();
	size_t n_max = n1 >= n2 ? n1 : n2;
	size_t n_min = n1 <= n2 ? n1 : n2;
	uint8_t remainder = 0;
	uint8_t sum = 0;
	uint8_t diff = 0;
	a.digits.resize(n_max); //resize digits to the new longer size
	size_t i = 0;
	// left is positive right is negative so its lhs - rhs
	if (lhs.negative == false && rhs.negative == true) {
		if (bigger_abs(lhs, rhs) == false && equal_abs(lhs,rhs) == false) { //If rhs has bigger absolute value
			a = (-rhs) + (-lhs);
			a.negative = true;
			return a;
		}
		// now we do lhs - rhs, lhs is bigger
		for (i=0; i < n_min; i++) { // loop until the smaller length, where both have defined digits
			if (lhs.digits[i] < (rhs.digits[i]+remainder)) {
				diff = (lhs.digits[i] + 10) - (rhs.digits[i] + remainder);
				remainder = 1;
			}
			else {
				diff = lhs.digits[i] - (rhs.digits[i] + remainder);
				remainder = 0;
			}
			a.digits[i] = diff;
		}
		for (; i < n1; i++) { //continue until n1
			if (lhs.digits[i] < remainder) {
				diff = lhs.digits[i] + 10 - remainder;
				remainder = 1;
			}
			else {
				diff = lhs.digits[i] - remainder;
				remainder = 0;
			}
			a.digits[i] = diff;
		}
		// delete 0 before for example 01
		for (size_t k = a.digits.size() - 1; k > 0; k--) {
			if (a.digits[k] != 0)
				break;
			else
				a.digits.erase(a.digits.begin()+k);
		}
		a.negative = lhs.negative;
		return a;
	}
	if (lhs.negative == true && rhs.negative == false) {
		if (bigger_abs(lhs, rhs) == true) {
			a = (-lhs) + (-rhs);
			a.negative = true;
			return a;
		}
		else {
			a = rhs + lhs;
			a.negative = false;
			return a;
		}
	}
	//BOTH ARE POSITIVE NUMBERS  or both are negative  numbers
	i = 0;
	for (; i < n_min; i++) { // loop until the smaller length, where both have defined digits
		sum = lhs.digits[i] + rhs.digits[i] + remainder;
		a.digits[i] = sum % 10;
		remainder = sum / 10;
	}
	if (n_max == n1) { // lhs is longer
		for (; i < n1; i++) {
			sum = (lhs.digits[i] + remainder);
			a.digits[i] = sum % 10;
			remainder = sum / 10;
		}
	}
	else if (n_max == n2) { // rhs is longer
		for (; i < n2; i++) {
			sum = (rhs.digits[i] + remainder);
			a.digits[i] = sum % 10;
			remainder = sum / 10;
		}
	}
	if (remainder != 0) {
		a.digits.push_back(remainder);
	}
	if (lhs.negative == true && rhs.negative == true)
		a.negative = true;
	return a;
};
BigNum operator-(BigNum lhs, const BigNum& rhs) {
	return lhs + (-rhs);
};
BigNum operator*(BigNum lhs, const BigNum& rhs) {
	BigNum a = BigNum();
	size_t n1 = lhs.digits.size();
	size_t n2 = rhs.digits.size();
	uint8_t remainder = 0;
	uint8_t multipl = 0;
	if ((n1 == 1 && lhs.digits[0] == 0) || (n2 == 1 && rhs.digits[0] == 0)) {
		return a;
	}
	std::vector<BigNum> temp;
	for (size_t i = 0; i < n2; i++) {
		temp.push_back(BigNum(0));
		temp[i].digits.clear();
		for (size_t k = 0; k < n1; k++) {
			multipl = (lhs.digits[k] * rhs.digits[i]+remainder);
			remainder = multipl / 10;
			temp[i].digits.push_back(multipl % 10);
		}
		if (remainder != 0) {
			temp[i].digits.push_back(remainder);
			remainder = 0;
		}
		//apend 0 for addition after
		for (size_t z = 0; z < i; z++) {
			temp[i].digits.insert(temp[i].digits.begin(), 0);
		}
	}
	for (size_t i = 0; i < temp.size(); i++) {
		a = a + temp[i];
	}
	if (lhs.negative == true && rhs.negative == false)
		a.negative = true;
	if (lhs.negative == false && rhs.negative == true)
		a.negative = true;
	return a;
};
#if SUPPORT_DIVISION == 1
BigNum operator/(BigNum lhs, const BigNum& rhs); // bonus
BigNum operator%(BigNum lhs, const BigNum& rhs); // bonus
#endif

// alternatively you can implement 
// std::strong_ordering operator<=>(const BigNum& lhs, const BigNum& rhs);
// idea is, that all comparison should work, it is not important how you do it
bool operator==(const BigNum& lhs, const BigNum& rhs) {
	if (lhs.digits.size() != rhs.digits.size()) {
		return false;
	}
	for (size_t i = 0; i < lhs.digits.size(); i++) {
		if (lhs.digits[i] != rhs.digits[i])
			return false;
	}
	return true;

};
bool operator!=(const BigNum& lhs, const BigNum& rhs) {
	return !(lhs == rhs);
}
bool operator<(const BigNum& lhs, const BigNum& rhs) {
	size_t n_left = lhs.digits.size();
	size_t n_right = rhs.digits.size();
	// both positive numbers, different length
	if (n_left > n_right && lhs.negative == false && rhs.negative == false) {
		return false;
	}
	if (n_right > n_left && lhs.negative == false && rhs.negative == false) {
		return true;
	}
	//both negative numbers, different length
	if (n_left > n_right && lhs.negative == true && rhs.negative == true) {
		return true;
	}
	if (n_left < n_right && lhs.negative == true && rhs.negative == true) {
		return false;
	}
	// one positive one negative
	if (lhs.negative == true && rhs.negative == false) {
		return true;
	}
	if (lhs.negative == false && rhs.negative == true) {
		return false;
	}
	//both positive or negative, equal length 
	size_t i = lhs.digits.size() - 1;
	for (; i > 0; i--) { // we go from bigger numbers to lower numbers
		if (lhs.digits[i] < rhs.digits[i]) { // we found smaller digit in lhs
			if (lhs.negative == true) //if they are both negative and lhs has smaller digit than its bigger
				return false;
			else
				return true; //if they are both positive and lhs has smaller digit than it is smaller
		}
		else if (lhs.digits[i] > rhs.digits[i]) { // if rhs digit was smaller
			if (lhs.negative == true) //if they are both negative rhs is bigger so return true lhs is smaller
				return true;
			else
				return false; // if they are both positive rhs is smaller, so return false lhs is bigger
		}
	}
	if (lhs.digits[0] < rhs.digits[0]) { //compare the last digit
		if (lhs.negative == true)
			return false;
		else
			return true;
	}
	else if (lhs.digits[i] > rhs.digits[i]) { // if rhs digit was smaller
		if (lhs.negative == true) //if they are both negative rhs is bigger so return true lhs is smaller
			return true;
		else
			return false; // if they are both positive rhs is smaller, so return false lhs is bigger
	}
	return false; // they are equal
};
bool operator>(const BigNum& lhs, const BigNum& rhs) {
	return rhs < lhs;
};
bool operator<=(const BigNum& lhs, const BigNum& rhs) {
	return !(lhs > rhs);
};
bool operator>=(const BigNum& lhs, const BigNum& rhs) {
	return !(lhs < rhs);
};

std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs) {
	std::string r_val{ "" };
	
	for (size_t i = 0; i < rhs.digits.size(); i++) {
		r_val.insert(r_val.begin(), '0' + rhs.digits[i]);
	}
	if (rhs.negative == true) {
		r_val.insert(r_val.begin(),'-');
	}
	return lhs << r_val;
}

#if SUPPORT_IFSTREAM == 1
std::istream& operator>>(std::istream& lhs, BigNum& rhs); // bonus
#endif
bool bigger_abs(BigNum& a, const BigNum& b) { // return true if A has bigger or equal absolute value than B
	if (a.digits.size() > b.digits.size())
		return true;
	if (b.digits.size() > a.digits.size())
		return false;
	for (size_t i = a.digits.size()-1; i > 0; i--) {
		if (a.digits[i] > b.digits[i])
			return true;
		if (a.digits[i] < b.digits[i])
			return false;
	}
	return a.digits[0] > b.digits[0];
}

bool equal_abs(BigNum& a, const BigNum& b) {
	if (a.digits.size() > b.digits.size())
		return false;
	if (b.digits.size() > a.digits.size())
		return false;
	for (size_t i = a.digits.size() - 1; i > 0; i--) {
		if (a.digits[i] != b.digits[i])
			return false;
	}
	return a.digits[0] == b.digits[0];
}