
#include <stdexcept>
#include "Register.h"

using namespace qasmcpp;

// Implementation of Bit class
Bit::Bit(const std::string& name, int index, BitType type) : name(name), index(index), type(type) {}

Bit::Bit(const std::string& name, int index) : name(name), index(index), type(BitType::Unknown) {}

// Implementation of Register class
Register::Register(const std::string& name, int size, BitType type) : name(name) {
    for (int i = 0; i < size; ++i) {
        bits.emplace_back(name, i, type); // Create bits with the specified type
    }
}

qasmcpp::Bit Register::getBit(int index) {
    if (index >= 0 && index < bits.size()) {
        return bits[index];
    } else {
        throw std::out_of_range("Bit index out of range");
    }
}