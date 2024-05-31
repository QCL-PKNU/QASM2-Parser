
#include <fstream>
#include <antlr4-runtime.h>
#include "QASM2Lexer.h"
#include "QASM2Parser.h"
#include "Visitor.h"
#include "Expr.h"

using namespace antlr4;
using namespace qasmcpp;

Any QASM2Visitor::visitMain(QASM2Parser::MainContext *ctx)
{

    program = std::make_shared<ProgramNode>();

    visitVersion(ctx->version());

    for (auto statement : ctx->statement())
    {
        // std::cout << "STATEMENT: " << statement->getText() << " " << std::endl;
        auto stmt = visitStatement(statement);
        auto gateStmt = stmt.as<std::shared_ptr<QASMNode>>();
        program->statements.push_back(stmt);
    }

    return program;
}

Any QASM2Visitor::visitVersion(QASM2Parser::VersionContext *ctx)
{
    program->version = ctx->ver;
    return visitChildren(ctx);
};

Any QASM2Visitor::visitIncludeDeclStmt(QASM2Parser::IncludeDeclStmtContext *ctx)
{
    auto includeNode = std::make_shared<IncludeDeclNode>();
    includeNode->filename = ctx->filename;
    auto node = std::dynamic_pointer_cast<QASMNode>(includeNode);

    std::string name = ctx->filename.substr(1, ctx->filename.size() - 2);

    std::ifstream stream;

    stream.open(name);
    if (!stream.is_open()) {
        std::cerr << "Could not open file: " << name<< std::endl;
    }

    ANTLRInputStream input(stream);
    QASM2Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    QASM2Parser parser(&tokens);
    QASM2Parser::MainContext *tree = parser.main();
    visit(tree);

    return node;
}

Any QASM2Visitor::visitRegDeclStmt(QASM2Parser::RegDeclStmtContext *ctx)
{

    RegDeclNode::RegType regType = RegDeclNode::RegType::QREG;

    if (ctx->QREG() != nullptr)
    {
        symbolTable.addQubitRegister(ctx->ID()->getText(), std::stoi(ctx->NNINTEGER()->getText()));
    }
    else if (ctx->CREG() != nullptr)
    {
        regType = RegDeclNode::RegType::CREG;
        symbolTable.addCbitRegister(ctx->ID()->getText(), std::stoi(ctx->NNINTEGER()->getText()));
    }

    auto regDecl = std::make_shared<RegDeclNode>();
    regDecl->regName = ctx->ID()->getText();
    regDecl->size = std::stoi(ctx->NNINTEGER()->getText());
    regDecl->regType = regType;

    auto node = std::dynamic_pointer_cast<QASMNode>(regDecl);
    return node;
}

Any QASM2Visitor::visitGateDeclStmt(QASM2Parser::GateDeclStmtContext *ctx)
{
    auto gateDecl = std::make_shared<GateDeclNode>();
    auto gateDef = std::make_shared<Gate>();

    // check size of idList()
    unsigned int size = ctx->idList().size();

    // set gate name
    gateDecl->gateName = ctx->ID()->getText();

    // handle params and qubits
    std::vector<std::string> params;
    std::vector<std::string> qubits;

    if (ctx->hasParams)
    {
        params = visitIdList(ctx->idList()[0]).as<std::vector<std::string>>();
        qubits = visitIdList(ctx->idList()[1]).as<std::vector<std::string>>();
    }
    else
    {
        qubits = visitIdList(ctx->idList()[0]).as<std::vector<std::string>>();
    }

    // iterate over uop() and add to body
    for (auto uop : ctx->uop())
    {
        auto uopNode = visitUop(uop).as<std::shared_ptr<QASMNode>>();
        gateDecl->body.push_back(uopNode);
    }

    // add gate to symbol table
    gateDef->name = gateDecl->gateName;
    gateDef->params = params;
    for (auto qubit : qubits)
    {
        gateDef->qubits.push_back(std::make_shared<Bit>(qubit, -1, BitType::Qubit));
    }
    gateDef->body = gateDecl->body;
    symbolTable.addGateDef(gateDef->name, gateDef);

    gateDecl->qubits = gateDef->qubits;

    auto node = std::dynamic_pointer_cast<QASMNode>(gateDecl);

    return node;
}

Any QASM2Visitor::visitOpaqueDeclStmt(QASM2Parser::OpaqueDeclStmtContext *ctx)
{
    throw std::runtime_error("Opaque statement not implemented yet");
    return visitChildren(ctx);
}

Any QASM2Visitor::visitQopStmt(QASM2Parser::QopStmtContext *ctx)
{
    switch (ctx->type)
    {
    case QASM2Parser::MEASURE:
    {
        auto qubit = visitArgument(ctx->argument()[0]).as<std::shared_ptr<Bit>>();
        auto cbit = visitArgument(ctx->argument()[1]).as<std::shared_ptr<Bit>>();

        auto measureStmt = std::make_shared<MeasureStmtNode>(*qubit, *cbit);
        auto node = std::dynamic_pointer_cast<QASMNode>(measureStmt);
        return node;
    }
    case QASM2Parser::RESET:
    {
        auto qubit = visitArgument(ctx->argument()[0]).as<std::shared_ptr<Bit>>();

        auto resetStmt = std::make_shared<ResetStmtNode>(*qubit);
        auto node = std::dynamic_pointer_cast<QASMNode>(resetStmt);
        return node;
    }
    default:
        return visitUop(ctx->uop());
        break;
    }
    return visitChildren(ctx);
}

Any QASM2Visitor::visitIfDeclStmt(QASM2Parser::IfDeclStmtContext *ctx)
{
    throw std::runtime_error("If statement not implemented yet");
    return visitChildren(ctx);
}

