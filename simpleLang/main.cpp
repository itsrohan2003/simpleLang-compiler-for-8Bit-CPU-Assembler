#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"
#include "codegenerator.h"

using namespace std;

int main() {
    string code;
    cout << "Enter code in simpleLang:\n";
    getline(cin, code);

    try {
        // 1. Lexical Analysis
        vector<Token> tokens = lex(code);

        // 2. Parsing
        Parser parser(tokens);
        ASTNode ast = parser.parse();

        // 3. Code Generation
        CodeGenerator generator;
        string assemblyCode = generator.generateCode(ast);

        // 4. Output to File
        int fileNumber = 1;
        string filename;
        stringstream ss;

        do {
            ss.str(""); 
            ss << "output/test" << fileNumber << ".asm";
            filename = ss.str();
            fileNumber++;
        } while (ifstream(filename)); 

        ofstream outputFile(filename);
        if (outputFile.is_open()) {
            outputFile << assemblyCode;
            outputFile.close();
            cout << "\nAssembly code saved to: " << filename << endl;
        } else {
            cerr << "Error: Unable to create output file." << endl;
        }

    } catch (const runtime_error& error) {
        cerr << "Error: " << error.what() << endl;
    }

    return 0;
}