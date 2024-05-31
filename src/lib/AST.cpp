#include "AST.h"
#include "Expr.h"
#include <iostream>

// ProgramNode Implementation
void ProgramNode::dump() const {
    std::cout << "ProgramNode:" << std::endl; 
    for (const auto& statement : statements) {
        statement->dump();
    }
}

// VersionDeclNode Implementation
void VersionDeclNode::dump() const {
    std::cout << "OPENQASM " << version << ";" << std::endl;
}

// RegDeclNode Implementation
void RegDeclNode::dump() const {
    std::cout << regType << " " << regName << "[" << size << "];" << std::endl;
}

// IncludeDeclNode Implementation
void IncludeDeclNode::dump() const {
    std::cout << "include \"" << filename << "\";" << std::endl;
}

// GateDeclNode Implementation
void GateDeclNode::dump() const {
    std::cout << "gate " << gateName << " ";
    for (const auto& qubit : qubits) {
        // std::cout << qubit << " ";
        printQubit(*qubit);
    }
    std::cout << "{" << std::endl;
    
    for (const auto& stmt : body) {
        stmt->dump();
    }

    std::cout << "}" << std::endl;
}

// GateStmtNode Implementation
GateStmtNode::GateStmtNode(const Identifier& gateName, const std::vector<std::shared_ptr<ExprNode>>& params, const std::vector<std::shared_ptr<Bit>>& qubits)
    : gateName(gateName), params(params), qubits(qubits) {}


void GateStmtNode::dump() const {
    std::cout << gateName << " ";
    for (const auto& qubit : qubits) {
        std::cout << qubit->name  << "[" << qubit->index << "] ";
    }
    std::cout << ";" << std::endl;
}

// UStmtNode Implementation
UStmtNode::UStmtNode(const Bit& qubit, std::shared_ptr<ExprNode> theta, std::shared_ptr<ExprNode> phi, std::shared_ptr<ExprNode> lambda)
    : qubit(qubit), theta(theta), phi(phi), lambda(lambda) {}

void UStmtNode::dump() const {
    std::cout << "U(";
    theta->dump();
    std::cout << ", ";
    phi->dump();
    std::cout << ", ";
    lambda->dump();
    std::cout << ") ";
    printQubit(qubit);
    std::cout << ";" << std::endl;
    
}

// CXStmtNode Implementation
CXStmtNode::CXStmtNode(const Bit& controlQubit, const Bit& targetQubit)
    : controlQubit(controlQubit), targetQubit(targetQubit) {}

void CXStmtNode::dump() const {
    std::cout << "CX ";
    printQubit(controlQubit);
    std::cout << ", ";
    printQubit(targetQubit);
    std::cout << ";" << std::endl;
}

// MeasureStmtNode Implementation
MeasureStmtNode::MeasureStmtNode(const Bit& qubit, const Bit& classicalRegister)
    : qubit(qubit), classicalRegister(classicalRegister) {}

void MeasureStmtNode::dump() const {
    std::cout << "measure ";
    printQubit(qubit);
    std::cout << " -> ";
    printQubit(classicalRegister);
    std::cout << ";" << std::endl;
}

// ResetStmtNode Implementation
ResetStmtNode::ResetStmtNode(const Bit& qubit) : qubit(qubit) {}

void ResetStmtNode::dump() const {
    std::cout << "reset ";
    printQubit(qubit);
    std::cout << ";" << std::endl;
}

// IfStmtNode Implementation
IfStmtNode::IfStmtNode(const Bit& classicalRegister, std::shared_ptr<QASMNode> statement)
    : classicalRegister(classicalRegister), statement(std::move(statement)) {}

void IfStmtNode::dump() const {
    std::cout << "if (" << classicalRegister.name << "[" << classicalRegister.index << "]) {" << std::endl;
    statement->dump();
    std::cout << "}" << std::endl;
}

// BarrierStmtNode Implementation
BarrierStmtNode::BarrierStmtNode(const std::vector<Bit>& qubits) : qubits(qubits) {}

void BarrierStmtNode::dump() const {
    std::cout << "barrier ";
    for (const auto& qubit : qubits) {
        std::cout << qubit.name << "[" << qubit.index << "] ";
    }
    std::cout << ";" << std::endl;
}
// void printQubit(const Bit& qubit) const;
void QASMNode::printQubit(const Bit& qubit) const {
    if (qubit.index == -1) {
        std::cout << qubit.name;
    } else {
        std::cout << qubit.name << "[" << qubit.index << "]";
    }
}