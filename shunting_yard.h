#pragma once

#include <list>
#include <stack>
#include <unordered_map>

#include "tokens.h"

class ShuntingYard {
public:
	std::list<Token> performAlgorithm(const std::vector<Token>& tokens) const;

	using Precedence = uint8_t;

private:

	bool isOpFound(char op) const;

	Precedence getPrecedence(char op) const;

	Precedence getTopPrecedence(const std::stack<Token>& operators) const ;

	std::unordered_map<char, Precedence> op_precedence_ = {{'(', 0},
														   {')', 0},
														   {'+', 1},
														   {'-', 1},
														   {'*', 2},
														   {'/', 2}};
};