// test/LexerTests.cpp
#include <gtest/gtest.h>
#include <antlr4-runtime.h>
#include "QASM2Lexer.h"

using namespace antlr4;
using namespace qasmcpp;

// Test the basic head of the QASM file
TEST(LexerTest, BasicTest) {
    std::string qasm_code = "OPENQASM 2.0;";
    ANTLRInputStream input(qasm_code);
    QASM2Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
    std::vector<Token*> allTokens = tokens.getTokens();

    ASSERT_EQ(allTokens.size(), 4); // Adjust the number based on expected tokens, include <EOF>
    ASSERT_EQ(allTokens[0]->getText(), "OPENQASM");
    ASSERT_EQ(allTokens[1]->getText(), "2.0");
    ASSERT_EQ(allTokens[2]->getText(), ";");
}

// Test the basic include statement
TEST(LexerTest, IncludeTest) {
    std::string qasm_code = "OPENQASM 2.0;\ninclude \"filename\";\n";
    ANTLRInputStream input(qasm_code);
    QASM2Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
    std::vector<Token*> allTokens = tokens.getTokens();

    ASSERT_EQ(allTokens.size(), 7); // Adjust the number based on expected tokens, include <EOF>
    ASSERT_EQ(allTokens[0]->getText(), "OPENQASM");
    ASSERT_EQ(allTokens[1]->getText(), "2.0");
    ASSERT_EQ(allTokens[2]->getText(), ";");
    ASSERT_EQ(allTokens[3]->getText(), "include");
    ASSERT_EQ(allTokens[4]->getText(), "\"filename\"");
    ASSERT_EQ(allTokens[5]->getText(), ";");
}


// Test the basic qreg and cqreg declaration
TEST(LexerTest, RegDeclTest) {
    std::string qasm_code = "qreg q[2];\ncreg c[2];\n";
    ANTLRInputStream input(qasm_code);
    QASM2Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
    std::vector<Token*> allTokens = tokens.getTokens();

    ASSERT_EQ(allTokens.size(), 13); // Adjust the number based on expected tokens, include <EOF>
    ASSERT_EQ(allTokens[0]->getText(), "qreg");
    ASSERT_EQ(allTokens[1]->getText(), "q");
    ASSERT_EQ(allTokens[2]->getText(), "[");
    ASSERT_EQ(allTokens[3]->getText(), "2");
    ASSERT_EQ(allTokens[4]->getText(), "]");
    ASSERT_EQ(allTokens[5]->getText(), ";");
    ASSERT_EQ(allTokens[6]->getText(), "creg");
    ASSERT_EQ(allTokens[7]->getText(), "c");
    ASSERT_EQ(allTokens[8]->getText(), "[");
    ASSERT_EQ(allTokens[9]->getText(), "2");
    ASSERT_EQ(allTokens[10]->getText(), "]");
    ASSERT_EQ(allTokens[11]->getText(), ";");
    
}

