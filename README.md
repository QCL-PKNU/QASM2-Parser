# QASM2 Parser

This project is a QASM2 (Quantum Assembly Language 2) parser implemented using ANTLR4 for lexing and parsing. This parser follow the standard grammer and syntax from orignal [Open Quantum Assembly Language](https://arxiv.org/pdf/1707.03429). 

The parser translates QASM2 code into an Abstract Syntax Tree (AST) for further processing. This parser includes the external source file or standard header such as `qelib1.inc`.

# Getting Started
## Prerequisites
- CMake 3.5 or higher
- Java 11 or higher
- C++ compiler supporting C++14
- Universally Unique IDs (uuid-dev)
    ```sh
    sudo apt-get install uuid-dev
    ```

## Building the project
1. Clone the repository:
    ```sh
    git clone <repository-url>
    cd qasm2-parser
    ```

2. Generate the build files using CMake:
    ```sh
    mkdir build
    cd build
    cmake ..
    ```
3. Build the project:
    ```sh
    make
    ```
    
4. Run the Parser:
    ```sh
    ./run_qasm2 <path-to-qasm-file>
    ```

5. Run Test
    ```sh
    ./run_test
    ```


## Project structure
```sh
.
├── cmake
│   └── ExternalAntlr4Cpp.cmake   # CMake script to handle external ANTLR4 dependencies
├── CMakeLists.txt                # CMake configuration file
├── gen_files.sh                  # Script to generate file_contents.txt
├── grammar
│   ├── QASM2Lexer.g4             # ANTLR4 grammar for QASM2 lexer
│   └── QASM2Parser.g4            # ANTLR4 grammar for QASM2 parser
├── main.cpp                      # Main entry point for the QASM2 parser
├── README.md                     # Project documentation
├── src
│   ├── include
│   │   ├── AST.h                 # Header for Abstract Syntax Tree
│   │   ├── Expr.h                # Header for expressions
│   │   ├── Register.h            # Header for quantum register
│   │   ├── SymbolTable.h         # Header for symbol table
│   │   └── Visitor.h             # Header for visitor pattern
│   └── lib
│       ├── AST.cpp               # Implementation of AST
│       ├── Expr.cpp              # Implementation of expressions
│       ├── Register.cpp          # Implementation of quantum register
│       ├── SymbolTable.cpp       # Implementation of symbol table
│       └── Visitor.cpp           # Implementation of visitor pattern
└── thirdparty
    ├── antlr
    │   └── antlr-4.7-complete.jar # ANTLR4 tool
    └── qplayer                    # QPlayer integration
```

## AST usage
AST nodes are generated by walking through the parse tree.
`program` is `QASMNode` that obtains the statement nodes. You can traverse that statements for furture process such code generation or execution.
```cpp
...
    QASM2Visitor visitor;
    visitor.visit(tree);
    auto program = visitor.getProgram();
...
```
While traverse the parse tree, it performs semantic analysis to generate Symbol Table. All registers (Qubit and CBit) and user-defined gates are stored in `Symbol Table`. The user-defined gates is the gate that user defines or it's may from `qelib1.inc`.
```cpp
    auto gateDefines = visitor.getSymbolTable().gateDefines;
    auto regDefines = visitor.getSymbolTable().qubitRegisters;
    auto cregDefines = visitor.getSymbolTable().cbitRegisters;
```

## Example of link with simulator
We will support simulator as backend to execute circuit.
Example of link library with QPlayer
1. Clone simulator
    ```sh
    cd ./thirdparty
    git clone https://github.com/eQuantumOS/QPlayer.git qplayer
    ```
2. Build with simulator
    ```sh
    cd ../build
    cmake .. -DBUILD_QPLAYER=ON 
    make;
    ```

For more details on link the library to used in here, check in CMakeLists.txt


## File Content Generation
The `gen_files.sh` script can be used to generate `file_contents.txt`, which lists all the files in the project directory, excluding certain directories and file types. It is for asking ChatGPT as you wish. :)

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgements
- [ANTLR4](https://www.antlr.org/)

For more details on the usage and contribution guidelines, refer to the CONTRIBUTING.md file.
