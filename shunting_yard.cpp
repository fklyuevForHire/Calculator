#include "shunting_yard.h"

bool ShuntingYard::isOpFound (char op) const {
	return op_precedence_.find(op) != op_precedence_.end();
}

ShuntingYard::Precedence ShuntingYard::getPrecedence(char op) const {
	return op_precedence_.at(op);
}

ShuntingYard::Precedence ShuntingYard::getTopPrecedence(const std::stack<Token>& operators) const {
	const token_type::Operator& top_op = operators.top().As<token_type::Operator>();
	return getPrecedence(top_op.getOperator());
}

std::list<Token> ShuntingYard::performAlgorithm(const std::vector<Token>& tokens) const {
	std::stack<Token> operators; 
	std::list<Token> output;

	using namespace token_type;
	for (auto token : tokens) {
		if (token.Is<Double>()) {
			output.push_back(token);
		} else {
			if (const auto ptr = token.tryAs<Operator>()){
				char op = ptr->getOperator();
				if (op == '(') {
					operators.push(token);
				} else if (op == ')') {
					while (!operators.empty() 
							&& operators.top().As<Operator>().getOperator() != '(') {
						output.push_back(operators.top()); 
						operators.pop();
					}
					if (operators.empty()) {
						throw std::runtime_error("Mismatched parenthesis");
					}
					operators.pop(); // Discarding left parenthesis
				} else {
					while (!operators.empty() 
							&& getTopPrecedence(operators) >= getPrecedence(op)) {
						output.push_back(operators.top());
						operators.pop();
					}
					operators.push(token);
				}
			} else {
				throw std::runtime_error("Invalid token");
			}	
		}
	}

	while (!operators.empty()) {
		output.push_back(operators.top());
		operators.pop();
	}
	return output;	
}