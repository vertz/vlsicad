boolean a[3..0] b[3..0] d[15..0] cin
#
# Build a “correct” version of the circuit
adder 4 cout_gold sum[3..0] a[3..0] b[3..0] cin
#
# define the to-be-repaired version with the MUX
adder 1 c1 s0 a0 b0 cin
adder 1 c2 s1 a1 b1 c1
adder 1 c3 s2 a2 b2 c2
adder 1 c4 s3 a3 b3 c3
mux 4 Select q1 q2 q3 q4 d[15..0]
eval cout !Select&c4 + Select&cin
#
# make the Z function that compares the right version of
# the network and the version with the MUX replacing the
# the gate
eval Z cout_gold&cout + !cout_gold&!cout
#
# universally quantify away the non-mux vars: a b cin
quantify u ForallZ Z a[3..0] b[3..0] cin
#
# what values of the d’s make this function == 1?
satisfy ForallZ
#
#
eval q1 a0^b0
eval q2 a1^b1
eval q3 a2^b2
eval q4 a3^b3

eval f1 q1+q2+q3+q4
eval f2 q1&q2&q3&q4
eval f3 q1^q2^q3^q4

eval cout1 !f1&c4 + f1&cin
eval cout2 !f2&c4 + f2&cin
eval cout3 !f3&c4 + f3&cin

verify cout_gold cout1
verify cout_gold cout2
verify cout_gold cout3

quit
