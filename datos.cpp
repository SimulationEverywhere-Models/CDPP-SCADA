/*******************************************************************
*
*  DESCRIPTION: Conversion de los Datos para el modelo Driver
*
*  AUTHOR: Ariel Gonzalez
*
*  EMAIL: mailto://egonzalz@dc.uba.ar
*         mailto://agonzalez@tecnet-ibermatica.com.ar
*
*  DATE: 26/9/2003
*
*******************************************************************/

//////////////////////////////////////////////////////////////////
/////////                         ////////////////////////////////
////      TAMBIEN AGREGAR EL ORIGEN O DESTINO    /////////////////
////////          A LOS PED/RESP ENTRE LA PDB Y LA RED   /////////
//////////////////////////////////////////////////////////////////

#include <stdio.h>

#include <math.h>
#include "datos.h"
#include "value.h"

//**************** DRIVER

Value          cod_pedido_drv( pedido_drv p )
{
	return (Value) (p.dispositivo * MAX_INTEGER + p.direccion);
}

pedido_drv     dec_pedido_drv( Value v )
{
	pedido_drv p;
	p.direccion = (long)v % MAX_INTEGER;
	p.dispositivo = (((long)v - p.direccion)
	                 / MAX_INTEGER)
			 % MAX_INTEGER;

	return p;
}

Value          cod_respuesta_drv( respuesta_drv r )
{
	return (Value) ( r.dispositivo * MAX_INTEGER * MAX_INTEGER
	               + r.direccion * MAX_INTEGER
		       + r.valor );
}

respuesta_drv  dec_respuesta_drv( Value v )
{
	respuesta_drv r;
	r.valor = (long)v % MAX_INTEGER;
	r.direccion = (((long) v - r.valor) / MAX_INTEGER) % MAX_INTEGER;
	r.dispositivo = (((long)v - r.direccion * MAX_INTEGER - r.valor)
	                 / (MAX_INTEGER * MAX_INTEGER))
		         % MAX_INTEGER;
	return r;
}

/*      DRIVER
main()
{
	respuesta_drv r;
	pedido_drv p;
	Value vr;
	Value vp;

	r.dispositivo = 1;
	r.direccion = 2;
	r.valor = 3;
	vr = cod_respuesta_drv(r);

	p.dispositivo = 4;
	p.direccion = 5;
	vp = cod_pedido_drv(p);

	printf( "r=%e \tp=%e\n", vr, vp);

	p = dec_pedido_drv(vp);
	r = dec_respuesta_drv(vr);

	printf( "r=<%i, %i, %i> \tp=<%i, %i>\n",
			r.dispositivo, r.direccion, r.valor,
			p.dispositivo, p.direccion );
}
*/

//**************** DATABASE

Value          cod_pedido_db( pedido_db p )
{
	return (Value) ( p.tag * MAX_SMALL * MAX_SMALL
	               + p.escribir * MAX_SMALL
		       + p.valor );
}

pedido_db      dec_pedido_db( Value v )
{
	pedido_db p;
	p.valor = (long)v % MAX_SMALL;
	p.escribir = (((long) v - p.valor) / MAX_SMALL) % MAX_SMALL;
	p.tag = (((long)v - p.escribir * MAX_SMALL - p.valor)
	         / (MAX_SMALL * MAX_SMALL))
		 % MAX_INTEGER;
	return p;
}

Value          cod_respuesta_db( respuesta_db r )
{
	return (Value) ( r.tag * MAX_SMALL
		       + r.valor );
}

respuesta_db   dec_respuesta_db( Value v )
{
	respuesta_db r;
	r.valor = (long)v % MAX_SMALL;
	r.tag = (((long)v - r.valor)
	         / MAX_SMALL)
		 % MAX_INTEGER;

	return r;
}

Value          cod_registro_db( registro_db r )
{
	return (Value) ( r.tag * MAX_SMALL * MAX_SMALL * MAX_SMALL
	               + r.dispositivo * MAX_SMALL * MAX_SMALL
	               + r.direccion * MAX_SMALL
		       + r.valor );
}

registro_db    dec_registro_db( Value v )
{
	long tmp = (long) v;
	registro_db r;

	r.valor = tmp % MAX_SMALL;
	tmp = ( tmp - r.valor ) / MAX_SMALL;

	r.direccion = tmp % MAX_SMALL;
	tmp = ( tmp - r.direccion ) / MAX_SMALL;

	r.dispositivo = tmp % MAX_SMALL;
	tmp = ( tmp - r.dispositivo ) / MAX_SMALL;

	r.tag = tmp % MAX_INTEGER;

	return r;
}

