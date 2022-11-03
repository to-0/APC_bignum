
#include <iostream>
#include<optional>
#include<vector>
#pragma once
// here you can include whatever you want :)
#include <string>
#include <stdint.h>
#include <exception>

#define SUPPORT_DIVISION 0 // define as 1 when you have implemented the division
#define SUPPORT_IFSTREAM 0 // define as 1 when you have implemented the input >>

// if you do not plan to implement bonus, you can delete those lines
// or just keep them as is and do not define the macro to 1
class BigNum final
{
public:
	// constructors
	std::vector<uint8_t> digits;
	bool negative{false};
	BigNum() {
		digits.push_back(0);
	};
	BigNum(int64_t n) {
		if (n < 0) {
			negative = true;
			n *= -1;
		}
		while (n > 0) {
			digits.push_back(static_cast<uint8_t>(n % 10));
			n = n / 10;
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
				digit = static_cast<uint8_t>(str[i]-'0'); // TODO toto je zle treba pouzit atoi alebo take daco
				if (digit == 0 && not_zero == false) { // ignore 0 at the begining
					continue;
				}
				else {
					if (digit != 0 && not_zero == false) {
						not_zero = true;
					}
					digits.insert(digits.begin(),digit);
				}
			}
		}
		if (not_zero == false) { // input was something like 000000 or -00000 or +00000
			digits.insert(digits.begin(), 0);
			negative = false;
		}
	}
	// copy
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
	const BigNum& operator+() const {
	}
	BigNum operator-() const;
	// binary arithmetics operators
	BigNum& operator+=(const BigNum& rhs) {
		size_t n1 = digits.size();
		size_t n2 = rhs.digits.size();
		size_t n_max = n1 >= n2 ? n1 : n2;
		digits.resize(n_max); //resize digits to the new longer size
		uint8_t remainder = 0;
		uint8_t sum = 0;
		size_t i = 0;
		for (i; i < n1; i++) { // loop until the smaller length is achieved
			if (i >= n2) { // if the second number is smaller break
				break;
			}
			sum = digits[i] + rhs.digits[i]+remainder;
			digits[i] = sum % 10;
			remainder = sum / 10;
		}
		if (n_max == n1) { // current BigNum is longer
			for (i; i < digits.size();i++) {
				sum = (digits[i] + remainder);
				digits[i] = sum % 10;
				remainder = sum / 10;
			}
		}
		else if (n_max == n2) { // rhs BigNum is longer
			for (i; i < n2; i++) {
				sum = (rhs.digits[i] + remainder);
				digits[i] = sum % 10;
				remainder = sum / 10;
			}
		}
		if (remainder != 0) {
			digits.push_back(remainder);
		}
		return *this;
	};
	BigNum& operator-=(const BigNum& rhs);
	BigNum& operator*=(const BigNum& rhs); 
#if SUPPORT_DIVISION == 1
		BigNum& operator/=(const BigNum& rhs); // bonus
	BigNum& operator%=(const BigNum& rhs); // bonus
