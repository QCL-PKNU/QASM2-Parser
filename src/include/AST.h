#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "SymbolTable.h"

namespace qasmcpp
{

    // Define string as Identifier
    typedef std::string Identifier;

    class ExprNode;

    // Base class for all QASM nodes
    class QASMNode
    {
    public:
        virtual ~QASMNode() = default; // Virtual destructor

        // Enumeration for node types
        enum NodeType
        {
            OPENQASM = 1,
            INCLUDE,
            QREG,
            CREG,
            MEASURE,
            GATE,
            IF,
            RESET,
            OPAQUE,
            U,
            CX,
            BARRIER,
            LBRACE,
            RBRACE,
            LBRACKET,
            RBRACKET,
            LPAREN,
            RPAREN,
            SEMICOLON,
            COMMA,
            EQ,
            ARROW,
            PLUS,
            MINUS,
            TIMES,
            DIVIDE,
            POWER,
            PI,
            SIN,
            COS,
            TAN,
            EXP,
            LN,
            SQRT,
            STAR,
            ID,
            NNINTEGER,
            REAL,
            STRING,
            FOO,
            COMMENT,
            LINE_COMMENT,
            WS
        };

        int type;

        // Pure virtual function to print the node
        virtual void dump() const = 0;
        void printQubit(const Bit &qubit) const;
    };

    // Derived class for program node
    class ProgramNode : public QASMNode
    {
    public:
        std::vector<std::shared_ptr<QASMNode>> statements;
        std::string version;
        void dump() const override;
    };

    // Derived class for version declaration node
    class VersionDeclNode : public QASMNode
    {
    public:
        std::string version;
        void dump() const override;
    };

    // Derived class for include declaration node
    class IncludeDeclNode : public QASMNode
    {
    public:
        std::string filename;
        void dump() const override;
    };

    // Derived class for register declaration node
    class RegDeclNode : public QASMNode
    {
    public:
        Identifier regName;
        int size;
        enum RegType
        {
            QREG,
            CREG
        } regType;
        void dump() const override;
    };

    // Derived class for gate declaration node
    class GateDeclNode : public QASMNode
    {
    public:
        Identifier gateName;
        std::vector<std::shared_ptr<Bit>> qubits;
        std::vector<std::shared_ptr<QASMNode>> body;
        void dump() const override;
    };

    // Derived class for gate statement node
    class GateStmtNode : public QASMNode
    {
    public:
        int line;
        Identifier gateName;
        std::vector<std::shared_ptr<ExprNode>> params;
        std::vector<std::shared_ptr<Bit>> qubits;

        GateStmtNode(const Identifier &gateName, const std::vector<std::shared_ptr<ExprNode>> &params, const std::vector<std::shared_ptr<Bit>> &qubits);
        void dump() const override;
    };

    // Derived class for U statement node
    class UStmtNode : public QASMNode
    {
    public:
        Bit qubit;
        std::shared_ptr<ExprNode> theta;
        std::shared_ptr<ExprNode> phi;
        std::shared_ptr<ExprNode> lambda;

        UStmtNode(const Bit &qubit, std::shared_ptr<ExprNode> theta, std::shared_ptr<ExprNode> phi, std::shared_ptr<ExprNode> lambda);
        void dump() const override;
    };

    // Derived class for CX statement node
    class CXStmtNode : public QASMNode
    {
    public:
        Bit controlQubit;
        Bit targetQubit;

        CXStmtNode(const Bit &controlQubit, const Bit &targetQubit);
        void dump() const override;
    };

    // Derived class for measure statement node
    class MeasureStmtNode : public QASMNode
    {
    public:
        Bit qubit;
        Bit classicalRegister;

        MeasureStmtNode(const Bit &qubit, const Bit &classicalRegister);
        void dump() const override;
    };

    // Derived class for reset statement node
    class ResetStmtNode : public QASMNode
    {
    public:
        Bit qubit;

        ResetStmtNode(const Bit &qubit);
        void dump() const override;
    };

    // Derived class for if statement node
    class IfStmtNode : public QASMNode
    {
    public:
        Bit classicalRegister;
        std::shared_ptr<QASMNode> statement;

        IfStmtNode(const Bit &classicalRegister, std::shared_ptr<QASMNode> statement);
        void dump() const override;
    };

    // Derived class for barrier statement node
    class BarrierStmtNode : public QASMNode
    {
    public:
        std::vector<Bit> qubits;

        BarrierStmtNode(const std::vector<Bit> &qubits);
        void dump() const override;
    };

} // namespace qasmcpp
#endif // AST_H