int	obtener_origen_pedido_db ( Value pedido )
{
	unsigned long p = (unsigned long) pedido;

	return p % (MAX_NETINT * MAX_NETINT);
}

Value	obtener_pedido_db ( Value pedido )
{
	unsigned long p = (unsigned long) pedido;

	return (Value)(p / (MAX_NETINT * MAX_NETINT));
}

Value 	agregar_origen_pedido_db ( Value p , int origen )
{
	return p * MAX_NETINT * MAX_NETINT + origen;
}

//**************** RED

configuracion_nodo leerConfiguracion (string nodo, string nodo_p, string nodo_s)
{
	configuracion_nodo c;

	c.usar_primario = 1;
	c.nombre_primario = 0;
	c.nombre_secundario = 0;

	if (nodo == "none") c.tipo_nodo = NOD_AUSENTE;
	else {
		c.nombre_logico = str2Int(nodo);

		if ((nodo_p != "none") && (nodo_s != "none")) {
			c.tipo_nodo = NOD_REDUND;
			c.nombre_primario = str2Int(nodo_p);
			c.nombre_secundario = str2Int(nodo_s);
		} else
			c.tipo_nodo = NOD_SIMPLE;
	}

	//printf ("NODO %i, %i, %i, %i\n",
	//	c.tipo_nodo,
	//	c.nombre_logico,
	//	c.nombre_primario,
	//	c.nombre_secundario);
	return c;
}

int obtener_nombre_fisico (
	int max_nodos,
	configuracion_nodo* configuracion,
	int nodo)
{
	configuracion_nodo c;
	for (int i = 0; i < max_nodos; i++) {
		c = configuracion[i];

		// los tres casos donde el nombre es un nombre fisico...
		if (
		  ((c.tipo_nodo == NOD_SIMPLE) && (c.nombre_logico == nodo))
		  ||
                  ((c.tipo_nodo == NOD_REDUND) && (c.nombre_primario == nodo))
		  ||
		  ((c.tipo_nodo == NOD_REDUND) && (c.nombre_secundario == nodo)))

			return nodo;

		// si efectivamente el nombre es logico, lo resuelvo...
		if ((c.tipo_nodo == NOD_REDUND) && (c.nombre_logico == nodo))
		{
			if (c.usar_primario == 1)
				return c.nombre_primario;
			else
				return c.nombre_secundario;
		}
	}

	return 0;
}

Value preparar_para_envio (
	int mi_nodo,
	int mi_modulo,
	int max_nodos,
	configuracion_nodo* configuracion,
	Value msg)
// esta funcion quita los dos primeros digitos de msg XY que se asumen
// que representan el destino del mensaje (X=nodo fisico o logico e Y indica
// si viene de la PDB o del WS), y le vuelve a agregar esa informacion
// pero con X' que corresponde al nodo fisico. Ademas, le agrega la info
// del origen del mensaje (mi_nodo, mi_modulo)
//
// si llega MXY, sale MX'YX''Y''
{
	Value mensaje_original ;
	mensaje_original = ((unsigned long)msg) / (MAX_NETINT*MAX_NETINT);
	unsigned long destino_log = ((unsigned long)msg) % (MAX_NETINT*MAX_NETINT);
	//Value x = modf ( msg / (Value)(MAX_NETINT*MAX_NETINT), &mensaje_original );
	//long destino_log = (long)(x * MAX_NETINT * MAX_NETINT);
	unsigned long modulo_destino = destino_log % MAX_NETINT ;
	unsigned long nodolog_destino = ( destino_log - modulo_destino ) / MAX_NETINT ;
	unsigned long nodofis_destino = obtener_nombre_fisico (
					max_nodos,
					configuracion,
					nodolog_destino
					);

	if (nodofis_destino == 0) return 0; // intento enviar a nodo que no existe

	unsigned long destino_fis = nodofis_destino * MAX_NETINT + modulo_destino;
	unsigned long origen_fis = mi_nodo * MAX_NETINT + mi_modulo;

	Value mensaje_listo = mensaje_original * MAX_NETINT * MAX_NETINT + destino_fis;
	mensaje_listo = mensaje_listo * MAX_NETINT * MAX_NETINT + origen_fis;

	return mensaje_listo;
}

