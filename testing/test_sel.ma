[top]
components : 	sel@Selector
Out  : 		pdb_out
Out  : 		pant_out
Out  : 		red_out
In   :		pdb_in
In   : 		pant_in
In   : 		red_in
In   : 		nombre_logico
Link : 		pdb_in 				pdb_in@sel
Link : 		pant_in     			pant_in@sel
Link : 		red_in  			red_in@sel
Link : 		nombre_logico 	 		nombre_logico@sel
Link : 		pdb_out@sel			pdb_out
Link : 		pant_out@sel			pant_out
Link : 		red_out@sel			red_out

[sel]
tiempo_respuesta : 0:0:0:100
nodo_local	 : 1
nodo_log	 : 5
nodo_prim 	 : 1
nodo_sec	 : 2
