#include "tokens.h"

static double Round(double var) {
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67

    double value = var >= 0 ? static_cast<int>(var * 100 + .5)
    						  : static_cast<int>(var * 100 - .5);
    return static_cast<double>(value / 100);
}

namespace token_type {

void Operator::setOperator(char op) {
	op_ = op;
}

char Operator::getOperator() const {
	return op_;
}

void Double::setDouble(double value) {
	value_ = value;
}

double Double::getDouble() const {
	return value_;
}

} // namespace token_type

namespace {

using namespace std::literals;
std::unordered_map<char, std::string> operator_desc = {{'+', "Addition"s},
													   {'-', "Subtraction"s},
													   {'*', "Multiplication"s},
													   {'/', "Division"s},
													   {'(', "LeftParenthesis"s},
													   {')', "RightParenthesis"s}};
} // namespace


bool isInteger(double value) {
	return ceilf(value) == value;
}


std::ostream& operator<<(std::ostream& out, const Token& token) {
	if (const auto ptr = token.tryAs<token_type::Operator>()) {
		out << operator_desc.at(ptr->getOperator());
	} else if (const auto ptr = token.tryAs<token_type::Double>()){
		double result = ptr->getDouble();
		double rounded_result = Round(result);
		if (isInteger(rounded_result)){
			out << static_cast<int>(rounded_result);
		} else {
			out << std::setprecision(2) << std::fixed << rounded_result;
		}
	}
	return out;
}
