lexer grammar QASM2Lexer;

channels { CommentsChannel }

OPENQASM: 'OPENQASM' | 'openqasm';
INCLUDE: 'include';
QREG: 'qreg';
CREG: 'creg';
MEASURE: 'measure';
GATE: 'gate';
IF: 'if';
RESET: 'reset';
OPAQUE: 'opaque';
U: 'U';
CX: 'CX';
BARRIER: 'barrier';

LBRACE: '{';
RBRACE: '}';
LBRACKET: '[';
RBRACKET: ']';
LPAREN: '(';
RPAREN: ')';
SEMICOLON: ';';
COMMA: ',';
EQ: '==';
ARROW: '->';

PLUS: '+';
MINUS: '-';
TIMES: '*';
DIVIDE: '/';
POWER: '^';
PI: 'pi';

SIN: 'sin';
COS: 'cos';
TAN: 'tan';
EXP: 'exp';
LN: 'ln';
SQRT: 'sqrt';
STAR: '*';

ID: [a-z][A-Za-z0-9_]*;
NNINTEGER: [0-9]+;
REAL: INTEGER '.' [0-9]+ ([eE][+-]? [0-9]+)?;

STRING: '"' .*? '"';

COMMENT : '#' ~[\r\n]* '\r'? '\n' -> channel(CommentsChannel);
LINE_COMMENT : '//' ~[\r\n]* -> channel(CommentsChannel);

WS: [ \t\u000C\r\n]+ -> skip;

fragment INTEGER: [+-]? NNINTEGER;