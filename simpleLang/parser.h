#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <string>

using namespace std;

struct ASTNode {
  string type;
  string value;
  vector<ASTNode> children;
};

class Parser {
public:
  Parser(const vector<Token>& tokens);
  ASTNode parse();

private:
  vector<Token> tokens;
  int current_token_index;

  ASTNode parseProgram();
  ASTNode parseStatement();
  ASTNode parseVariableDeclaration();
  ASTNode parseAssignment();
  ASTNode parseExpression();
  ASTNode parseTerm();
  ASTNode parseConditional();
  ASTNode parseCondition();
  Token current_token();
  void consume(const string& expected_type);
};

#endif // PARSER_H