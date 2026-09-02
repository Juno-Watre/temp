#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <compiler/token.hpp>


class Lexer {
public:
    Lexer(std::string source);
    void letex_analyzer();

    std::map<std::string, TokenType> keywords = {
        {"if", TokenType::If},
        {"else", TokenType::Else},
        {"while", TokenType::While},
        {"int", TokenType::Int_},
        {"return", TokenType::Return},
        {"float", TokenType::Float_},
        {"double", TokenType::Double},
        {"char", TokenType::Char_},
        {"void", TokenType::Void_},
    };

    // operator map
    std::map<std::string, TokenType> operators = {
        {"+", TokenType::Operator},
        {"-", TokenType::Operator},
        {"==", TokenType::Operator},
        {"=", TokenType::Operator}
    };

    std::vector<Token> tokens;
    void display();
private:
    std::string sources;
};