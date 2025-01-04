// parser.cpp
#include "parser.h"
#include <iostream> // Include for runtime_error

using namespace std;

Parser::Parser(const vector<Token>& tokens) : tokens(tokens), current_token_index(0) {}

ASTNode Parser::parse() {
  return parseProgram();
}

ASTNode Parser::parseProgram() {
  ASTNode programNode = {"PROGRAM", "", {}};
  while (current_token_index < tokens.size()) {
    programNode.children.push_back(parseStatement());
  }
  return programNode;
}

ASTNode Parser::parseStatement() {
  if (current_token().type == "INT") {
    return parseVariableDeclaration();
  } else if (current_token().type == "IDENTIFIER") {
    return parseAssignment();
  } else if (current_token().type == "IF") {
    return parseConditional();
  } else {
    throw runtime_error("Invalid statement starting with: " + current_token().value);
  }
}

ASTNode Parser::parseVariableDeclaration() {
  ASTNode declarationNode = {"VAR_DECL", "", {}};
  consume("INT");
  declarationNode.children.push_back({"IDENTIFIER", current_token().value, {}});
  consume("IDENTIFIER");
  consume("ASSIGN"); // Consume the assignment operator
  declarationNode.children.push_back(parseExpression()); // Parse the initialization expression
  consume("SEMICOLON");
  return declarationNode;
}

ASTNode Parser::parseAssignment() {
  ASTNode assignmentNode = {"ASSIGNMENT", "", {}};
  assignmentNode.children.push_back({"IDENTIFIER", current_token().value, {}});
  consume("IDENTIFIER");
  consume("ASSIGN");
  assignmentNode.children.push_back(parseExpression());
  consume("SEMICOLON");
  return assignmentNode;
}

ASTNode Parser::parseExpression() {
  ASTNode expressionNode = parseTerm();

  while (current_token().type == "OPERATOR" && 
         (current_token().value == "+" || current_token().value == "-")) {
    expressionNode = {
        "BINARY_OP", current_token().value, {expressionNode, parseTerm()}};
  }

  return expressionNode;
}

ASTNode Parser::parseTerm() {
  if (current_token().type == "IDENTIFIER") {
    ASTNode identifierNode = {"IDENTIFIER", current_token().value, {}};
    consume("IDENTIFIER");
    return identifierNode;
  } else {
    throw runtime_error("Expected identifier in expression, found: " + current_token().value);
  }
}

ASTNode Parser::parseConditional() {
  ASTNode conditionalNode = {"IF", "", {}};
  consume("IF");
  consume("LPAREN");
  conditionalNode.children.push_back(parseCondition());
  consume("RPAREN");
  consume("LBRACE");
  while (current_token().type != "RBRACE") {
    conditionalNode.children.push_back(parseStatement());
  }
  consume("RBRACE");
  return conditionalNode;
}

ASTNode Parser::parseCondition() {
  ASTNode conditionNode = {"CONDITION", "", {}};
  conditionNode.children.push_back({"IDENTIFIER", current_token().value, {}});
  consume("IDENTIFIER");
  consume("EQUAL");
  conditionNode.children.push_back({"IDENTIFIER", current_token().value, {}});
  consume("IDENTIFIER");
  return conditionNode;
}

Token Parser::current_token() {
  return tokens[current_token_index];
}

void Parser::consume(const string& expected_type) {
  if (current_token().type == expected_type) {
    current_token_index++;
  } else {
    throw runtime_error("Expected token type: " + expected_type +
                        ", but found: " + current_token().type);
  }
}