CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(antlr4cpp_fetcher CXX)
INCLUDE(ExternalProject)
FIND_PACKAGE(Git REQUIRED)

# only JRE required
FIND_PACKAGE(Java COMPONENTS Runtime REQUIRED)

############ Download and Generate runtime #################
set(ANTLR4CPP_EXTERNAL_ROOT ${CMAKE_BINARY_DIR}/externals/antlr4cpp)
set(ANTLR4CPP_LOCAL_ROOT ${CMAKE_BINARY_DIR}/locals/antlr4cpp)

# external repository
# GIT_REPOSITORY     https://github.com/antlr/antlr4.git
set(ANTLR4CPP_EXTERNAL_REPO "https://github.com/antlr/antlr4.git")
set(ANTLR4CPP_EXTERNAL_TAG  "4.7")
# SET(ANTLR4CPP_LOCAL_REPO ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr4-master.zip)

if(NOT EXISTS "${ANTLR4CPP_JAR_LOCATION}")
  message(FATAL_ERROR "Unable to find antlr tool. ANTLR4CPP_JAR_LOCATION:${ANTLR4CPP_JAR_LOCATION}")
endif()

# default path for source files
if (NOT ANTLR4CPP_GENERATED_SRC_DIR)
  set(ANTLR4CPP_GENERATED_SRC_DIR ${CMAKE_BINARY_DIR}/antlr4cpp_generated_src)
endif()

# download runtime environment
ExternalProject_ADD(
  #--External-project-name------
  antlr4cpp
  #--Depend-on-antrl-tool-----------
  # DEPENDS antlrtool
  #--Core-directories-----------
  PREFIX             ${ANTLR4CPP_LOCAL_ROOT}
  #--Download step--------------
  # URL                 ${ANTLR4CPP_LOCAL_REPO}
  GIT_REPOSITORY     ${ANTLR4CPP_EXTERNAL_REPO}
  GIT_TAG            ${ANTLR4CPP_EXTERNAL_TAG}
  TIMEOUT            10
  LOG_DOWNLOAD       ON
  #--Update step----------
  # UPDATE_COMMAND     ${GIT_EXECUTABLE} pull
  #--Patch step----------
  # PATCH_COMMAND sh -c "cp <SOURCE_DIR>/scripts/CMakeLists.txt <SOURCE_DIR>"
  #--Configure step-------------
  CONFIGURE_COMMAND  ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug -DANTLR4CPP_JAR_LOCATION=${ANTLR4CPP_JAR_LOCATION} -DBUILD_SHARED_LIBS=ON -DBUILD_TESTS=ON -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DCMAKE_SOURCE_DIR:PATH=<SOURCE_DIR>/runtime/Cpp <SOURCE_DIR>/runtime/Cpp
  LOG_CONFIGURE ON
  #--Build step-----------------
  # BUILD_COMMAND ${CMAKE_MAKE_PROGRAM}
  LOG_BUILD ON
  #--Install step---------------
  # INSTALL_COMMAND    ""
  # INSTALL_DIR ${CMAKE_BINARY_DIR}/
  #--Install step---------------
  # INSTALL_COMMAND    ""
)

ExternalProject_Get_Property(antlr4cpp INSTALL_DIR)

list(APPEND ANTLR4CPP_INCLUDE_DIRS ${INSTALL_DIR}/include/antlr4-runtime)
foreach(src_path misc atn dfa tree support)
  list(APPEND ANTLR4CPP_INCLUDE_DIRS ${INSTALL_DIR}/include/antlr4-runtime/${src_path})
endforeach(src_path)

set(ANTLR4CPP_LIBS "${INSTALL_DIR}/lib")

# antlr4_shared ${INSTALL_DIR}/lib/libantlr4-runtime.so
# antlr4_static ${INSTALL_DIR}/lib/libantlr4-runtime.a

############ Generate runtime #################
# macro to add dependencies to target
#
# Param 1 project name
# Param 1 namespace (postfix for dependencies)
# Param 2 Lexer file (full path)
# Param 3 Parser File (full path)
#
# output
#
# antlr4cpp_src_files_{namespace} - src files for add_executable
# antlr4cpp_include_dirs_{namespace} - include dir for generated headers
# antlr4cpp_generation_{namespace} - for add_dependencies tracking
set(antlr4cpp_project_namespace "qasmcpp")
set(antlr4cpp_src_files_${antlr4cpp_project_namespace})


macro(antlr4cpp_process_grammar
antlr4cpp_project
antlr4cpp_project_namespace
antlr4cpp_grammar_lexer
antlr4cpp_grammar_parser)


  if(EXISTS "${ANTLR4CPP_JAR_LOCATION}")
    message(STATUS "Found antlr tool: ${ANTLR4CPP_JAR_LOCATION}")
  else()
    message(FATAL_ERROR "Unable to find antlr tool. ANTLR4CPP_JAR_LOCATION:${ANTLR4CPP_JAR_LOCATION}")
  endif()

  add_custom_target("antlr4cpp_generation_${antlr4cpp_project_namespace}"
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ANTLR4CPP_GENERATED_SRC_DIR}
    COMMAND
    "${Java_JAVA_EXECUTABLE}" -jar "${ANTLR4CPP_JAR_LOCATION}" -Werror -Dlanguage=Cpp -no-listener -visitor -o "${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace}" -package ${antlr4cpp_project_namespace} "${antlr4cpp_grammar_lexer}" "${antlr4cpp_grammar_parser}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS "${antlr4cpp_grammar_lexer}" "${antlr4cpp_grammar_parser}"
    )

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ANTLR4CPP_GENERATED_SRC_DIR}
    COMMAND
    ${Java_JAVA_EXECUTABLE} -jar ${ANTLR4CPP_JAR_LOCATION} -Werror -Dlanguage=Cpp -no-listener -visitor -o ${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace} -package ${antlr4cpp_project_namespace} ${antlr4cpp_grammar_lexer} ${antlr4cpp_grammar_parser}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

  message(STATUS "GEN: ${Java_JAVA_EXECUTABLE} -jar ${ANTLR4CPP_JAR_LOCATION} -Werror -Dlanguage=Cpp -no-listener -visitor -o ${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace} -package ${antlr4cpp_project_namespace} ${antlr4cpp_grammar_lexer} ${antlr4cpp_grammar_parser}")
  # Find all the input files
  FILE(GLOB generated_files ${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace}/*.cpp)
  

  # export generated cpp files into list
  foreach(generated_file ${generated_files})
    list(APPEND antlr4cpp_src_files_${antlr4cpp_project_namespace} ${generated_file})
    set_source_files_properties(
      ${generated_file}
      PROPERTIES
      COMPILE_FLAGS -Wno-overloaded-virtual
      )
  endforeach(generated_file)
  message(STATUS "Antlr4Cpp  ${antlr4cpp_project_namespace} Generated: ${generated_files}")

  # export generated include directory
  set(antlr4cpp_include_dirs_${antlr4cpp_project_namespace} ${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace})
  message(STATUS "Antlr4Cpp ${antlr4cpp_project_namespace} include: ${ANTLR4CPP_GENERATED_SRC_DIR}/${antlr4cpp_project_namespace}")

endmacro()