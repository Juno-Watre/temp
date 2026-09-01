#include <string>
#include <map>
#include <iostream>
#include <compiler/token.hpp>


class Lexer {
public:
	Lexer(std::string source) :sources(source) {};
	void letex_analyzer();

	std::map<std::string, TokenType> keywords = {
		{"if", TokenType::Keyword},
		{"else", TokenType::Keyword},
		{"while", TokenType::Keyword}
	};

	// ‘ÀÀ„∑˚”≥…‰±Ì
	std::map<std::string, TokenType> operators = {
		{"+", TokenType::Operator},
		{"-", TokenType::Operator},
		{"==", TokenType::Operator},
		{"=", TokenType::Operator}
	};

	std::vector<Token> tokens;
private:
	std::string sources;
};