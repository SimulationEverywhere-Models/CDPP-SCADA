[top]

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
