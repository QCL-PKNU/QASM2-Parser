#ifndef EXPRESSION_NODE_H
#define EXPRESSION_NODE_H

#include <string>
#include <iostream>
#include <memory>
#include "AST.h"

namespace qasmcpp
{

    class QASMNode;

    // Base class for all QASM nodes
    class ExprNode : public QASMNode
    {
    public:
        enum
        {
            EXPR = 0,
            NNINTEGER = 1,
            REAL = 2,
            ID = 3,
            UNARY = 4,
            BINARY = 5,
            PI = 6,
            NAG = 7,
        };

        enum ArithOpType
        {
            PLUS = 0,
            MINUS = 1,
            TIMES = 2,
            DIVIDE = 3,
            POWER = 4,
        };

        enum UnaryOpType
        {
            SIN = 0,
            COS = 1,
            TAN = 2,
            EXP = 3,
            LN = 4,
            SQRT = 5,
            NAGATIVE = 6,
        };

        // Virtual destructor
        virtual ~ExprNode() = default;

        virtual int getExpType() const { return EXPR; };

        void dump() const { /* Need to implement */ };
    };

    // Numeric literal expressions
    class NNIntegerLiteralNode : public ExprNode
    {
    public:
        int value;

        NNIntegerLiteralNode(int value) : value(value) {}
        int getExpType() const override { return NNINTEGER; }
        void dump() const override
        {
            std::cout << "NNIntegerLiteralNode(" << value << ")" << std::endl;
        }
    };

    class RealLiteralNode : public ExprNode
    {
    public:
        double value;

        RealLiteralNode(double value) : value(value) {}
        int getExpType() const override { return REAL; }
        void dump() const override
        {
            std::cout << "RealLiteralNode(" << value << ")" << std::endl;
        }
    };

    // Identifier expression
    class IdentifierNode : public ExprNode
    {
    public:
        std::string name;

        IdentifierNode(const std::string &name) : name(name) {}
        int getExpType() const override { return ID; }
    };

    // Unary expression (e.g., negation)
    class UnaryExprNode : public ExprNode
    {
    public:
        int op; // Operator symbol (-, sin, cos, etc.) // check UnaryOpType
        std::shared_ptr<ExprNode> operand;

        UnaryExprNode(const int &op, std::shared_ptr<ExprNode> operand)
            : op(op), operand(std::move(operand)) {}
        int getExpType() const override { return UNARY; }
    };

    // Binary expression (e.g., addition, multiplication)
    class BinaryExprNode : public ExprNode
    {
    public:
        int op; // Operator symbol (+, -, *, etc.) // check ArithOpType
        std::shared_ptr<ExprNode> left;
        std::shared_ptr<ExprNode> right;

        BinaryExprNode(const int &op, std::shared_ptr<ExprNode> left, std::shared_ptr<ExprNode> right)
            : op(op), left(std::move(left)), right(std::move(right)) {}

        int getExpType() const override { return BINARY; }
        int getOp() const { return op; }
    };

} // namespace qasmcpp
#endif // EXPRESSION_NODE_H
