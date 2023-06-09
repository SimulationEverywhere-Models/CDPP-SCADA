[top]

components :	WS PDB
In  :		nombre_logico
In  :		reload_pdb
Out :		respuesta_db
Out :		pedido_ws
Link : 		nombre_logico	nombre_logico@WS
Link : 		reload_pdb	reload_pdb@PDB
Link : 		ws_out@PDB	pdb_in@WS
Link : 		pdb_out@WS	ws_in@PDB
Link :		ws_out@PDB	respuesta_db
Link :		pdb_out@WS	pedido_ws

[PDB]
components : 	con@Concentrador db@Database
Out  : 		drv_pedido
Out  : 		ws_out
Out  : 		red_out
In   : 		drv_respuesta
In   : 		ws_in
In   : 		red_in
In   : 		reload_pdb
Link : 		drv_respuesta		drv_respuesta@db
Link : 		ws_in			in2@con
Link : 		red_in			in1@con
Link : 		reload_pdb		recargar@db
Link : 		drv_pedido@db		drv_pedido
Link : 		out2@con		ws_out
Link : 		out1@con		red_out
Link :		out@con			leer@db
Link :		registro_pedido@db	in@con

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

[con]
timeout : 0:0:5:0

[db]
tiempo_respuesta : 0:0:0:100
tiempo_recarga : 0:0:10:0
tiempo_scan : 0:0:0:200
nodo_local : 1
archivo_inicial : DB_MASTER

