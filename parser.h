#pragma once

#include <algorithm>
#include <cassert>
#include <vector>
#include <string_view>
#include <iostream>
#include <unordered_set>
#include <utility>

#include "tokens.h"

class Set : public std::unordered_set<char> {
public:
	using unordered_set::unordered_set;

	bool Contains(char c) const {
		return this->find(c) != this->end();
	}
};

class ParserError : public std::runtime_error {
	using runtime_error::runtime_error;
};

enum class TOKEN_TYPE {
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	LEFT_PAR,
	RIGHT_PAR,
	NUMBER,
	NONE
};

class Parser {
public:
	std::vector<Token> parseLine(std::string_view expr) const ;

private:
	Set allowed_symbols_ = {'+', '-', '*', '/', '(', ')', '.', ',', ' ', '\t'};
	Set delim_symbols_ = {'+', '-', '*', '/', '(', ')', ' ', '\t'};
	Set floating_symbols_ = {'.', ','};

	bool forbiddenSymbolFound(std::string_view expr) const ;

	std::vector<std::string_view> splitLine(std::string_view expr) const ;

	bool wrongTokenOrder(const std::vector<Token>& tokens) const ;

	std::pair<Token, TOKEN_TYPE> parseToken(std::string_view token_literal) const ;

	std::string_view cutOffWhiteSpaces(std::string_view expr) const;

	size_t getDelimPos(std::string_view expr) const;
};