// esta funcion devuelve el destino del msg, asumiendo que este viene
// con el formato MXYX'Y' con XY es el destino
// ademas le paso mi_nombre_fisico, para que verifique que este mensaje es
// para mi
// devuelve 0 si no lo es, si no MOD_WORKSPACE o MOD_PDB
// ADEMAS!! msg lo devuelve de la forma MX'Y'
int obtener_destino (Value &msg, int mi_nombre_fisico)
{
	unsigned long m = (unsigned long) msg;
	// m = MMMMMDDOO   (DD destino, OO origen)
	unsigned long origen = m % ( MAX_NETINT * MAX_NETINT );
	m /= ( MAX_NETINT * MAX_NETINT );
	// m = MMMMMDD
	msg = m - (m % (MAX_NETINT * MAX_NETINT)) + origen;

	m %= ( MAX_NETINT * MAX_NETINT );
	// m = DD
	int modulo_destino = ( m % MAX_NETINT ) ;
	int nodo_destino = ( m - modulo_destino ) / MAX_NETINT ;

	if ( nodo_destino != mi_nombre_fisico ) return 0;
	else if ( modulo_destino == MOD_WORKSPACE ) return MOD_WORKSPACE;
	else if ( modulo_destino == MOD_PDB ) return MOD_PDB;
	else if ( modulo_destino == MOD_HS ) return MOD_HS;
	else return 0;
}

void cambiar_nombre_fisico(
	Value msg,
	configuracion_nodo* configuracion,
	int max_nodos)
{
	int rol = (long)msg % MAX_NETINT;
	int nodo = (((long)msg - rol) / MAX_NETINT) % MAX_NETINT;
	configuracion_nodo c;
	for (int i = 0; i < max_nodos; i++) {
		c = configuracion[i];

		if ((c.nombre_logico == nodo) && (c.tipo_nodo == NOD_REDUND)) {
			if (rol == PRIMARIO)
				configuracion[i].usar_primario = 1;
			else if (rol == SECUNDARIO)
				configuracion[i].usar_primario = 0;
		}
	}
}
/*
main()
{
	configuracion_nodo conf[3];

	conf[0].tipo_nodo = NOD_SIMPLE;
	conf[0].nombre_logico = 1;
	conf[0].nombre_primario = 0;
	conf[0].nombre_secundario = 0;
	conf[0].usar_primario = 0;

	conf[1].tipo_nodo = NOD_REDUND;
	conf[1].nombre_logico = 2;
	conf[1].nombre_primario = 5;
	conf[1].nombre_secundario = 6;
	conf[1].usar_primario = 1;

	conf[2].tipo_nodo = NOD_SIMPLE;
	conf[2].nombre_logico = 3;
	conf[2].nombre_primario = 0;
	conf[2].nombre_secundario = 0;
	conf[2].usar_primario = 0;

	int un_nl = 1;
	int un_nf = obtener_nombre_fisico (3, conf, un_nl);
	printf ( "NL = %i  \t->  NF = %i\n", un_nl, un_nf );

	un_nl = 2;
	un_nf = obtener_nombre_fisico (3, conf, un_nl);
	printf ( "NL = %i  \t->  NF = %i\n", un_nl, un_nf );

	un_nl = 6;
	un_nf = obtener_nombre_fisico (3, conf, un_nl);
	printf ( "NL = %i  \t->  NF = %i\n\n\n", un_nl, un_nf );

	int mi_nodo = 2;
	int mi_modulo = 1;
	Value msg = 41999922;
	Value msg_prep =
		preparar_para_envio ( mi_nodo, mi_modulo, 3, conf, msg );

	printf ( "MSG=%12.f\tNODO ORIG=%i\tMOD ORIG=%i\tenvia...\tMSG'=%12.f\n\n\n",
		msg,
		mi_nodo, mi_modulo,
		msg_prep );

	Value msg2 = msg_prep;
	mi_nodo = 5;
	int mod_destino = obtener_destino(msg_prep, mi_nodo);
	printf ( "NODO %i recibe...\tMSG=%12.f\tMOD DEST=%i\tMSG PASADO=%12.f\n\n\n",
		mi_nodo,
		msg2,
		mod_destino,
		msg_prep );

} */

