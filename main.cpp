#include <iostream>
#include <utility>

#include "calculator.h"
#include "parser.h"
#include "tests.h"

int main() {
	Calculator calculator;
	std::string expr;

	runTests();
	while (std::getline(std::cin, expr)) {
		try {
			std::cout << calculator.Calculate(std::move(expr)) << std::endl;
		}
		catch(const ParserError& pe) {
			std::cout << pe.what() << std::endl;
		}
		catch(const std::runtime_error& re) {
			std::cout << re.what() << std::endl;
		}
	}
}