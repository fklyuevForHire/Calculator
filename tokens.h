#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <variant>
#include <unordered_map>
#include <math.h>

namespace token_type {

struct Operator {

	Operator(char op) : op_(op) {}

	void setOperator(char op);

	char getOperator() const;

private:
	char op_;
};

struct Double {

	explicit Double(double value) : value_(value) {}

	void setDouble(double value);

	double getDouble() const;

private:	
	double value_;
};

} // namespace token_type


using TokenBase = std::variant<token_type::Operator, token_type::Double>;

struct Token : TokenBase {
	using TokenBase::TokenBase;

	template<typename T>
    bool Is() const {
        return std::holds_alternative<T>(*this);
    }

    template <typename T>
    const T& As() const {
        return std::get<T>(*this);
    }

    template <typename T>
    T& As() {
    	return std::get<T>(*this);
    }

    template <typename T>
    const T* tryAs() const {
        return std::get_if<T>(this);
    }
};

std::ostream& operator<<(std::ostream& out, const Token& token);

bool forbiddenSymbolsFound(const std::string& expr);