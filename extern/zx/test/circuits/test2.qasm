OPENQASM 2.0;
include "qelib1.inc";
qreg q[3];
h q[0];
cx q[0], q[1];
cx q[1], q[2];
u1(pi/8) q[2];
cx q[1], q[2];
cx q[0], q[1];
cp(pi/8) q[0], q[1];
