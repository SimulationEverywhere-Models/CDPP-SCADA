[top]
components :	WSserv1  redserv1@Red  hsserv1@HSServer  PDBserv1  drvserv1@Driver
In  :		hs_start
In  :		hs_stop
In  :		fo_master
In  :		fo_slave
In  :		server_in
Out :		server_out
Link :		hs_start		start@hsserv1
Link :		hs_stop			stop@hsserv1
Link :		fo_master		failover_master@hsserv1
Link :		fo_slave		failover_slave@hsserv1
Link :		start_drv@hsserv1	start@drvserv1
Link :		stop_drv@hsserv1	stop@drvserv1
Link :		reload_pdb@hsserv1	reload_pdb@PDBserv1
Link :		red_out@hsserv1		aux_in@redserv1
Link :		aux_out@redserv1	red_in@hsserv1
Link : 		nombre_logico@hsserv1	nombre_logico@redserv1
Link : 		nombre_logico@hsserv1	nombre_logico@WSserv1
Link : 		server_in		eth_in@redserv1
Link :		eth_out@redserv1	server_out
Link :		ws_out@redserv1		red_in@WSserv1
Link : 		red_out@WSserv1		ws_in@redserv1
Link :		pdb_out@redserv1	red_in@PDBserv1
Link : 		red_out@PDBserv1	pdb_in@redserv1
Link :		ws_out@PDBserv1		pdb_in@WSserv1
Link :		pdb_out@WSserv1		ws_in@PDBserv1
Link :		drv_pedido@PDBserv1	pedido@drvserv1
Link :		respuesta@drvserv1	drv_respuesta@PDBserv1

[WSserv1]
components :	selserv1@Selector	pantserv1@Pantalla
In  :		pdb_in
In  :		red_in
In  :		nombre_logico
Out :		pdb_out
Out :		red_out
Link :		nombre_logico		nombre_logico@selserv1
Link :		red_in			red_in@selserv1
Link :		pdb_in			pdb_in@selserv1
Link :		red_out@selserv1	red_out
Link :		pdb_out@selserv1	pdb_out
Link :		pant_out@selserv1	in@pantserv1
Link :		out@pantserv1		pant_in@selserv1

[selserv1]
tiempo_respuesta : 0:0:0:100
nodo_local	 : 1
nodo_log	 : 5
nodo_prim 	 : 1
nodo_sec	 : 2

[pantserv1]
refresh_rate :  0:0:1:0

[redserv1]
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

[hsserv1]
poll_time      : 0:0:7:0
max_reintentos : 5
nodo_local     : 1
nodo_log       : 5
nodo_prim      : 1
nodo_sec       : 2

[PDBserv1]
components : 	conserv1@Concentrador dbserv1@Database
Out  : 		drv_pedido
Out  : 		ws_out
Out  : 		red_out
In   : 		drv_respuesta
In   : 		ws_in
In   : 		red_in
In   : 		reload_pdb
Link : 		drv_respuesta		drv_respuesta@dbserv1
Link : 		ws_in			in2@conserv1
Link : 		red_in			in1@conserv1
Link : 		reload_pdb		recargar@dbserv1
Link : 		drv_pedido@dbserv1	drv_pedido
Link : 		out2@conserv1		ws_out
Link : 		out1@conserv1		red_out
Link :		out@conserv1		leer@dbserv1
Link :		registro_pedido@dbserv1	in@conserv1

[conserv1]
timeout : 0:0:5:0

[dbserv1]
tiempo_respuesta : 0:0:0:100
tiempo_recarga : 0:0:10:0
tiempo_scan : 0:0:0:200
nodo_local : 1
archivo_inicial : DB_MASTER

[drvserv1]
tiempo_respuesta : 0:0:0:100
