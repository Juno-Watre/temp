#pragma

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <compiler/token.hpp>
#include <compiler/lexer.hpp>



Lexer::Lexer(std::string source) :sources(source) {
	std::cout << "Lexering..." << std::endl;
};


void Lexer::letex_analyzer() {
	Token temp_token;
	std::string word;
	size_t line_num = 1,roll_num = 0;
	size_t pos = 0;

	while (pos < sources.length()) {
		char c = sources[pos];

		if (isspace(c) ) {
			if (c == '\n') {
				line_num += 1;roll_num = 1;
			}
			else {
				roll_num += 1;
			}
			pos += 1;
			continue;
		}

		if (isalpha(c) || c == '_') {
			size_t start = pos;
			while (pos < sources.length() && isalnum(sources[pos]) || sources[pos] == '_') {
				pos += 1;
			}
			temp_token.lexeme = sources.substr(start, pos - start);
			// TODO : 在这里添加判断
			// now is default
			temp_token.type = TokenType::Identifier;
			temp_token.line = line_num;
			temp_token.column = roll_num;

			roll_num += (pos - start);
			tokens.push_back(temp_token);
			continue;
		}

		if (isdigit(c)) {
			size_t start = pos;
			while (pos < sources.length() && isdigit(sources[pos])) {
				pos++;
			}
			std::string num = sources.substr(start, pos - start);
			tokens.push_back({ TokenType::Integer, num, line_num, roll_num });
			roll_num += (pos - start);
			continue;
		}

		if (operators.find(std::string(1,c)) != operators.end())
		{
			std::string two_chars = sources.substr(pos, 2);
			if (operators.find(two_chars) != operators.end()) {
				pos += 2;
				tokens.push_back({ TokenType::Operator,two_chars,line_num,roll_num });
				roll_num += 2;
			}
			else { // single word
				tokens.push_back({ TokenType::Operator,std::string(1,c),line_num,roll_num });
				pos++;roll_num++;
			}
			continue;
		}
	}
}

