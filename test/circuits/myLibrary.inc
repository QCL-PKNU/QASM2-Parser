OPENQASM 2.0;
gate my_gate (theta, phi, lambda) a,b,c {
    CX a, b;
    CX b, c;
}