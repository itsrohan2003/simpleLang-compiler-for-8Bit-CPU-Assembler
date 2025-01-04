## simpleLang-compiler-for-8Bit-CPU-Assembler

This is the documentation of the development and the implementation of my compiler(simpleLang) which converts the code written in a high level language called simpleLang into an assembly code which is compatible with the assembler program created for the 8bit CPU by the repository creator of https://github.com/lightcode/8bit-computer

## The process of the development includes
1. creating a lexer and header
2. creating a parser and header
3. creating a codegenerator and header
4. creating a main.cpp file which can direct the working of parser, lexer and codegenerator

## understanding the problem
The goal of the compiler is to create a compiler that can convert my simpleLang code into an assembly code which is compatible with the asm/asm.py file so that it can convert the code into an assembly code which is according to the format of the cpu(such as the "tests/multiplication_test.asm")

## understanding lexer

lexer analysis is the first stage of the the compilation. It's job is to identify the tokens from the high level code written by us. It then forms a sequence of valid tokens. The process of identification of a part of code as whether it is token or not is the job of lexer.

Example: If you have the simpleLang code int a = 10;, the lexer would produce tokens like:
INT (keyword)
IDENTIFIER (for 'a')
ASSIGN (for '=')
NUMBER (for '10')
SEMICOLON

## understanding parser

The parser converts the sequence of tokens generated by the lexer, and forms an AST-Abstract Syntax Tree. the AST represents the logical relation between different tokens and the grammatical structure as well.

Example: For the code int a = 10;, the parser might create an AST node of type VAR_DECL with two children:
An IDENTIFIER node for 'a'
A NUMBER node for '10'

## understanding codegnerator

The code generator takes the AST generated by the parser and translates it into assembly code for your 8-bit CPU. It maps the high-level constructs of simpleLang (like variable declarations, assignments, and if statements) into equivalent low-level assembly instructions.

## the main.cpp file

this file defines how to direct the lexer, parser and the code generator to operate  correctly. This is the driver code of the compiler.

In my compiler, I have defined the compiler to  ->
Reads the simpleLang source code.
Call the lexer to generate tokens.
Call the parser to create the AST.
Call the code generator to produce assembly code.
Write the assembly code to an output file.

## how to run the compiler(using bash)

I have created this compiler on ubuntu 20.04 because the CPU I cloned from the repository was configured with linux commands for particular permissions which were ivnalid in the windows CLI. 

Since I had windows and ubuntu both, I preffered to use ubuntu.

1. clone the repository

git clone https://github.com/itsrohan2003/simpleLang-compiler-for-8Bit-CPU-Assembler.git

2. In the repositoy, there is a folder "simpleLang". This is the compiler folder. It has all the files required to compile.

3. The asm/asm.py file is the assembler code file. It is responsible for converting the assembly code into machine code for the CPU. Our job is to convert the HLL into an Assembly code which is expected by the asm.py file ( to put it simply )

4. Open linux terminal/bash. and type
g++ lexer.cpp parser.cpp codegenerator.cpp main.cpp -o simple_compiler
This is a code to link the lexer, parser, code generator and the main.cpp file together to form an executable file called simple_compiler

5. After successfull compilation, a simple_lang executable file is created in the simpleLang folder itself

6. Type ./simple_compiler

7. the terminal will prompt you to type you simpleLang code

8. write the code (for example int a = 1092;)

9. sample input and output 

Enter code in simpleLang:
int a =1092;

Assembly code saved to: output/test2.asm

here the assmebly code is generated and stored in the output/test2.asm. For testing purpose, I simply copy-pasted the code from output/test2.asm to tests/multiplication_test.asm 

10. In order to test whether the assembly code is perfect or not,
in the terminal in vscode, type 
./asm/asm.py tests/multiplication_test.asm > memory.list
This command tells the terminal to use asm.py file to compile multiplication_test.asm file and store the machine code generated into memory.list

11. If the simpleLang compiler works correctly, it will generate the assembly code which is executed by the asm.py file correctly and store the machine code in memory.list which can be checked after execution of the command.

## testing
[![simpleLang Compiler Demo](http://img.youtube.com/vi/1v-zGGsbsymZt6tIIlY8C7OSMpHQjBXoT/0.jpg)](http://www.youtube.com/watch?v=1v-zGGsbsymZt6tIIlY8C7OSMpHQjBXoT)

This compiler translates code written in the simpleLang programming language into assembly code compatible with an 8-bit CPU. It's designed to work with the assembler found in the [lightcode/8bit-computer](https://github.com/lightcode/8bit-computer) repository.