#endif
private:
	// here you can add private data and members, but do not add stuff to 
	// public interface, also you can declare friends here if you want
};
BigNum operator+(BigNum lhs, const BigNum& rhs) {
	BigNum a = BigNum();
	//Todo ODKOMENTOVAT KED BUDEM MAT MINUS
	/*
	if (lhs.negative == false && rhs.negative == true) {
		BigNum b = BigNum(lhs);
		BigNum c = BigNum(rhs);
		b.negative = false;
		c.negative = false;
		a = b - c;
		// a = lhs - rhs; // neprepnu sa mi ale negative flagy veci cize neviem ci to viem nejako rozumne spravit
	}
	if (lhs.negative == true && rhs.negative == false) {
		BigNum b = BigNum(lhs);
		BigNum c = BigNum(rhs);
		b.negative = false;
		c.negative = false;
		a = c - b;
		//a = rhs - lhs; // neprepnu sa mi ale negative flagy veci cize neviem ci to viem nejako rozumne spravit
	}
	*/
	
	size_t n1 = lhs.digits.size();
	size_t n2 = rhs.digits.size();
	size_t n_max = n1 >= n2 ? n1 : n2;
	size_t n_min = n1 <= n2 ? n1 : n2;
	a.digits.resize(n_max); //resize digits to the new longer size
	uint8_t remainder = 0;
	uint8_t sum = 0;
	size_t i = 0;
	for (i; i < n_min; i++) { // loop until the smaller length, where both have defined digits
		sum = lhs.digits[i] + rhs.digits[i] + remainder;
		a.digits[i] = sum % 10;
		remainder = sum / 10;
	}
	if (n_max == n1) { // lhs is longer
		for (i; i < n1; i++) {
			sum = (lhs.digits[i] + remainder);
			a.digits[i] = sum % 10;
			remainder = sum / 10;
		}
	}
	else if (n_max == n2) { // rhs is longer
		for (i; i < n2; i++) {
			sum = (rhs.digits[i] + remainder);
			a.digits[i] = sum % 10;
			remainder = sum / 10;
		}
	}
	if (remainder != 0) {
		a.digits.push_back(remainder);
	}
	/* TODO ODKOMENTOVAT KED BUDEM MAT -A
	if (lhs.negative && rhs.negative) // both numbers are negative
		a = -a;
		*/
	return a;
};
BigNum operator-(BigNum lhs, const BigNum& rhs);
BigNum operator*(BigNum lhs, const BigNum& rhs);
#if SUPPORT_DIVISION == 1
BigNum operator/(BigNum lhs, const BigNum& rhs); // bonus
BigNum operator%(BigNum lhs, const BigNum& rhs); // bonus
#endif

// alternatively you can implement 
// std::strong_ordering operator<=>(const BigNum& lhs, const BigNum& rhs);
// idea is, that all comparison should work, it is not important how you do it
bool operator==(const BigNum& lhs, const BigNum& rhs) {
	if (lhs.digits.size() != lhs.digits.size()) {
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
	// one positive one negative
	if (lhs.negative == true && rhs.negative == false) {
		return true;
	}
	if (lhs.negative == false &&  rhs.negative == true) {
		return false;
	}
	//both positive equal length or both negative different length
	size_t i = lhs.digits.size() - 1;
	bool end_flag = false;
	for (size_t j = 0; j < lhs.digits.size(); j++) { // we go from the bigger numbers down and compare those, retarded syntax but for(size_t i=digits.size()-1; i >=0; i--) gave me a warning even when i had a break
		if (i > rhs.digits.size()) { // rhs is shorter 
			if (rhs.negative == true) { //they are both negative and rhs is shorter than lhs so lhs is smaller since we work with negative numbers
				return true;
			}
			else { // rhs is positive so lhs is positive too, lhs is longer so lhs is bigger we return false
				return false;
			}
		}
		if (lhs.digits[i] < rhs.digits[i]) { // we found smaller digit in lhs
			return true;
		}
		if (i == 0) {
			end_flag = true;
			break;
		}
		i -= 1;
	}
	return false; 
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

std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs);

#if SUPPORT_IFSTREAM == 1
std::istream& operator>>(std::istream& lhs, BigNum& rhs); // bonus
#endif

int main()
{
	BigNum a = BigNum(50);
	BigNum b = BigNum(50);
	BigNum c = b;
	c.negative = true;
	b += a;
	BigNum x = BigNum(150);
	BigNum y = BigNum("+20");
	BigNum d = x + y;
	x = BigNum(931);
	y = BigNum(628);
	x += y;
	bool val = (BigNum(123) < BigNum(456));
	val = BigNum(1234) < BigNum(456);
	val = BigNum(-1234) < BigNum(456);
	x = BigNum(1234);
	y = BigNum(-456);
	val = x < y;
	val = BigNum(123) == BigNum(123);
	std::cout << 'test';
	return 0;
}
