[top]

components : hsc@HSClient
In   : start
In   : stop
In   : red_in
Out  : red_out
Out  : nombre_logico
Link : start			start@hsc
Link : stop			stop@hsc
Link : red_in			red_in@hsc
Link : red_out@hsc		red_out
Link : nombre_logico@hsc	nombre_logico

[hsc]
poll_time    : 0:0:5:0
nodo_hs	     : 5
nodo_hs_prim : 1
nodo_hs_sec  : 2
