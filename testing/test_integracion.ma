#===========================================================
#
# Modelo "top": Dos servidores con redundancia y un cliente
#
#

[top]
components :	serv1	serv2	cliente	  eth@Ethernet
In   :		eth_hab
In   :		eth_inhab
In   :		hss1_start
In   :		hss1_stop
In   :		fo1_master
In   :		fo1_slave
In   :		hss2_start
In   :		hss2_stop
In   :		fo2_master
In   :		fo2_slave
In   :		hsc_start
In   :		hsc_stop
Link :		server_out@serv1	in1@eth
Link : 		out1@eth		server_in@serv1
Link :		server_out@serv2	in2@eth
Link : 		out2@eth		server_in@serv2
Link :		client_out@cliente	in3@eth
Link : 		out3@eth		client_in@cliente
Link :		eth_hab			habilitar@eth
Link :		eth_inhab		inhabilitar@eth
#
#
# Control del Servidor 1
#
Link :		hss1_start	hs_start@serv1
Link :		hss1_stop	hs_stop@serv1
Link :		fo1_master	fo_master@serv1
Link :		fo1_slave	fo_slave@serv1
#
#
# Control del Servidor 2
#
Link :		hss2_start	hs_start@serv2
Link :		hss2_stop	hs_stop@serv2
Link :		fo2_master	fo_master@serv2
Link :		fo2_slave	fo_slave@serv2
#
#
# Control del Cliente
#
Link :		hsc_start	hs_start@cliente
Link :		hsc_stop	hs_stop@cliente

[eth]
delay : 0:0:0:0
nodo_1 : 1
nodo_2 : 2
nodo_3 : 3

#-----------------------------------------------------------
#  CLIENTE
#
#  Nodo Fisico: 3
#
#
#

[cliente]
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
tiempo_respuesta : 0:0:0:0
nodo_local	 : 3
nodo_log	 : NONE
nodo_prim 	 : NONE
nodo_sec	 : NONE

[pantcliente]
refresh_rate :  0:0:30:07

[redcliente]
tiempo_respuesta : 0:0:0:0
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
poll_time    : 0:0:3:000
nodo_hs	     : 5
nodo_hs_prim : 1
nodo_hs_sec  : 2

#-----------------------------------------------------------
#  SERVIDOR 1
#
#  Nodo Logico: 5
#  Nodo Fisico: 1
#
#
#

[serv1]
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
tiempo_respuesta : 0:0:0:0
nodo_local	 : 1
nodo_log	 : 5
nodo_prim 	 : 1
nodo_sec	 : 2

[pantserv1]
refresh_rate :  0:0:10:0

[redserv1]
tiempo_respuesta : 0:0:0:0
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
poll_time      : 0:0:3:333
max_reintentos : 3
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
timeout : 0:0:0:1

[dbserv1]
tiempo_respuesta : 0:0:0:0
tiempo_recarga : 0:0:0:0
tiempo_scan : 0:0:0:200
nodo_local     : 1
archivo_inicial : DB_MASTER

[drvserv1]
tiempo_respuesta : 0:0:0:0

#-----------------------------------------------------------
#  SERVIDOR 2
#
#  Nodo Logico: 5
#  Nodo Fisico: 2
#
#
#

[serv2]
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
tiempo_respuesta : 0:0:0:0
nodo_local	 : 2
nodo_log	 : 5
nodo_prim 	 : 1
nodo_sec	 : 2

[pantserv2]
refresh_rate :  0:0:1:5

[redserv2]
tiempo_respuesta : 0:0:0:0
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
poll_time      : 0:0:3:333
max_reintentos : 3
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
timeout : 0:0:0:1

[dbserv2]
tiempo_respuesta : 0:0:0:0
tiempo_recarga : 0:0:0:0
tiempo_scan : 0:0:0:200
nodo_local     : 2
archivo_inicial : DB_SLAVE

[drvserv2]
tiempo_respuesta : 0:0:0:0
