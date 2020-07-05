// Copyright (C) 2020 Maxim, 2dev2fun@gmail.com. All rights reserved.

#include "String.h"

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <vector>

using namespace simple;

namespace std {

std::istream& getline(std::istream& in, String& string, char delimiter = '\n') {
	char symbol;

	string = "";

	while (in.get(symbol)) {
		if (symbol == delimiter) {
			break;
		}
		string.append(symbol);
	}

	return in;
}

} // namespace std

void read_file(char const* path) {
	std::ifstream file(path);

	std::vector<String> book;

	if (file) {
		String line;

		while (getline(file, line)) {
			book.push_back(line);
		}

		std::sort(book.begin(), book.end(), [] (String const& a, String const& b) {
				String a_temp = a;
				String b_temp = b;

				a_temp.to_lower();
				b_temp.to_lower();

				return a_temp > b_temp;
		});

		for (auto const& e : book) {
			std::cout << e << std::endl;
		}
	}
}

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "Should specify path of the file with text!" << std::endl;
		return 0;
	}

	try {
		read_file(argv[1]);
	} catch (std::exception const& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
