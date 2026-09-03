#pragma once

#include <string>
#include <map>
#include <vector>
#include <compiler/token.hpp>


class Lexer {
public:
    Lexer(std::string source);
    void letex_analyzer();

    std::map<std::string, TokenType> keywords = {
        {"if", TokenType::If},
        {"else", TokenType::Else},
        {"while", TokenType::While},
        {"int", TokenType::Int_Key},
        {"return", TokenType::Return},
        {"float", TokenType::Float_Key},
        {"double", TokenType::Double_Key},
        {"char", TokenType::Char_Key},
        {"void", TokenType::Void_Key},
    };

    // operator map
    std::map<std::string, TokenType> operators = {
        {"+", TokenType::Plus},
        {"-", TokenType::Minus},
        {"==", TokenType::Operator},
        {"=", TokenType::Operator}
    };

    std::vector<Token> tokens;
    void display();
private:
    std::string sources;
};