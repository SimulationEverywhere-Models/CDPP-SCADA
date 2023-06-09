[top]
components : eth@Ethernet
In   : in1
In   : in2
In   : in3
In   : hab
In   : inh
Out  : out1
Out  : out2
Out  : out3

Link : in1	in1@eth
Link : in2	in2@eth
Link : in3	in3@eth
Link : hab	habilitar@eth
Link : inh	inhabilitar@eth
Link : out1@eth	out1
Link : out2@eth	out2
Link : out3@eth	out3


[eth]
delay : 0:0:0:100
nodo_1 : 1
nodo_2 : 2
nodo_3 : 3

