#include "lexer.h"
#include <iostream>
#include <unordered_map>

using namespace std;

vector<Token> lex(const string& code) {
  vector<Token> tokens;
  string current_token = "";

  unordered_map<string, string> keywords = {
    {"int", "INT"},
    {"if", "IF"},
  };

  for (int i = 0; i < code.length(); ++i) {
    char c = code[i];

    if (isalnum(c) || c == '_') {
      current_token += c;
    } else {
      if (!current_token.empty()) {
        if (keywords.count(current_token)) {
          tokens.push_back({keywords[current_token], current_token});
        } else {
          tokens.push_back({"IDENTIFIER", current_token});
        }
        current_token = "";
      }

      if (c == '+') {
          tokens.push_back({"OPERATOR", string(1, c)});
      } else if (c == '-') { // Corrected to single '-'
          tokens.push_back({"OPERATOR", string(1, c)});
      } else if (c == '=') {
          if (!tokens.empty() && tokens.back().type == "ASSIGN") {
              tokens.back().type = "EQUAL";
              tokens.back().value = "==";
          } else {
              tokens.push_back({"ASSIGN", string(1, c)});
          }
      } else if (c == ';') {
          tokens.push_back({"SEMICOLON", string(1, c)});
      } else if (c == '(') {
          tokens.push_back({"LPAREN", string(1, c)});
      } else if (c == ')') {
          tokens.push_back({"RPAREN", string(1, c)});
      } else if (c == '{') {
          tokens.push_back({"LBRACE", string(1, c)});
      } else if (c == '}') {
          tokens.push_back({"RBRACE", string(1, c)});
      } else if (isdigit(c)) {
          string num_str;
          num_str += c;
          while (i + 1 < code.length() && isdigit(code[i + 1])) {
              num_str += code[++i];
          }
          tokens.push_back({"NUMBER", num_str});
      } else if (!isspace(c)) {
          throw runtime_error("Invalid character: " + string(1, c));
      }
    }
  }

  if (!current_token.empty()) {
    if (keywords.count(current_token)) {
      tokens.push_back({keywords[current_token], current_token});
    } else {
      tokens.push_back({"IDENTIFIER", current_token});
    }
  }

  return tokens;
}