#include "calculator.h"

Token Calculator::Calculate (std::string expr) const {
	std::list<Token> output = shunting_yard_.performAlgorithm(parser_.parseLine(expr));

	using namespace token_type;
	while (output.size() > 1) {
		auto operator_it = std::find_if(output.begin(), output.end(), [](const Token& token){
			return token.Is<Operator>();
		});
		if (operator_it == output.end() || operator_it == output.begin()) {
			throw std::runtime_error("Operator not found");
		}

		char op = operator_it->As<Operator>().getOperator();

		auto back_it = operator_it;
		const Double* rhs = (--back_it)->tryAs<Double>();
		if (back_it == output.begin()) {
			throw std::runtime_error("Lhs part of expression not found");
		}
		const Double* lhs = (--back_it)->tryAs<Double>();
		if (lhs && rhs) {
			auto fn = ops_.at(op);
			back_it->As<Double>().setDouble(fn(lhs->getDouble(), rhs->getDouble()));
		} else {
			throw std::runtime_error("Lhs or rhs is not a number");
		}
		output.erase(++back_it, ++operator_it);
	}
	return output.front();
}
