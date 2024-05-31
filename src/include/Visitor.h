#ifndef QASM2Visitor_H
#define QASM2Visitor_H

#include <iostream>
#include <antlr4-runtime.h>
#include "QASM2ParserBaseVisitor.h"
#include "AST.h"

/* base visitor postinclude section */

using namespace antlrcpp;

namespace qasmcpp
{
    /**
     * This class provides an empty implementation of QASM2ParserVisitor, which can be
     * extended to create a visitor which only needs to handle a subset of the available methods.
     */
    class QASM2Visitor : public QASM2ParserBaseVisitor
    {

    private:
        /* base visitor private declarations/members section */
        SymbolTable symbolTable;

        // list of QASMNode
        std::shared_ptr<ProgramNode> program;

    public:
        /* base visitor public declarations/members section */

        Any visitMain(QASM2Parser::MainContext *ctx) override;
        Any visitVersion(QASM2Parser::VersionContext *ctx) override;
        Any visitIncludeDeclStmt(QASM2Parser::IncludeDeclStmtContext *ctx) override;
        Any visitRegDeclStmt(QASM2Parser::RegDeclStmtContext *ctx) override;
        Any visitGateDeclStmt(QASM2Parser::GateDeclStmtContext *ctx) override;
        Any visitOpaqueDeclStmt(QASM2Parser::OpaqueDeclStmtContext *ctx) override;
        Any visitQopStmt(QASM2Parser::QopStmtContext *ctx) override;
        Any visitIfDeclStmt(QASM2Parser::IfDeclStmtContext *ctx) override;
        Any visitBarrierDeclStmt(QASM2Parser::BarrierDeclStmtContext *ctx) override;
        Any visitUop(QASM2Parser::UopContext *ctx) override;
        Any visitArgument(QASM2Parser::ArgumentContext *ctx) override;
        Any visitExpList(QASM2Parser::ExpListContext *ctx) override;
        Any visitExp(QASM2Parser::ExpContext *ctx) override;
        Any visitIdList(QASM2Parser::IdListContext *ctx) override;
        Any visitMixedList(QASM2Parser::MixedListContext *ctx) override;
        Any visitOp(QASM2Parser::OpContext *ctx) override;

        // inline get methods
        inline const std::shared_ptr<ProgramNode> getProgram() { return program; }
        inline const SymbolTable getSymbolTable() { return symbolTable; }
    };

} // namespace qasmcpp

#endif /* QASM2Visitor_H */