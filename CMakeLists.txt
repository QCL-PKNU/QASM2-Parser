# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.5 FATAL_ERROR)

# Add compiler option to disable the warning
add_compile_options(-Wno-attributes)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Define the project
project(QASM2Compiler)

LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

# compiler must be 11 or 14
SET (CMAKE_CXX_STANDARD 14)

# set variable pointing to the antlr tool that supports C++
set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.7-complete.jar)

# add external build for antlrcpp
include( ExternalAntlr4Cpp )

# add antrl4cpp artifacts to project environment
include_directories( ${ANTLR4CPP_INCLUDE_DIRS} )
include_directories(${CMAKE_SOURCE_DIR}/src/include)

link_directories( ${ANTLR4CPP_LIBS} )
message(STATUS "Found antlr4cpp libs: ${ANTLR4CPP_LIBS} and includes: ${ANTLR4CPP_INCLUDE_DIRS} ")



# Call macro to add lexer and grammar to your build dependencies.
antlr4cpp_process_grammar(QASM2Compiler qasmcpp
${CMAKE_CURRENT_SOURCE_DIR}/grammar/QASM2Lexer.g4
${CMAKE_CURRENT_SOURCE_DIR}/grammar/QASM2Parser.g4)

# include generated files in project environment
include_directories(${antlr4cpp_include_dirs_qasmcpp})

set(QASM2_SRC_FILES
  ${PROJECT_SOURCE_DIR}/src/include/Register.h
  ${PROJECT_SOURCE_DIR}/src/include/SymbolTable.h
  ${PROJECT_SOURCE_DIR}/src/include/AST.h
  ${PROJECT_SOURCE_DIR}/src/include/Expr.h
  ${PROJECT_SOURCE_DIR}/src/include/Visitor.h

  ${PROJECT_SOURCE_DIR}/src/lib/Register.cpp
  ${PROJECT_SOURCE_DIR}/src/lib/SymbolTable.cpp
  ${PROJECT_SOURCE_DIR}/src/lib/AST.cpp
  ${PROJECT_SOURCE_DIR}/src/lib/Expr.cpp
  ${PROJECT_SOURCE_DIR}/src/lib/Visitor.cpp
)

####### Google Test Integration
# Download and configure Google Test

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.10.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()


####### QPlayer Integration 
# QPlayer Integration (Manual Compilation)
option(BUILD_QPLAYER "Build and link QPlayer" OFF)

if(BUILD_QPLAYER)
  add_custom_target(QPlayer ALL
      COMMAND make -C ${PROJECT_SOURCE_DIR}/thirdparty/qplayer
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/thirdparty/qplayer
  )

  include_directories(thirdparty/qplayer/core/include)
  link_directories(thirdparty/qplayer/core/src)

  # Find and Link OpenMP
  find_package(OpenMP REQUIRED)
  if (OPENMP_FOUND)
      message(STATUS "OpenMP found")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  endif()

endif()

# print antlr4cpp_generation_qasmcpp
message(STATUS "antlr4cpp_generation_qasmcpp: ${antlr4cpp_generation_qasmcpp}")
message(STATUS "antlr4cpp_src_files_qasmcpp: ${antlr4cpp_src_files_qasmcpp}")
message(STATUS "QASM2_SRC_FILES: ${QASM2_SRC_FILES}")


add_executable(run_qasm2 
  main.cpp 
  ${antlr4cpp_src_files_qasmcpp} 
  ${QASM2_SRC_FILES} 
  )


if (BUILD_QPLAYER)
message(STATUS "Linking QPlayer")
set(LIB_QPLAYER ${PROJECT_SOURCE_DIR}/thirdparty/qplayer/release/lib/libqplayer.a)
target_link_libraries(run_qasm2 PRIVATE antlr4-runtime ${LIB_QPLAYER})
add_dependencies(run_qasm2 antlr4cpp antlr4cpp_generation_qasmcpp QPlayer OpenMP::OpenMP_CXX)
else()
# target_link_libraries(run_qasm2 PRIVATE antlr4-runtime)
  add_dependencies(run_qasm2 antlr4cpp antlr4cpp_generation_qasmcpp)
  target_link_libraries(run_qasm2 antlr4-runtime)
    message(STATUS "Not Linking QPlayer")
endif()


####### Add Google Test
add_executable(run_test
    test/LexerTests.cpp
    test/ParserTests.cpp
    test/ASTTests.cpp
    test/main.cpp
    ${antlr4cpp_src_files_qasmcpp}
    ${QASM2_SRC_FILES}
)

target_link_libraries(run_test PRIVATE gtest gtest_main antlr4-runtime)
add_dependencies(run_test antlr4cpp antlr4cpp_generation_qasmcpp)

add_test(NAME run_test COMMAND run_test)