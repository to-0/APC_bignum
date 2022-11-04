
#include <iostream>
#include<optional>
#include<vector>
#pragma once
// here you can include whatever you want :)
#include <string>
#include <stdint.h>
#include <exception>
#include "BigNum.h"
int main()
{
	BigNum a = BigNum("15");
	BigNum b = BigNum("-8");
	BigNum c = BigNum();
	c = a * b;
	std::cout << c << "\n";
	a = BigNum("5");
	b = BigNum("-1");
	c = a + b;
	std::cout << c << "\n";

	a = BigNum("-3");
	b = BigNum("4");
	c = a + b;
	std::cout << c << "\n";

	a = BigNum("-6");
	b = BigNum("2");
	c = a + b;
	std::cout << c << "\n";

	a = BigNum("-5");
	b = BigNum("-5");
	c = a + b;
	std::cout << c << "\n";

	std::cout << c << "\n";
	std::cout << b << '\n';
	BigNum x = BigNum("45678956621564865422156498456486465165258789");
	BigNum y = BigNum("55649874894561232456478945312348793552569595818945");
	std::cout << x << "\n" << y << "\n";
	BigNum d = x + y;
	std::cout << d;
	x = BigNum(931);
	y = BigNum(628);
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
