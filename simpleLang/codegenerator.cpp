#include "codegenerator.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map> 

std::map<std::string, int> CodeGenerator::inst = {
    {"call", 0b00000001},
    {"ret", 0b00000010},
    {"lda", 0b10000111},
    {"out", 0b00000011},
    {"in", 0b00000100},
    {"hlt", 0b00000101},
    {"cmp", 0b00000110},
    {"sta", 0b10111000},
    {"jmp", 0b00011000},
    {"jz", 0b00011001},
    {"jnz", 0b00011010},
    {"je",  0b00011001},
    {"jne", 0b00011010}, 
    {"nop", 0b00010000}, 
    {"jc",  0b00011011},
    {"jnc", 0b00011100},
    {"push", 0b00100000},
    {"pop", 0b00101000},
    {"add", 0b01000000},
    {"sub", 0b01001000},
    {"inc", 0b01010000},
    {"dec", 0b01011000},
    {"and", 0b01100000},
    {"or", 0b01101000},
    {"xor", 0b01110000},
    {"adc", 0b01111000},
    {"ldi", 0b00010000},
    {"mov", 0b10000000},
};

std::map<std::string, int> CodeGenerator::reg = {
    {"A", 0b000},
    {"B", 0b001},
    {"C", 0b010},
    {"D", 0b011},
    {"E", 0b100},
    {"F", 0b101},
    {"G", 0b110},
    {"M", 0b111},
};

std::string CodeGenerator::generateCode(const ASTNode& ast) {
    std::stringstream code;
    current_label_index = 0; 

    generateCodeForNode(ast, code);

    return code.str();
}

void CodeGenerator::generateCodeForNode(const ASTNode& node, std::stringstream& code) {
    if (node.type == "PROGRAM") {
        code << ".text" << std::endl; 
        for (const auto& child : node.children) {
            generateCodeForNode(child, code);
        }
    } else if (node.type == "VAR_DECL") {
        code << ".data" << std::endl; 
        code << node.children[0].value << " = "  << node.children[1].value << std::endl; 
    } else if (node.type == "ASSIGNMENT") {
        generateCodeForNode(node.children[1], code);
        code << "sta " << node.children[0].value << std::endl;
    } else if (node.type == "BINARY_OP") {
        generateCodeForNode(node.children[0], code);
        code << "push A" << std::endl;
        generateCodeForNode(node.children[1], code);
        code << "pop B" << std::endl;

        if (node.value == "+") {
            int opcode = CodeGenerator::inst["add"] | (CodeGenerator::reg["A"] << 3) | CodeGenerator::reg["B"]; 
            code << "ldi A, " << opcode << std::endl; 
        } else if (node.value == "-") {
            int opcode = CodeGenerator::inst["sub"] | (CodeGenerator::reg["A"] << 3) | CodeGenerator::reg["B"];
            code << "ldi A, " << opcode << std::endl; 
        } else if (node.value == "*") {
            // Multiplication using repeated addition
            std::string loopLabel = generateNewLabel("loop");
            std::string endLabel = generateNewLabel("end");

            generateCodeForNode(node.children[0], code);
            code << "push A" << std::endl;
            generateCodeForNode(node.children[1], code);
            code << "pop B" << std::endl;

            code << "ldi C, 0" << std::endl; 

            code << loopLabel << ":" << std::endl;
            code << "cmp C, B" << std::endl; 
            code << "je " << endLabel << std::endl; 

            code << "add A, A" << std::endl;  
            code << "inc C" << std::endl; 
            code << "jmp " << loopLabel << std::endl; 

            code << endLabel << ":" << std::endl;
        } else if (node.value == "/") {
            // Division using repeated subtraction
            std::string loopLabel = generateNewLabel("loop");
            std::string endLabel = generateNewLabel("end");

            generateCodeForNode(node.children[0], code);
            code << "push A" << std::endl;
            generateCodeForNode(node.children[1], code);
            code << "pop B" << std::endl;

            code << "ldi C, 0" << std::endl; 

            code << loopLabel << ":" << std::endl;
            code << "cmp A, B" << std::endl; 
            code << "jl " << endLabel << std::endl; 

            code << "sub A, B" << std::endl;  // You might need to adjust this
            code << "inc C" << std::endl; 
            code << "jmp " << loopLabel << std::endl; 

            code << endLabel << ":" << std::endl;
            code << "mov A, C" << std::endl; 
        }
    } else if (node.type == "IDENTIFIER") {
        code << "lda " << node.value << std::endl;
    } else if (node.type == "IF") {
        std::string label_else = generateNewLabel("else");
        std::string label_end = generateNewLabel("endif");

        generateCodeForNode(node.children[0], code);
        code << "jz " << label_else << std::endl;

        generateCodeForNode(node.children[1], code);
        code << "jmp " << label_end << std::endl;

        code << label_else << ":" << std::endl;
        if (node.children.size() > 2) {
            generateCodeForNode(node.children[2], code);
        }

        code << label_end << ":" << std::endl;
    } else if (node.type == "CONDITION") {
        generateCodeForNode(node.children[0], code);
        code << "push A" << std::endl;
        generateCodeForNode(node.children[1], code);
        code << "pop B" << std::endl;

        std::string labelTrue = generateNewLabel("true");

        if (node.value == "==") {
            code << "cmp A, B" << std::endl;
            code << "je " << labelTrue << std::endl;
        } else if (node.value == "!=") {
            code << "cmp A, B" << std::endl;
            code << "jne " << labelTrue << std::endl;
        } else if (node.value == "<") {
            code << "cmp A, B" << std::endl;
            code << "jlt " << labelTrue << std::endl;
        } else if (node.value == ">") {
            code << "cmp A, B" << std::endl;
            code << "jgt " << labelTrue << std::endl;
        } else if (node.value == "<=") {
            code << "cmp A, B" << std::endl;
            code << "jle " << labelTrue << std::endl;
        } else if (node.value == ">=") {
            code << "cmp A, B" << std::endl;
            code << "jge " << labelTrue << std::endl;
        }
        code << labelTrue << ":" << std::endl;
    } else if (node.type == "NUMBER") {
        code << "ldi A, " << node.value << std::endl;
    }
}

std::string CodeGenerator::generateNewLabel(const std::string& prefix) {
    return prefix + "_" + std::to_string(++current_label_index);
}