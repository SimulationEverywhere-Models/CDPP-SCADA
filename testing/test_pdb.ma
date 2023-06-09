[top]
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

