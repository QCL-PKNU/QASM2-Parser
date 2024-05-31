#include <stdexcept>
#include "SymbolTable.h"
#include "Register.h"


// Implementation of SymbolTable class
void SymbolTable::addQubitRegister(const std::string& name, int size) {
    addRegister(name, size, BitType::Qubit);
}

void SymbolTable::addCbitRegister(const std::string& name, int size) {
    addRegister(name, size, BitType::Cbit);
}

void SymbolTable::addRegister(const std::string& name, int size, BitType type) {
    // check if the register already exists and show the name and size and type of it
    if (isQubitRegister(name) || isCbitRegister(name)) {
        std::string errorMsg = "Register already exists: " + name + " " + std::to_string(size) + " ";
        throw std::runtime_error(errorMsg);
    }
    if (type == BitType::Qubit) {
        qubitRegisters[name] = std::make_shared<Register>(name, size, BitType::Qubit);
    } else {
        cbitRegisters[name] = std::make_shared<Register>(name, size, BitType::Cbit);
    }
}

void SymbolTable::addGateDef(const std::string& name, std::shared_ptr<Gate> gate) {
    if (hasGateDef(name)) {
        std::string errorMsg = "Gate definition already exists: " + name;
        throw std::runtime_error(errorMsg);
    }
    gateDefines[name] = gate;
}


std::shared_ptr<Gate> SymbolTable::getGateDef(const std::string& name) {
    auto it = gateDefines.find(name);
    if (it != gateDefines.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Gate definition not found");
    }
}

std::shared_ptr<Register> SymbolTable::getQubitRegister(const std::string& name) {
    auto it = qubitRegisters.find(name);
    if (it != qubitRegisters.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Qubit register not found");
    }
}
std::shared_ptr<Register> SymbolTable::getCbitRegister(const std::string& name) {
    auto it = cbitRegisters.find(name);
    if (it != cbitRegisters.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Cbit register not found");
    }
}