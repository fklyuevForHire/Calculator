#pragma once

#include <algorithm>
#include <functional>
#include <string>
#include <variant>
#include <stack>
#include <list>
#include <unordered_map>

#include "tokens.h"
#include "shunting_yard.h"
#include "parser.h"

class Calculator {
public:

	Token Calculate(std::string expr) const;

	using Operations = std::unordered_map<char, std::function<double(double, double)>>;

private:
	ShuntingYard shunting_yard_;
	Parser parser_;
	Operations ops_ = {{'+', [](double lhs, double rhs){return lhs + rhs;}},
					   {'-', [](double lhs, double rhs){return lhs - rhs;}},
					   {'*', [](double lhs, double rhs){return lhs * rhs;}},
					   {'/', [](double lhs, double rhs){if (rhs == 0.0){ throw std::runtime_error("Division by zero");} 
					   								  else {return lhs / rhs;}}}};

};