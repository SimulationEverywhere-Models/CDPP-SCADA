[top]
components : 	red@Red
Out  : 		pdb_out
Out  : 		ws_out
Out  : 		eth_out
Out  : 		aux_out
In   :		pdb_in
In   : 		ws_in
In   : 		eth_in
In   : 		aux_in
In   : 		nombre_logico
Link : 		pdb_in 				pdb_in@red
Link : 		ws_in     			ws_in@red
Link : 		eth_in  			eth_in@red
Link : 		aux_in  			aux_in@red
Link : 		nombre_logico 	 		nombre_logico@red
Link : 		pdb_out@red			pdb_out
Link : 		ws_out@red			ws_out
Link : 		eth_out@red			eth_out
Link : 		aux_out@red			aux_out

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
