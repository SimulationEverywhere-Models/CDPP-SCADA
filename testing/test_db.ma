[top]
components : db@Database 
Out : registro_pedido
Out : drv_pedido
In : recargar
In : leer
In : drv_respuesta
Link : recargar            recargar@db
Link : leer                leer@db
Link : drv_respuesta       drv_respuesta@db
Link : registro_pedido@db  registro_pedido
Link : drv_pedido@db       drv_pedido

[db]
tiempo_respuesta : 0:0:0:100
tiempo_recarga : 0:0:10:0
tiempo_scan : 0:0:0:200
nodo_local : 1
archivo_inicial : DB_MASTER

