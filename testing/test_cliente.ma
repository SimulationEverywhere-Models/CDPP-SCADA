[top]
components :	WScliente   redcliente@Red   hscliente@HSClient
In  :		hs_start
In  :		hs_stop
In  :		client_in
Out :		client_out
Link :		hs_start		start@hscliente
Link :		hs_stop			stop@hscliente
Link :		red_out@hscliente	aux_in@redcliente
Link :		aux_out@redcliente	red_in@hscliente
Link : 		nombre_logico@hscliente	nombre_logico@redcliente
Link : 		nombre_logico@hscliente	nombre_logico@WScliente
Link : 		client_in		eth_in@redcliente
Link :		eth_out@redcliente	client_out
Link :		ws_out@redcliente	red_in@WScliente
Link : 		red_out@WScliente	ws_in@redcliente

[WScliente]
components :	selcliente@Selector	pantcliente@Pantalla
In  :		pdb_in
In  :		red_in
In  :		nombre_logico
Out :		pdb_out
Out :		red_out
Link :		nombre_logico		nombre_logico@selcliente
Link :		red_in			red_in@selcliente
Link :		pdb_in			pdb_in@selcliente
Link :		red_out@selcliente	red_out
Link :		pdb_out@selcliente	pdb_out
Link :		pant_out@selcliente	in@pantcliente
Link :		out@pantcliente		pant_in@selcliente

[selcliente]
tiempo_respuesta : 0:0:0:100
nodo_local	 : 3
nodo_log	 : NONE
nodo_prim 	 : NONE
nodo_sec	 : NONE

[pantcliente]
refresh_rate :  0:0:1:07

[redcliente]
tiempo_respuesta : 0:0:0:25
nodo_local	 : 3
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

[hscliente]
poll_time    : 0:0:5:0
nodo_hs	     : 5
nodo_hs_prim : 1
nodo_hs_sec  : 2
