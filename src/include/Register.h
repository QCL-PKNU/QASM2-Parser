#ifndef QASM_REGISTER_H
#define QASM_REGISTER_H

#include <vector>
#include <string>
#include <memory>

namespace qasmcpp
{

    enum class BitType
    {
        Qubit,
        Cbit,
        Unknown,
    };

    // Class representing a single bit (either a qubit or a classical bit)
    class Bit
    {
    public:
        std::string name;
        int index;
        BitType type;

        Bit(const std::string &name, int index, BitType type);
        Bit(const std::string &name, int index);
    };

    // Class representing a register (array of bits)
    class Register
    {
    public:
        std::string name;
        std::vector<Bit> bits;

        Register(const std::string &name, int size, BitType type);
        Bit getBit(int index);
    };

    // Class representing a gate that user defines
    class QASMNode;

    class Gate
    {
    public:
        std::string name;
        std::vector<std::shared_ptr<Bit>> qubits;
        std::vector<std::string> params;
        std::vector<std::shared_ptr<QASMNode>> body;
    };

} // namespace qasmcpp

#endif // QASM_REGISTER_H