Value procesar_datalink_para_salida ( datalink dl )
{
	Value v;

	// al datalink le tengo que agregar el origen
	// que se forma de dl.nodo y MOD_PDB
	pedido_db p;
	p.tag = dl.tag;
	p.escribir = 0;
	p.valor = NOVALUE;
	v = cod_pedido_db (p);

	v = v * MAX_NETINT * MAX_NETINT +
	    dl.nodo * MAX_NETINT +
	    MOD_PDB ;

	//printf ("<< %10.0f >>\n",v);
	return v;
}

datalink procesar_datalink_que_ingresa ( Value v )
{
	datalink dl;

	// v es un mensaje que contiene la respuesta de
	// la db y el destino

	respuesta_db r;
	unsigned long origen;
	unsigned long msg = (unsigned long) v;
	origen = msg % ( MAX_NETINT * MAX_NETINT );
	msg /= ( MAX_NETINT * MAX_NETINT );
	r = dec_respuesta_db ((Value)msg);

	dl.nodo = origen / MAX_NETINT;
	dl.tag = r.tag;
	dl.valor = r.valor;

	return dl;
}

void actualizar_datalink (
	datalink dl,
	datalink *datalinks,
	int cant_datalinks )
{
	for (int i=0; i < cant_datalinks; i++)
		if (( dl.tag == datalinks[i].tag ) &&
		    ( dl.nodo == datalinks[i].nodo )) {

		    	//printf ("GOT IT !!\n");
		    	datalinks[i].valor = dl.valor;

		}
}

// asume que el pedido tiene la forma MMMMMABCD donde A y C son los
// nodos (fisicos) de destino y origen, resp. Esta función devuelve A.
int obtener_destino_mensaje_red ( Value pedido )
{
	unsigned long msg = (unsigned long) pedido;
	msg /= MAX_NETINT;  // quita D
	msg /= MAX_NETINT;  // quita C
	msg /= MAX_NETINT;  // quita B
	return msg % MAX_NETINT;  // se queda con A
}

// esta funcion crea un pedido para enviar a la red, con destino
// al nodo logico del par HS y el modulo PDB
Value obtener_poleo_hs ( int nodo_hs )
{
	Value v;

	pedido_db p;
	p.tag = TAG_STATUS;
	p.escribir = 0;
	p.valor = NOVALUE;
	v = cod_pedido_db (p);

	v = v * MAX_NETINT * MAX_NETINT +
	    nodo_hs * MAX_NETINT +
	    MOD_PDB ;

	//printf ("<< %10.0f >>\n",v);
	return v;
}

// esta funcion crea un pedido para enviar a la red, con destino
// al nodo fisico y el modulo PDB y escribir el valor al tag status
Value actualizar_tag_status ( int nodo , int valor )
{
	Value v;

	pedido_db p;
	p.tag = TAG_STATUS;
	p.escribir = 1;
	p.valor = valor;
	v = cod_pedido_db (p);

	v = v * MAX_NETINT * MAX_NETINT +
	    nodo * MAX_NETINT +
	    MOD_PDB ;

	//printf ("<< %10.0f >>\n",v);
	return v;
}

// esta funcion examina un poleo que llegó, extrae la respuesta_db
// y devuelve el valor (deberia ser PRIM_MASTER o SEC_MASTER)
int procesar_poleo_hs ( Value poleo )
{
	unsigned long v = (unsigned long) poleo;

	// quito el destino
	v /= ( MAX_NETINT * MAX_NETINT );

	respuesta_db r = dec_respuesta_db (v);

	return r.valor;
}

// arma un mensaje XY donde X es el nodo logico, e Y es PRIMARIO/SECUNDARIO
// indicando que el nombre logico X se resuelve por el nombre del nodo
// primario o secundario respec.
Value obtener_msg_nombre_logico( int nodo_logico, int rol )
{
	return nodo_logico * MAX_NETINT + rol;
}

int es_master (int mi_nombre_fisico, configuracion_nodo mi_config )
{
	if ( ( mi_config.usar_primario == 1) &&
	     ( mi_config.nombre_primario == mi_nombre_fisico ) )
	     	return -1;

	if ( ( mi_config.usar_primario == 0) &&
	     ( mi_config.nombre_secundario == mi_nombre_fisico ) )
	     	return -1;

	return 0;
}
