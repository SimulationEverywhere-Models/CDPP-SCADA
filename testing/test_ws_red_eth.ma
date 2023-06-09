[top]

components :	WS red@Red eth@Ethernet
In  :		in2
In  :		in3
In  :		nombre_logico
Out :		out2
Out :		out3
Link : 		in2		in2@eth
Link : 		in3		in3@eth
Link : 		out2@eth	out2
Link : 		out3@eth	out3
Link :		out1@eth	eth_in@red
Link :		eth_out@red	in1@eth
Link : 		nombre_logico	nombre_logico@red
Link :		ws_out@red	red_in@WS
Link : 		red_out@WS	ws_in@red
Link : 		nombre_logico	nombre_logico@WS

[WS]
components :	sel@Selector	pant@Pantalla
In  :		pdb_in
In  :		red_in
In  :		nombre_logico
Out :		pdb_out
Out :		red_out
Link :		nombre_logico	nombre_logico@sel
Link :		red_in		red_in@sel
Link :		pdb_in		pdb_in@sel
Link :		red_out@sel	red_out
Link :		pdb_out@sel	pdb_out
Link :		pant_out@sel	in@pant
Link :		out@pant	pant_in@sel

[sel]
tiempo_respuesta : 0:0:0:100
nodo_local	 : 1
nodo_log	 : 5
nodo_prim 	 : 1
nodo_sec	 : 2

[pant]
refresh_rate :  0:0:1:0

[red]
tiempo_respuesta : 0:0:0:100
nodo_local	 : 2
nodo_1 		 : 5
nodo_1_prim 	 : 1
nodo_1_sec	 : 2
nodo_2 		 : 3
nodo_2_prim 	 : NONE
nodo_2_sec	 : NONE
nodo_3 		 : NONE
nodo_3_prim 	 : NONE
nodo_3_sec	 : NONE
nodo_4 		 : NONE
nodo_4_prim 	 : NONE
nodo_4_sec	 : NONE
nodo_5 		 : NONE
nodo_5_prim 	 : NONE
nodo_5_sec	 : NONE

[eth]
delay : 0:0:0:100
nodo_1 : 1
nodo_2 : 2
nodo_3 : 3