// Test the basic gate declaration
TEST(LexerTest, GateDeclTest) {
    std::string qasm_code = "gate h a { h a; }\ngate rx(theta) a { u3(theta, -pi/2,pi/2) a; }\n";
    ANTLRInputStream input(qasm_code);
    QASM2Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
    std::vector<Token*> allTokens = tokens.getTokens();

    ASSERT_EQ(allTokens.size(), 32); // Adjust the number based on expected tokens, include <EOF>
    ASSERT_EQ(allTokens[0]->getText(), "gate");
    ASSERT_EQ(allTokens[1]->getText(), "h");
    ASSERT_EQ(allTokens[2]->getText(), "a");
    ASSERT_EQ(allTokens[3]->getText(), "{");
    ASSERT_EQ(allTokens[4]->getText(), "h");
    ASSERT_EQ(allTokens[5]->getText(), "a");
    ASSERT_EQ(allTokens[6]->getText(), ";");
    ASSERT_EQ(allTokens[7]->getText(), "}");
    ASSERT_EQ(allTokens[8]->getText(), "gate");
    ASSERT_EQ(allTokens[9]->getText(), "rx");
    ASSERT_EQ(allTokens[10]->getText(), "(");
    ASSERT_EQ(allTokens[11]->getText(), "theta");
    ASSERT_EQ(allTokens[12]->getText(), ")");
    ASSERT_EQ(allTokens[13]->getText(), "a");
    ASSERT_EQ(allTokens[14]->getText(), "{");
    ASSERT_EQ(allTokens[15]->getText(), "u3");
    ASSERT_EQ(allTokens[16]->getText(), "(");
    ASSERT_EQ(allTokens[17]->getText(), "theta");
    ASSERT_EQ(allTokens[18]->getText(), ",");
    ASSERT_EQ(allTokens[19]->getText(), "-");
    ASSERT_EQ(allTokens[20]->getText(), "pi");
    ASSERT_EQ(allTokens[21]->getText(), "/");
    ASSERT_EQ(allTokens[22]->getText(), "2");
    ASSERT_EQ(allTokens[23]->getText(), ",");
    ASSERT_EQ(allTokens[24]->getText(), "pi");
    ASSERT_EQ(allTokens[25]->getText(), "/");
    ASSERT_EQ(allTokens[26]->getText(), "2");
    ASSERT_EQ(allTokens[27]->getText(), ")");
    ASSERT_EQ(allTokens[28]->getText(), "a");
}

// Test the basic gate statement
TEST(LexerTest, GateStmtTest) {
    std::string qasm_code = "OPENQASM 2.0;\nh q[0];\n";
    ANTLRInputStream input(qasm_code);
    QASM2Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
    std::vector<Token*> allTokens = tokens.getTokens();

    ASSERT_EQ(allTokens.size(), 10); // Adjust the number based on expected tokens, include <EOF>
    ASSERT_EQ(allTokens[0]->getText(), "OPENQASM");
    ASSERT_EQ(allTokens[1]->getText(), "2.0");
    ASSERT_EQ(allTokens[2]->getText(), ";");
    ASSERT_EQ(allTokens[3]->getText(), "h");
    ASSERT_EQ(allTokens[4]->getText(), "q");
    ASSERT_EQ(allTokens[5]->getText(), "[");
}

// Test the basic sin cos tan exp ln sqrt operation
// e.g:  U(sin(pi), cos(pi), pi) b;
TEST(LexerTest, MathOpTest) {
    std::string qasm_code = "U(sin(pi), cos(pi), tan(0.2)) b;\n";
    ANTLRInputStream input(qasm_code);
    QASM2Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
    std::vector<Token*> allTokens = tokens.getTokens();

    ASSERT_EQ(allTokens.size(), 20); // Adjust the number based on expected tokens, include <EOF>
    ASSERT_EQ(allTokens[0]->getText(), "U");
    ASSERT_EQ(allTokens[1]->getText(), "(");
    ASSERT_EQ(allTokens[2]->getText(), "sin");
    ASSERT_EQ(allTokens[3]->getText(), "(");
    ASSERT_EQ(allTokens[4]->getText(), "pi");
    ASSERT_EQ(allTokens[5]->getText(), ")");
    ASSERT_EQ(allTokens[6]->getText(), ",");
    ASSERT_EQ(allTokens[7]->getText(), "cos");
    ASSERT_EQ(allTokens[8]->getText(), "(");
    ASSERT_EQ(allTokens[9]->getText(), "pi");
    ASSERT_EQ(allTokens[10]->getText(), ")");
    ASSERT_EQ(allTokens[11]->getText(), ",");
    ASSERT_EQ(allTokens[12]->getText(), "tan");
    ASSERT_EQ(allTokens[13]->getText(), "(");
    ASSERT_EQ(allTokens[14]->getText(), "0.2");
    ASSERT_EQ(allTokens[15]->getText(), ")");
  
}

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
