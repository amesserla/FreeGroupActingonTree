#pragma once
#include <string>

std::string invertWord(std::string word) {
	std::string inverse{};

	for (char c : word) {
		switch (c) {
		case 'a':
			inverse = "A" + inverse;
			break;
		case 'b':
			inverse = "B" + inverse;
			break;
		case 'A':
			inverse = "a" + inverse;
			break;
		case 'B':
			inverse = "b" + inverse;
			break;
		default:
			return "";
		}
	}

	return inverse;
}