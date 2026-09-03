#include <string>
#include <cctype>
#include <utility>
#include <vector>
#include <iostream>
#include <map>
#include <iomanip>

#include <compiler/token.hpp>
#include <compiler/lexer.hpp>
#include <compiler/tools.hpp>


Lexer::Lexer(std::string source) :sources(std::move(source)) {
    std::cout << "Lexering..." << '\n';
};

void Lexer::letex_analyzer() {
      tokens.clear();
      size_t line_num = 1, roll_num = 1, pos = 0;

      const std::map<std::string, TokenType> preprocessors = {
          {"include", TokenType::Include}, {"define",
          TokenType::Define},
          {"undef", TokenType::Undef}, {"ifdef", TokenType::IfDef},
          {"ifndef", TokenType::IfNDef}, {"else",
          TokenType::ElsePre},
          {"endif", TokenType::EndIf}, {"pragma",
          TokenType::Pragma},
          {"line", TokenType::Line}, {"error", TokenType::Error}
      };

      const std::map<std::string, TokenType> ops = {
          {"+", TokenType::Plus}, {"-", TokenType::Minus},
          {"*", TokenType::Multiply}, {"/", TokenType::Divide},
          {"%", TokenType::Modulo}, {"++", TokenType::Increment},
          {"--", TokenType::Decrement}, {"=", TokenType::Assign},
          {"+=", TokenType::AddAssign}, {"-=",
          TokenType::SubAssign},
          {"*=", TokenType::MulAssign}, {"/=",
          TokenType::DivAssign},
          {"%=", TokenType::ModAssign}, {"==", TokenType::Equal},
          {"!=", TokenType::NotEqual}, {">", TokenType::Greater},
          {"<", TokenType::Less}, {">=", TokenType::GreaterEqual},
          {"<=", TokenType::LessEqual}, {"&&",
          TokenType::LogicalAnd},
          {"||", TokenType::LogicalOr}, {"!",
          TokenType::LogicalNot},
          {"&", TokenType::BitwiseAnd}, {"|", TokenType::BitwiseOr},
          {"^", TokenType::BitwiseXor}, {"~",
          TokenType::BitwiseNot},
          {"<<", TokenType::ShiftLeft}, {">>",
          TokenType::ShiftRight},
          {"->", TokenType::Arrow}, {"?", TokenType::QuestionMark},
          {":", TokenType::Colon}
      };

      const std::map<char, TokenType> punctuation = {
          {'(', TokenType::ParenOpen}, {')', TokenType::ParenClose},
          {'{', TokenType::BraceOpen}, {'}', TokenType::BraceClose},
          {'[', TokenType::BracketOpen}, {']',
          TokenType::BracketClose},
          {';', TokenType::Semicolon}, {',', TokenType::CommaSep},
          {'.', TokenType::DotSep}, {'@', TokenType::AtSign}
      };

      while (pos < sources.length()) {
          const char ch = sources[pos];
          const auto uch = static_cast<unsigned char>(ch);

          if (std::isspace(uch)) {
              if (ch == '\n') {
                  ++line_num;
                  roll_num = 1;
              } else {
                  ++roll_num;
              }
              ++pos;
              continue;
          }

          if (std::isalpha(uch) || ch == '_') {
              const size_t start = pos;
              const size_t column = roll_num;
              while (pos < sources.length()) {
                  const auto current = static_cast<unsigned
                  char>(sources[pos]);
                  if (!std::isalnum(current) && sources[pos] != '_') {
                    break;
                }
                  ++pos;
                  ++roll_num;
              }

              const std::string word = sources.substr(start, pos -
              start);
              const auto keyword = keywords.find(word);
              tokens.push_back({
                  keyword == keywords.end() ?
                  TokenType::Identifier : keyword->second,
                  word, line_num, column
              });
              continue;
          }

          if (std::isdigit(uch) != 0) {
              const size_t start = pos;
              const size_t column = roll_num;
              while (pos < sources.length() &&
                     (std::isdigit(static_cast<unsigned
                     char>(sources[pos])) != 0)) {
                  ++pos;
                  ++roll_num;
              }
              tokens.push_back(
                  {TokenType::Integer, sources.substr(start, pos -
                  start), line_num, column});
              continue;
          }

          if (ch == '"' || ch == '\'') {
              const size_t start = pos;
              const size_t start_line = line_num;
              const size_t column = roll_num;
              const char quote = ch;
              bool closed = false;
              ++pos;
              ++roll_num;

              while (pos < sources.length()) {
                  if (sources[pos] == '\\' && pos + 1 <
                  sources.length()) {
                      pos += 2;
                      roll_num += 2;
                      continue;
                  }
                  if (sources[pos] == quote) {
                      ++pos;
                      ++roll_num;
                      closed = true;
                      break;
                  }
                  if (sources[pos] == '\n') {
                      ++pos;
                      ++line_num;
                      roll_num = 1;
                  } else {
                      ++pos;
                      ++roll_num;
                  }
              }

              tokens.push_back({
                  closed ? (quote == '"' ? TokenType::String :
                  TokenType::Char)
                         : TokenType::Error_type,
                  sources.substr(start, pos - start), start_line,
                  column
              });
              continue;
          }

          if (ch == '#') {
              const size_t start = pos;
              const size_t column = roll_num;
              ++pos;
              ++roll_num;
              while (pos < sources.length() &&
                     (sources[pos] == ' ' || sources[pos] == '\t'))
                     {
                  ++pos;
                  ++roll_num;
              }

              const size_t name_start = pos;
              while (pos < sources.length() &&
                     std::isalpha(static_cast<unsigned
                     char>(sources[pos]))) {
                  ++pos;
                  ++roll_num;
              }

              const auto directive = preprocessors.find(
                  sources.substr(name_start, pos - name_start));
              tokens.push_back({
                  directive == preprocessors.end() ?
                  TokenType::Unknown : directive->second,
                  sources.substr(start, pos - start), line_num,
                  column
              });
              continue;
          }

          if (ch == '/' && pos + 1 < sources.length() && sources[pos
          + 1] == '/') {
              const size_t start = pos;
              const size_t column = roll_num;
              pos += 2;
              roll_num += 2;
              while (pos < sources.length() && sources[pos] != '\n')
              {
                  ++pos;
                  ++roll_num;
              }
              tokens.push_back(
                  {TokenType::Comment, sources.substr(start, pos -
                  start), line_num, column});
              continue;
          }

          if (ch == '/' && pos + 1 < sources.length() && sources[pos
          + 1] == '*') {
              const size_t start = pos;
              const size_t start_line = line_num;
              const size_t column = roll_num;
              bool closed = false;
              pos += 2;
              roll_num += 2;

              while (pos < sources.length()) {
                  if (sources[pos] == '*' && pos + 1 <
                  sources.length() &&
                      sources[pos + 1] == '/') {
                      pos += 2;
                      roll_num += 2;
                      closed = true;
                      break;
                  }
                  if (sources[pos] == '\n') {
                      ++pos;
                      ++line_num;
                      roll_num = 1;
                  } else {
                      ++pos;
                      ++roll_num;
                  }
              }

              tokens.push_back({
                  closed ? TokenType::Comment :
                  TokenType::Error_type,
                  sources.substr(start, pos - start), start_line,
                  column
              });
              continue;
          }

          const std::string two_chars = sources.substr(pos, 2);
          const auto two_char_op = ops.find(two_chars);
          if (two_char_op != ops.end()) {
              tokens.push_back({two_char_op->second, two_chars,
              line_num, roll_num});
              pos += 2;
              roll_num += 2;
              continue;
          }

          const std::string one_char(1, ch);
          const auto one_char_op = ops.find(one_char);
          if (one_char_op != ops.end()) {
              tokens.push_back({one_char_op->second, one_char,
              line_num, roll_num});
              ++pos;
              ++roll_num;
              continue;
          }

          const auto mark = punctuation.find(ch);
          if (mark != punctuation.end()) {
              tokens.push_back({mark->second, one_char, line_num,
              roll_num});
              ++pos;
              ++roll_num;
              continue;
          }

          tokens.push_back({TokenType::Error_type, one_char,
          line_num, roll_num});
          ++pos;
          ++roll_num;
      }

      tokens.push_back({TokenType::EndOfFile, "", line_num,
      roll_num});
  }


void Lexer::display() {
    // 1. 打印表头
    std::cout << "\n========== Token List ==========\n";
    std::cout << std::left 
              << std::setw(8) << "Line" 
              << std::setw(8) << "Col" 
              << std::setw(20) << "Lexeme" 
              << "Type\n";
    std::cout << "----------------------------------\n";

    // 2. 遍历并格式化输出每个 Token
    for (const auto& token : tokens) { // 使用 const auto& 避免拷贝
        std::cout << std::left 
                  << std::setw(8) << token.line 
                  << std::setw(8) << token.column 
                  << std::setw(20) << token.lexeme 
                  << ToolsClass::tokenTypeToString(token.type) << '\n';
    }

    // 3. 打印结束标记
    std::cout << "==================================\n\n";
}