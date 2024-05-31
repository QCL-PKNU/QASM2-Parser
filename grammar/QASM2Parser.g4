parser grammar QASM2Parser;

options {
	tokenVocab = QASM2Lexer;
}

// Follows directly after the standard #includes in h + cpp files.
@parser::postinclude {
  /* parser postinclude section */
  #ifndef _WIN32
  #pragma GCC diagnostic ignored "-Wunused-parameter"
  #endif

  #include "Expr.h"
}

// Actual grammar start.
main: version statement* EOF;

version returns [std::string ver]
  : OPENQASM REAL SEMICOLON { $ver = $REAL.text; }
  ;

statement 
  : includeDeclStmt
  | regDeclStmt
  | gateDeclStmt
  | opaqueDeclStmt
  | qopStmt
  | ifDeclStmt
  | barrierDeclStmt
  ;


includeDeclStmt returns [std::string filename]
  : INCLUDE STRING SEMICOLON 
  {
    $filename = $STRING.text;
  }
  ;

regDeclStmt
  : QREG ID LBRACKET NNINTEGER RBRACKET SEMICOLON
  | CREG ID LBRACKET NNINTEGER RBRACKET SEMICOLON
  ;

gateDeclStmt returns [bool hasParams]
  : GATE ID idList LBRACE uop* RBRACE { $hasParams = false; }
  | GATE ID LPAREN RPAREN idList LBRACE uop* RBRACE { $hasParams = false;}
  | GATE ID LPAREN idList RPAREN idList LBRACE uop* RBRACE { $hasParams = true;}
  ;

opaqueDeclStmt
  : OPAQUE ID idList SEMICOLON
  | OPAQUE ID LPAREN RPAREN idList SEMICOLON
  | OPAQUE ID LPAREN idList RPAREN idList SEMICOLON
  ;

qopStmt returns [std::string gateName, int type]
  : uop                                               { $gateName = $uop.gateName;  $type = $uop.type; }
  | MEASURE argument ARROW argument SEMICOLON         { $gateName = "MEASURE";      $type = QASM2Parser::MEASURE; }
  | RESET argument SEMICOLON                          { $gateName = "RESET";        $type = QASM2Parser::RESET;}
  ;

ifDeclStmt
  : IF LPAREN ID EQ NNINTEGER RPAREN qopStmt
  ;

barrierDeclStmt
  : BARRIER mixedList SEMICOLON
  ;

// I don't think BACKSLASH is used in the grammar, 
// so I removed it and repalce in uop in gateDeclStmt
// gopList 
//   : (uop | BARRIER idList SEMICOLON)+
//   ;


uop returns [std::string gateName, int type]
  : U LPAREN expList RPAREN argument SEMICOLON    { $gateName = "U";      $type = QASM2Parser::U;}
  | CX argument COMMA argument SEMICOLON          { $gateName = "CX";     $type = QASM2Parser::CX;}
  | ID mixedList SEMICOLON                          { $gateName = $ID.text; $type = QASM2Parser::ID;}
  | ID LPAREN RPAREN mixedList SEMICOLON            { $gateName = $ID.text; $type = QASM2Parser::ID;}
  | ID LPAREN expList RPAREN mixedList SEMICOLON    { $gateName = $ID.text; $type = QASM2Parser::ID;}
  ;

idList
  : (ID COMMA)* ID
  ;

mixedList
  : (argument COMMA)* argument
  ;
  
argument 
  : ID 
  | ID LBRACKET NNINTEGER RBRACKET 
  ;

expList 
  : (exp COMMA)* exp 
  ;

exp returns [int exprType]
  : REAL                        { $exprType = ExprNode::REAL; }
  | NNINTEGER                   { $exprType = ExprNode::NNINTEGER; }
  | PI                          { $exprType = ExprNode::PI; }
  | ID                          { $exprType = ExprNode::ID; }
  | exp op exp                  { $exprType = ExprNode::BINARY; }
  | MINUS exp                   { $exprType = ExprNode::NAG; }
  | LPAREN exp RPAREN           { $exprType = ExprNode::EXPR; }
  | unaryop LPAREN exp RPAREN   { $exprType = ExprNode::UNARY; }
  ;

op returns [ExprNode::ArithOpType opType]
  : PLUS    { $opType = ExprNode::ArithOpType::PLUS; }
  | MINUS   { $opType = ExprNode::ArithOpType::MINUS; }
  | TIMES   { $opType = ExprNode::ArithOpType::TIMES; }
  | DIVIDE  { $opType = ExprNode::ArithOpType::DIVIDE; }
  | POWER   { $opType = ExprNode::ArithOpType::POWER; }  
  ;

unaryop returns [ExprNode::UnaryOpType opType]
  : SIN     { $opType = ExprNode::UnaryOpType::SIN; }
  | COS     { $opType = ExprNode::UnaryOpType::COS; }
  | TAN     { $opType = ExprNode::UnaryOpType::TAN; }
  | EXP     { $opType = ExprNode::UnaryOpType::EXP; }
  | LN      { $opType = ExprNode::UnaryOpType::LN; }
  | SQRT    { $opType = ExprNode::UnaryOpType::SQRT; }
  ;

catch [...] {
  // Replaces the standard exception handling.
  
}
finally {

}