Any QASM2Visitor::visitBarrierDeclStmt(QASM2Parser::BarrierDeclStmtContext *ctx)
{
    // rise error if barrier is not implemented
    throw std::runtime_error("Barrier statement not implemented yet");
    return visitChildren(ctx);
}

Any QASM2Visitor::visitUop(QASM2Parser::UopContext *ctx)
{
    switch (ctx->type)
    {
    case QASM2Parser::U:
    {
        /* code */
        auto qubit = visitArgument(ctx->argument()[0]).as<std::shared_ptr<Bit>>();
        auto expList = visitExpList(ctx->expList()).as<std::vector<std::shared_ptr<ExprNode>>>();

        auto theta = expList[0];
        auto phi = expList[1];
        auto lambda = expList[2];

        auto uStmt = std::make_shared<UStmtNode>(*qubit, theta, phi, lambda);
        auto node = std::dynamic_pointer_cast<QASMNode>(uStmt);

        return node;
    }
    case QASM2Parser::CX:
    {
        /* code */
        auto control = visitArgument(ctx->argument()[0]).as<std::shared_ptr<Bit>>();
        auto target = visitArgument(ctx->argument()[1]).as<std::shared_ptr<Bit>>();

        auto cxStmt = std::make_shared<CXStmtNode>(*control, *target);
        auto node = std::dynamic_pointer_cast<QASMNode>(cxStmt);

        return node;
    }
    case QASM2Parser::ID:
    {
        std::string gateName = ctx->gateName;

        // check if gate has params
        std::vector<std::shared_ptr<ExprNode>> expList;
        if (ctx->expList() != nullptr)
        {
            expList = visitExpList(ctx->expList()).as<std::vector<std::shared_ptr<ExprNode>>>();
        }

        // vitsit mixedList
        auto qubits = visitMixedList(ctx->mixedList());

        auto gateStmt = std::make_shared<GateStmtNode>(gateName, expList, qubits);
        auto node = std::dynamic_pointer_cast<QASMNode>(gateStmt);

        return node;
    }
    }
    return visitChildren(ctx);
}

Any QASM2Visitor::visitArgument(QASM2Parser::ArgumentContext *ctx)
{

    std::string id = ctx->ID()->getText();

    int index = -1;

    if (ctx->NNINTEGER() != nullptr)
        index = std::stoi(ctx->NNINTEGER()->getText());

    return std::make_shared<Bit>(id, index, BitType::Unknown);
}

Any QASM2Visitor::visitExpList(QASM2Parser::ExpListContext *ctx)
{
    // list of expressions node
    std::vector<std::shared_ptr<ExprNode>> expList;

    for (auto exp : ctx->exp())
    {
        auto expNode = visitExp(exp).as<std::shared_ptr<ExprNode>>();
        expList.push_back(expNode);
    }

    return expList;
}

Any QASM2Visitor::visitExp(QASM2Parser::ExpContext *ctx)
{

    std::shared_ptr<ExprNode> expNode;

    // check if expression is a number
    switch (ctx->exprType)
    {
    case ExprNode::NNINTEGER:
    {
        expNode = std::make_shared<NNIntegerLiteralNode>(std::stoi(ctx->NNINTEGER()->getText()));
        break;
    }
    case ExprNode::REAL:
    {
        expNode = std::make_shared<RealLiteralNode>(std::stod(ctx->REAL()->getText()));
        break;
    }
    case ExprNode::ID:
    {
        expNode = std::make_shared<IdentifierNode>(ctx->ID()->getText());
        break;
    }
    case ExprNode::PI:
    {
        const double pi = 3.1415926535897932384626433;
        expNode = std::make_shared<RealLiteralNode>(pi);
        break;
    }
    case ExprNode::UNARY:
    {
        // unary expression
        auto unary = ctx->unaryop()->opType;
        auto exp = visitExp(ctx->exp()[0]).as<std::shared_ptr<ExprNode>>();
        expNode = std::make_shared<UnaryExprNode>(unary, exp);
        break;
    }
    case ExprNode::BINARY:
    {
        auto left = visitExp(ctx->exp()[0]).as<std::shared_ptr<ExprNode>>();
        auto right = visitExp(ctx->exp()[1]).as<std::shared_ptr<ExprNode>>();
        auto op = visitOp(ctx->op()).as<ExprNode::ArithOpType>();

        expNode = std::make_shared<BinaryExprNode>(op, left, right);
        break;
    }
    case ExprNode::NAG:
    {
        auto exp = visitExp(ctx->exp()[0]).as<std::shared_ptr<ExprNode>>();
        expNode = std::make_shared<UnaryExprNode>(ExprNode::UnaryOpType::NAGATIVE, exp);
        break;
    }
    case ExprNode::EXPR: 
    {
        expNode = visitExp(ctx->exp()[0]).as<std::shared_ptr<ExprNode>>();
        break;
    }
    default:
    {
        throw std::runtime_error("Expression not implemented yet");
        break;
    }
    }
    auto node = std::dynamic_pointer_cast<ExprNode>(expNode);

    return node;
}

Any QASM2Visitor::visitOp(QASM2Parser::OpContext *ctx)
{
    return ctx->opType;
}

Any QASM2Visitor::visitIdList(QASM2Parser::IdListContext *ctx)
{
    std::vector<std::string> ids;

    for (auto id : ctx->ID())
    {
        ids.push_back(id->getText());
    }
    return ids;
}

Any QASM2Visitor::visitMixedList(QASM2Parser::MixedListContext *ctx)
{
    std::vector<std::shared_ptr<Bit>> ids;

    for (auto id : ctx->argument())
    {
        auto bit = visitArgument(id).as<std::shared_ptr<Bit>>();
        ids.push_back(bit);
    }

    return ids;
}