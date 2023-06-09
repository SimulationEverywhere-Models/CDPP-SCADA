[top]

components : hss@HSServer
In   : start
In   : stop
In   : failover_master
In   : failover_slave
In   : red_in
Out  : start_drv
Out  : stop_drv
Out  : reload_pdb
Out  : nombre_logico
Out  : red_out
Link : start			start@hss
Link : stop			stop@hss
Link : failover_master		failover_master@hss
Link : failover_slave		failover_slave@hss
Link : red_in			red_in@hss
Link : start_drv@hss		start_drv
Link : stop_drv@hss		stop_drv
Link : reload_pdb@hss		reload_pdb
Link : nombre_logico@hss	nombre_logico
Link : red_out@hss		red_out

[hss]
poll_time      : 0:0:5:0
max_reintentos : 3
nodo_local     : 2
nodo_log       : 5
nodo_prim      : 1
nodo_sec       : 2
