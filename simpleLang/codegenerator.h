
#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <string>
#include <sstream>
#include <map> 
#include "parser.h" 

class CodeGenerator {
public:
  CodeGenerator() : current_label_index(0) {}

  std::string generateCode(const ASTNode& root); 

private:
  void generateCodeForNode(const ASTNode& node, std::stringstream& assemblyCode);
  std::string generateNewLabel(const std::string& prefix);

  int current_label_index;

  // Declare inst and reg as static members
  static std::map<std::string, int> inst;
  static std::map<std::string, int> reg;
};

#endif 