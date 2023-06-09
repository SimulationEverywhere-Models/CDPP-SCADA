[top]

components :	PDB red@Red eth@Ethernet
In  :		in2
In  :		in3
In  :		nombre_logico
In  :		reload_pdb 
Out :		out2
Out :		out3
Link : 		in2		in2@eth
Link : 		in3		in3@eth
Link : 		out2@eth	out2
Link : 		out3@eth	out3
Link :		out1@eth	eth_in@red
Link :		eth_out@red	in1@eth
Link :		pdb_out@red	red_in@PDB
Link :		red_out@PDB 	pdb_in@red
Link :		nombre_logico	nombre_logico@red
Link :		reload_pdb	reload_pdb@PDB

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

[con]
timeout : 0:0:5:0

[db]
tiempo_respuesta : 0:0:0:100
tiempo_recarga : 0:0:10:0
tiempo_scan : 0:0:0:200
nodo_local : 1
archivo_inicial : DB_MASTER

[red]
tiempo_respuesta : 0:0:0:100
nodo_local	 : 1
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

