// test/ParserTests.cpp

#include <gtest/gtest.h>
#include <antlr4-runtime.h>
#include "QASM2Lexer.h"
#include "QASM2Parser.h"
#include "Visitor.h"
#include "AST.h"

using namespace antlr4;
using namespace qasmcpp;

class ParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    std::shared_ptr<ProgramNode> parse(const std::string& qasm_code) {
        ANTLRInputStream input(qasm_code);
        QASM2Lexer lexer(&input);
        CommonTokenStream tokens(&lexer);

        QASM2Parser parser(&tokens);
        tree::ParseTree *tree = parser.main();

        QASM2Visitor visitor;
        visitor.visit(tree);
        return visitor.getProgram();
    }
};

TEST_F(ParserTest, ParseVersion) {
    std::string qasm_code = "OPENQASM 2.0;";
    auto program = parse(qasm_code);

    ASSERT_EQ(program->version, "2.0");
}

TEST_F(ParserTest, ParseQubitRegister) {
    std::string qasm_code = "OPENQASM 2.0;\nqreg q[2];";
    auto program = parse(qasm_code);

    auto regDecl = std::dynamic_pointer_cast<RegDeclNode>(program->statements[0]);
    ASSERT_NE(regDecl, nullptr);
    ASSERT_EQ(regDecl->regType, RegDeclNode::QREG);
    ASSERT_EQ(regDecl->regName, "q");
    ASSERT_EQ(regDecl->size, 2);
}

TEST_F(ParserTest, ParseClassicalRegister) {
    std::string qasm_code = "OPENQASM 2.0;\ncreg c[3];";
    auto program = parse(qasm_code);

    auto regDecl = std::dynamic_pointer_cast<RegDeclNode>(program->statements[0]);
    ASSERT_NE(regDecl, nullptr);
    ASSERT_EQ(regDecl->regType, RegDeclNode::CREG);
    ASSERT_EQ(regDecl->regName, "c");
    ASSERT_EQ(regDecl->size, 3);
}

TEST_F(ParserTest, ParseGateDeclaration) {
    std::string qasm_code = "OPENQASM 2.0;\ngate mygate a, b { U(1, 2, 3) a; CX a, b; }";
    auto program = parse(qasm_code);

    auto gateDecl = std::dynamic_pointer_cast<GateDeclNode>(program->statements[0]);
    ASSERT_NE(gateDecl, nullptr);
    ASSERT_EQ(gateDecl->gateName, "mygate");
    ASSERT_EQ(gateDecl->qubits.size(), 2);
    ASSERT_EQ(gateDecl->qubits[0]->name, "a");
    ASSERT_EQ(gateDecl->qubits[1]->name, "b");
    ASSERT_EQ(gateDecl->body.size(), 2);
    // cast to UStmtNode and CXStmtNode and check their properties

    auto uStmt = std::dynamic_pointer_cast<UStmtNode>(gateDecl->body[0]);
    ASSERT_NE(uStmt, nullptr);
    ASSERT_EQ(uStmt->qubit.name, "a");

    auto cxStmt = std::dynamic_pointer_cast<CXStmtNode>(gateDecl->body[1]);
    ASSERT_NE(cxStmt, nullptr);
    ASSERT_EQ(cxStmt->controlQubit.name, "a");
    ASSERT_EQ(cxStmt->targetQubit.name, "b");

}

TEST_F(ParserTest, ParseGateStatement) {
    std::string qasm_code = "OPENQASM 2.0;\nqreg q[1];\ngate mygate a {U(1,2,3) a;} \nmygate q[0];";
    auto program = parse(qasm_code);

    auto gateStmt = std::dynamic_pointer_cast<GateStmtNode>(program->statements[2]);
    ASSERT_NE(gateStmt, nullptr);
    ASSERT_EQ(gateStmt->gateName, "mygate");
    ASSERT_EQ(gateStmt->qubits.size(), 1);
    ASSERT_EQ(gateStmt->qubits[0]->name, "q");
    ASSERT_EQ(gateStmt->qubits[0]->index, 0);
}


TEST_F(ParserTest, ParseMeasure) {
    std::string qasm_code = "OPENQASM 2.0;\nqreg q[1];\ncreg c[1];\nmeasure q[0] -> c[0];";
    auto program = parse(qasm_code);

    auto measureStmt = std::dynamic_pointer_cast<MeasureStmtNode>(program->statements[2]);
    ASSERT_NE(measureStmt, nullptr);
    ASSERT_EQ(measureStmt->qubit.name, "q");
    ASSERT_EQ(measureStmt->qubit.index, 0);
    ASSERT_EQ(measureStmt->classicalRegister.name, "c");
    ASSERT_EQ(measureStmt->classicalRegister.index, 0);
}

TEST_F(ParserTest, ParseUOperation) {
    std::string qasm_code = "OPENQASM 2.0;\nqreg q[1];\nU(1, 2, 3) q[0];";
    auto program = parse(qasm_code);

    auto uStmt = std::dynamic_pointer_cast<UStmtNode>(program->statements[1]);
    ASSERT_NE(uStmt, nullptr);
    ASSERT_EQ(uStmt->qubit.name, "q");
    ASSERT_EQ(uStmt->qubit.index, 0);
}

TEST_F(ParserTest, ParseCXOperation) {
    std::string qasm_code = "OPENQASM 2.0;\nqreg q[2];\nCX q[0], q[1];";
    auto program = parse(qasm_code);

    auto cxStmt = std::dynamic_pointer_cast<CXStmtNode>(program->statements[1]);
    ASSERT_NE(cxStmt, nullptr);
    ASSERT_EQ(cxStmt->controlQubit.name, "q");
    ASSERT_EQ(cxStmt->controlQubit.index, 0);
    ASSERT_EQ(cxStmt->targetQubit.name, "q");
    ASSERT_EQ(cxStmt->targetQubit.index, 1);
}
