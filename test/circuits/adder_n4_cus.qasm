OPENQASM 2.0;
include "../test/circuits/qelib1.inc";
include "../test/circuits/myLibrary.inc";

qreg q[4];
creg c[4];

gate my_gate2 a,b,c {
    U(0.7853981633974483,0,pi) a;
    U(sin(pi), cos(pi), pi) b;
}

gate my_gate3 (theta, phi, lambda) a,b,c {
    CX a, b;
    CX b, c;
}

x q[0];
h q[3];
cx q[2],q[3];
t q[2];
tdg q[3];
cx q[0],q[1];
cx q[1],q[2];
cx q[2],q[3];
s q[3];
cx q[3],q[0];
h q[3];
CX q[0],q[1];
reset q[0];

measure q[0] -> c[0];
measure q[1] -> c[1];
measure q[2] -> c[2];
measure q[3] -> c[3];
