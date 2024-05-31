#ifndef QASM_YMBOL_TABLE_H
#define QASM_SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <memory>

#include "Register.h"

// Symbol table for managing qubit and classical registers
/**
 * @class SymbolTable
 * @brief Represents a symbol table for qasm2-parser.
 *
 * This class stores information about qubit and cbit registers.
 * It provides methods to add and retrieve qubit and cbit registers.
 */

using namespace qasmcpp;
namespace qasmcpp
{

    class SymbolTable
    {
    public:
        std::unordered_map<std::string, std::shared_ptr<Register>> qubitRegisters; /**< Map of qubit registers. */
        std::unordered_map<std::string, std::shared_ptr<Register>> cbitRegisters;  /**< Map of cbit registers. */
        std::unordered_map<std::string, std::shared_ptr<Gate>> gateDefines;        /** Map of gate definitions */

        // gate declaration
        std::unordered_map<std::string, std::shared_ptr<Register>> gates;

        /**
         * @brief Adds a qubit register to the symbol table.
         *
         * @param name The name of the qubit register.
         * @param size The size of the qubit register.
         */
        void addQubitRegister(const std::string &name, int size);

        /**
         * @brief Retrieves a qubit register from the symbol table.
         *
         * @param name The name of the qubit register.
         * @return A shared pointer to the qubit register, or nullptr if not found.
         */
        std::shared_ptr<Register> getQubitRegister(const std::string &name);

        /**
         * @brief Adds a cbit register to the symbol table.
         *
         * @param name The name of the cbit register.
         * @param size The size of the cbit register.
         */
        void addCbitRegister(const std::string &name, int size);

        void addRegister(const std::string &name, int size, BitType type);

        /**
         * @brief Retrieves a cbit register from the symbol table.
         *
         * @param name The name of the cbit register.
         * @return A shared pointer to the cbit register, or nullptr if not found.
         */
        std::shared_ptr<Register> getCbitRegister(const std::string &name);

        /**
         * @brief Adds a gate definition to the symbol table.
         *
         * @param name The name of the gate.
         * @param gate The gate definition.
         */
        void addGateDef(const std::string &name, std::shared_ptr<Gate> gate);

        /**
         * @brief Retrieves a gate definition from the symbol table.
         *
         * @param name The name of the gate.
         * @return A shared pointer to the gate definition, or nullptr if not found.
         */
        std::shared_ptr<Gate> getGateDef(const std::string &name);

        /**
         * @brief Checks if a qubit register exists in the symbol table.
         *
         * @param name The name of the qubit register.
         * @return True if the qubit register exists, false otherwise.
         */
        inline bool hasGateDef(const std::string &name)
        {
            return gateDefines.find(name) != gateDefines.end();
        }

        /**
         * @brief Checks if a qubit register exists in the symbol table.
         *
         * @param name The name of the qubit register.
         * @return True if the qubit register exists, false otherwise.
         */
        inline bool isQubitRegister(const std::string &name)
        {
            return qubitRegisters.find(name) != qubitRegisters.end();
        }

        /**
         * @brief Checks if a cbit register exists in the symbol table.
         *
         * @param name The name of the cbit register.
         * @return True if the cbit register exists, false otherwise.
         */
        inline bool isCbitRegister(const std::string &name)
        {
            return cbitRegisters.find(name) != cbitRegisters.end();
        }
        
        /**
         * @brief Checks if a register exists in the symbol table.
         *
         * @param name The name of the register.
         * @return True if the register exists, false otherwise.
         */
        inline bool isRegister(const std::string &name)
        {
            return isQubitRegister(name) || isCbitRegister(name);
        }
    };
} // namespace qasmcpp
#endif // SYMBOL_TABLE_H