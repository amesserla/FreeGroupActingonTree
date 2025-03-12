#pragma once
#include <iostream>
#include <string>
#include <regex>
#include <stack>

const std::regex allowedGenerators("[a | A | b | B]*");

void addLine() {
	std::cout << "what word should generate the new line?\n";
	std::string input{};
	std::cin >> input;

	if (!std::regex_match(input, allowedGenerators)) {
		std::cout << "Allowable words only contain a,A,b or B.\n";
		return;
	}


	std::stack<char> reduction{};
	for (char c : input) {
		if (!reduction.empty()) {
			if ((c == 'a' && reduction.top() == 'A') ||
				(c == 'b' && reduction.top() == 'B') ||
				(c == 'A' && reduction.top() == 'a') ||
				(c == 'B' && reduction.top() == 'b')) {
				reduction.pop();
				continue;
			}
			
		}
		reduction.push(c);
	}
	if (reduction.size() != input.size()) {
		std::cout << "we can reduce that to ";
		input = "";
		while (!reduction.empty()) {
			input = reduction.top() + input;
			reduction.pop();
		}
		std::cout << "this " << input << '\n';
	}
}