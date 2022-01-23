#pragma once

#include <cassert>
#include <iostream>
#include <string>

#include "calculator.h"

template<typename Fn>
void printTestSuccess(const Fn& fn) {
	std::cerr << fn << " OK!\n";
}

template<typename Fn>
void printTestFailure(const Fn& fn) {
	std::cerr << fn << " FAIL!\n";
}

void testEmpty(const Calculator& calc) {
	calc.Calculate("");
	printTestSuccess(__func__);
}

void testWS(const Calculator& calc) {	
	std::string expected = "9";
	std::stringstream str;
	{
		str << calc.Calculate("      ");
		assert("" == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("     5 + 4");
		assert(expected == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("5 + 4    ");
		assert(expected == str.str());
	}
	str.str(std::string());
	{
		//tabs included
		str << calc.Calculate("   5     +  		 4   ");
		assert(expected == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("1*1+1+1+1+1-2+1+1+1+1+1*1+1");
		assert(expected == str.str());
	}	
	printTestSuccess(__func__);
}

void testParsingErrorHandling(const Calculator& calc) {
	std::vector<std::string> requests = {"+", "k", "()", "13+",
										"31++", "(15)()", "14 14",
										"(14+32-)", "13 12 +", 
									 	"5 000 + 4", "142 ++ 3"};
	for (auto& request : requests) {
		try {
			calc.Calculate(std::move(request));
		}
		catch (...) {
			continue;
		}
		// means that error was not caught
		printTestFailure(__func__);
	}
	printTestSuccess(__func__);
}

void testParenthesis(const Calculator& calc) {
	std::vector<std::string> invalid_requests = {"((1)", "1 + (1+5))", "("};

	for (auto& request : invalid_requests) {
		try {
			calc.Calculate(std::move(request));
		}
		catch(...) {
			continue;
		}
		printTestFailure(__func__);
	}

	std::string expected = "100";
	std::stringstream str;
	{
		str << calc.Calculate("(50)+(50)");
		assert(expected == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("(50 + 50)");
		assert(expected == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("((((((50 + 10)+10)+10) + 10)) )+10");
		assert(expected == str.str());
	}
	str.str(std::string());
	{
		// gets converted into 2 * (50);
		str << calc.Calculate("2(50)");
		assert(expected == str.str());
	}
	str.str(std::string());
	{
		// gets converted into (1 + 1) * (50);
		str << calc.Calculate("(+1 + 1)(50)");
		assert(expected == str.str());
	}
	printTestSuccess(__func__);
}

void testNegative(const Calculator& calc) {
	std::string expected = "-9";
	std::stringstream str;
	{
		str << calc.Calculate("5 - 14");
		assert(expected == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("-(5 + 4)");
		assert(expected == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("-5 - 4");
		assert(expected == str.str());
	}
	printTestSuccess(__func__);		
}

void testFloats(const Calculator& calc) {
	std::stringstream str;
	{
		str << calc.Calculate("2.00000 - 2");
		assert("0" == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("1.9939999 + 1");
		assert("2.99" == str.str());
	}
	str.str(std::string());
	{
		// gets rounded
		str << calc.Calculate("-1,25 - 3,749");
		assert("-5" == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("113,23516 * 132,12561");
		assert("14961.26"== str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("1.755 + 0.246");
		assert("2"== str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate(".11 + .1");
		assert("0.21" == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("1,25 + 1,25");
		assert("2.50" == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("1,554 + 0");
		assert("1.55" == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("1,555 + 0");
		assert("1.56" == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("1,555 * 0");
		assert("0" == str.str());
	}			
	printTestSuccess(__func__);	
}

void testDivideByZero(const Calculator& calc) {
	try {
		calc.Calculate("15 / (5,00 - 5.000)");
	}
	catch(...) {
		printTestSuccess(__func__);
		return;
	}
	printTestFailure(__func__);
}

void testLargeNumbers(const Calculator& calc) {
	std::stringstream str;
	{
		str << calc.Calculate("10000000,25121231 - 99,12591295912");
		assert("9999901.13" == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("-99999999,99999 + 99999999,2414");
		assert("-0.76" == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("1003231,231 * 10,161 + (999999,1241 - 1321.151)  - 12.5132115123");
		assert("11192498" == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("2.51231312151241231245152131213124123 + 1.1251159149192391391959194912491991239193123");
		assert("3.64" == str.str());
	}
	printTestSuccess(__func__);
}

void testExpressions(const Calculator& calc) {
	std::stringstream str;
	{
		str << calc.Calculate("(112412.31 + 3131)5 -12521 +3(15(11231.31 - 31.5151321) + 1) - 123");
		assert("1069066.32" == str.str());
	}
	str.str(std::string());
	{
		str << calc.Calculate("1301032/1321+12313123+3(1241.213/886)+9613/5");
		assert("12316034.69" == str.str());
	}
	printTestSuccess(__func__);
}

void runTests() {
	Calculator calc;

	testEmpty(calc);
	testWS(calc);
	testParsingErrorHandling(calc);
	testParenthesis(calc);
	testNegative(calc);
	testFloats(calc);
	testDivideByZero(calc);
	testLargeNumbers(calc);
	testExpressions(calc);
}