[top]
components :	WSserv2  redserv2@Red  hsserv2@HSServer  PDBserv2  drvserv2@Driver
In  :		hs_start
In  :		hs_stop
In  :		fo_master
In  :		fo_slave
In  :		server_in
Out :		server_out
Link :		hs_start		start@hsserv2
Link :		hs_stop			stop@hsserv2
Link :		fo_master		failover_master@hsserv2
Link :		fo_slave		failover_slave@hsserv2
Link :		start_drv@hsserv2	start@drvserv2
Link :		stop_drv@hsserv2	stop@drvserv2
Link :		reload_pdb@hsserv2	reload_pdb@PDBserv2
Link :		red_out@hsserv2		aux_in@redserv2
Link :		aux_out@redserv2	red_in@hsserv2
Link : 		nombre_logico@hsserv2	nombre_logico@redserv2
Link : 		nombre_logico@hsserv2	nombre_logico@WSserv2
Link : 		server_in		eth_in@redserv2
Link :		eth_out@redserv2	server_out
Link :		ws_out@redserv2		red_in@WSserv2
Link : 		red_out@WSserv2		ws_in@redserv2
Link :		pdb_out@redserv2	red_in@PDBserv2
Link : 		red_out@PDBserv2	pdb_in@redserv2
Link :		ws_out@PDBserv2		pdb_in@WSserv2
Link :		pdb_out@WSserv2		ws_in@PDBserv2
Link :		drv_pedido@PDBserv2	pedido@drvserv2
Link :		respuesta@drvserv2	drv_respuesta@PDBserv2

[WSserv2]
components :	selserv2@Selector	pantserv2@Pantalla
In  :		pdb_in
In  :		red_in
In  :		nombre_logico
Out :		pdb_out
Out :		red_out
Link :		nombre_logico		nombre_logico@selserv2
Link :		red_in			red_in@selserv2
Link :		pdb_in			pdb_in@selserv2
Link :		red_out@selserv2	red_out
Link :		pdb_out@selserv2	pdb_out
Link :		pant_out@selserv2	in@pantserv2
Link :		out@pantserv2		pant_in@selserv2

[selserv2]
tiempo_respuesta : 0:0:0:80
nodo_local	 : 2
nodo_log	 : 5
nodo_prim 	 : 1
nodo_sec	 : 2

[pantserv2]
refresh_rate :  0:0:1:5

[redserv2]
tiempo_respuesta : 0:0:0:75
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

[hsserv2]
poll_time      : 0:0:6:300
max_reintentos : 5
nodo_local     : 2
nodo_log       : 5
nodo_prim      : 1
nodo_sec       : 2

[PDBserv2]
components : 	conserv2@Concentrador dbserv2@Database
Out  : 		drv_pedido
Out  : 		ws_out
Out  : 		red_out
In   : 		drv_respuesta
In   : 		ws_in
In   : 		red_in
In   : 		reload_pdb
Link : 		drv_respuesta		drv_respuesta@dbserv2
Link : 		ws_in			in2@conserv2
Link : 		red_in			in1@conserv2
Link : 		reload_pdb		recargar@dbserv2
Link : 		drv_pedido@dbserv2	drv_pedido
Link : 		out2@conserv2		ws_out
Link : 		out1@conserv2		red_out
Link :		out@conserv2		leer@dbserv2
Link :		registro_pedido@dbserv2	in@conserv2

[conserv2]
timeout : 0:0:5:0

[dbserv2]
tiempo_respuesta : 0:0:0:100
tiempo_recarga : 0:0:10:0
tiempo_scan : 0:0:0:200
nodo_local : 2
archivo_inicial : DB_MASTER

[drvserv2]
tiempo_respuesta : 0:0:0:100
