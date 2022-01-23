#include "parser.h"

bool Parser::forbiddenSymbolFound(std::string_view expr) const {
	for (char c : expr) {
		if (!std::isdigit(c)){
			if (!allowed_symbols_.Contains(c)) {
				return true;
			}
		}
	}
	return false;
}

bool Parser::wrongTokenOrder(const std::vector<Token>& tokens) const {
	bool number_encountered = false;
	bool is_number = false;
	bool is_op = false;
	using namespace token_type;
	for (const auto& token : tokens) {
		if (token.Is<Double>()) {
			is_op = false;
			is_number = true;
			number_encountered = true;
		} else if (token.Is<Operator>() &&  token.As<Operator>().getOperator() != '('
										   && token.As<Operator>().getOperator() != ')') {
			if (!number_encountered || is_op) {
				return true;
			}
			is_op = true;
			is_number = false;
		}
	}
	// handling () case
	if (!is_number && tokens.size() > 0) {
		return true;
	}
	// Operator cannot be last
	if (is_op) {
		return true;
	}
	return false;
}

std::pair<Token, TOKEN_TYPE> Parser::parseToken(std::string_view token_literal) const {
	std::string token_str;
	uint8_t floating_points = 0;
	bool is_number = true;
	for (char c : token_literal) {
		if (!std::isdigit(c) && !floating_symbols_.Contains(c)){
			is_number = false;
		}
		if (floating_symbols_.Contains(c)) {
			token_str += '.';
			floating_points++;
		} else {
			token_str += c;
		}
	}
	if (floating_points > 1) {
		throw ParserError("Multiple floating points in single number");
	} else if (is_number) {
		return {token_type::Double(std::stod(token_str)), TOKEN_TYPE::NUMBER};
	} else {
		TOKEN_TYPE type;
		assert(token_str.size() == 1);
		switch (token_str[0]) {
			case '+' :
				type = TOKEN_TYPE::ADD;
				break;
			case '-' :
				type = TOKEN_TYPE::SUBTRACT;
				break;
			case '*' :
				type = TOKEN_TYPE::MULTIPLY;
				break;
			case '/' :
				type = TOKEN_TYPE::DIVIDE;
				break;
			case '(' :
				type = TOKEN_TYPE::LEFT_PAR;
				break;
			case ')':
				type = TOKEN_TYPE::RIGHT_PAR;
				break;
		}
		return {token_type::Operator(token_str[0]), type};
	}
}

std::vector<Token> Parser::parseLine(std::string_view expr) const {
	using namespace token_type;
	if (forbiddenSymbolFound(expr)){
		throw ParserError("Forbidden symbols found");
	}
	std::vector<std::string_view> splitted_line = splitLine(expr);
	std::vector<Token> tokens;
	bool negative_sign = false;
	bool positive_sign = false;
	TOKEN_TYPE prev_type = TOKEN_TYPE::NONE;
	for (auto token_literal : splitted_line) {
		auto [token, type] = parseToken(token_literal);
		if (negative_sign || positive_sign){
			positive_sign = false;
			// means either -(1 + 5) or (-1) or +1 + 5 cases
			tokens.pop_back(); //popping minus or plus
			if (negative_sign) {
				negative_sign = false;
				if (type == TOKEN_TYPE::LEFT_PAR) {
					// handling case: -(1 + 5) where expr is substituted with -1 * (1 + 5)
					tokens.push_back({Double(-1)});
					tokens.push_back({Operator('*')});
				} else if (type == TOKEN_TYPE::NUMBER) {
					token.As<Double>().setDouble(token.As<Double>().getDouble() * -1);
				} else {
					throw ParserError("Wrong token after negative sign");
				}
			}
		} else if (type == TOKEN_TYPE::SUBTRACT) {
			if (tokens.size() == 0 || prev_type == TOKEN_TYPE::LEFT_PAR) {
				// means either -(1 + 5) or (-1) or -1 + 5 cases
				negative_sign = true;
			}
		} else if (type == TOKEN_TYPE::ADD) {
			if (tokens.size() == 0 || prev_type == TOKEN_TYPE::LEFT_PAR) {
				// means either -(1 + 5) or (-1) or -1 + 5 cases
				positive_sign = true;
			}
		} else if (type == TOKEN_TYPE::LEFT_PAR){
			// handling case (2)(5) or 2(5) where * is inserted before right par
			if (prev_type == TOKEN_TYPE::NUMBER || prev_type == TOKEN_TYPE::RIGHT_PAR) {
				tokens.push_back({Operator('*')});
			}
		} else if (type == TOKEN_TYPE::NUMBER && prev_type == TOKEN_TYPE::RIGHT_PAR) {
			// handling (3)5 == (3) * 5
			tokens.push_back({Operator('*')});
		}
		tokens.push_back(token);
		prev_type = type;
	}
	if (wrongTokenOrder(tokens)){
		throw ParserError("Wrong token order or only operator expr");
	}
	return tokens;
}

std::string_view Parser::cutOffWhiteSpaces(std::string_view expr) const {
	for (size_t i = 0; i < expr.size(); i++) {
		if (expr[i] != ' ' && expr[i] != '\t') {
			return expr.substr(i);
		}
	}
	return std::string_view{};
}

size_t Parser::getDelimPos(std::string_view expr) const {
	for (size_t i = 0; i < expr.size(); i++) {
		char c = expr[i];
		if (delim_symbols_.Contains(c)){
			return i;
		}
	}
	return expr.npos;
}

std::vector<std::string_view> Parser::splitLine(std::string_view expr) const {
	std::vector<std::string_view> result;
    while (expr.size() > 0) {
    	expr = cutOffWhiteSpaces(expr);
    	if (expr.empty()) {
    		break ;
    	}
    	size_t delim = getDelimPos(expr);
    	if (delim == expr.npos) {
    		result.push_back(expr);
    		break;
    	} else {
    		if (delim == 0) {
    			delim += 1;
    		}
    		result.push_back(expr.substr(0, delim));
    		expr = expr.substr(delim);
    	}
    }
	return result;
}
