[top]
components : driver@Driver
Out : respuesta
In : pedido
In : start
In : stop
Link : pedido pedido@driver
Link : start start@driver
Link : stop stop@driver
Link : respuesta@driver respuesta

[driver]
tiempo_respuesta : 0:0:0:100
