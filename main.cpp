#include <iostream>
#include <fstream>
#include <antlr4-runtime.h>
#include "QASM2Parser.h"
#include "QASM2Lexer.h"
#include "Visitor.h"
#include "AST.h"

#include "qplayer.h"

using namespace std;
using namespace qasmcpp;
using namespace antlr4;

int main(int argc, const char* argv[]) {


    // This is a test if QPlayer works
    QRegister QReg = new QRegister(12);
    cout << "QReg: " << QReg.getNumQubits() << endl;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path-to-qasm>" << std::endl;
        return 1;
    }

    const char* filePath = argv[1];
    
    // Open the input file
    std::ifstream stream;
    stream.open(filePath);
    
    if (!stream.is_open()) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return 1;
    }

    ANTLRInputStream input(stream);
    qasmcpp::QASM2Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();

    qasmcpp::QASM2Parser parser(&tokens);
    tree::ParseTree *tree = parser.main();

    QASM2Visitor visitor;
    visitor.visit(tree);

    auto program = visitor.getProgram();


    auto gateDefines = visitor.getSymbolTable().gateDefines;
    auto regDefines = visitor.getSymbolTable().qubitRegisters;
    auto cregDefines = visitor.getSymbolTable().cbitRegisters;

    for (const auto& gate : gateDefines) {
        std::cout << "GATE: " << gate.first << std::endl;
    }

    // for(const auto& statement : program->statements) {
    //     statement->dump();
    // }


    // std::cout << tree->toStringTree(&parser) << std::endl;
    std::cout << "FINISH PARSING\n";    
    return 0;
}

