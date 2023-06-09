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

#ifndef __DATOS_H
#define __DATOS_H

#include "value.h"
#include "strutil.h"

//**************** GENERAL

#define AVALUE 		2l
#define NOVALUE 	0l
#define NOTAG 		1l
#define PRIM_MASTER	5l
#define PRIM_SLAVE	6l
#define SEC_MASTER	7l
#define SEC_SLAVE	8l

// esto es para codificar la mayoria de los mensajes
#define MAX_INTEGER  100l
#define MAX_SMALL    10l
// esto es para codificar num de nodo, e identificaciones de modulos
// usados en la parte de red
#define MAX_NETINT   10l

//**************** DRIVER

typedef struct {
	int dispositivo; //MAX_SMALL
	int direccion;   //MAX_SMALL
} pedido_drv;

typedef struct {
	int dispositivo; //MAX_SMALL
	int direccion;   //MAX_SMALL
	int valor;       //MAX_SMALL
} respuesta_drv;

Value          cod_pedido_drv( pedido_drv );
pedido_drv     dec_pedido_drv( Value );
Value          cod_respuesta_drv( respuesta_drv );
respuesta_drv  dec_respuesta_drv( Value );

//**************** DATABASE

#define DB_MASTER	0
#define DB_SLAVE	1

typedef struct {
	int tag;
	int dispositivo;
	int direccion;
	int valor;    	  //AVALUE o NOVALUE
} registro_db;

typedef struct {
	int tag;      //MAX_INTEGER
	int escribir; //MAX_SMALL
		      // 0 para leer, !0 para escribir
	int valor;    //MAX_SMALL
	              // solo cuando escribir!=0
} pedido_db;

typedef struct {
	int tag;      //MAX_INTEGER
	int valor;    //MAX_SMALL
} respuesta_db;

Value          cod_pedido_db( pedido_db );
pedido_db      dec_pedido_db( Value );
Value          cod_respuesta_db( respuesta_db );
respuesta_db   dec_respuesta_db( Value );
Value          cod_registro_db( registro_db );
registro_db    dec_registro_db( Value );
int	       obtener_origen_pedido_db ( Value pedido );
Value	       obtener_pedido_db ( Value pedido );
Value	       agregar_origen_pedido_db ( Value p , int origen );

//**************** RED

#define NOD_AUSENTE	0
#define NOD_SIMPLE	1
#define NOD_REDUND	2
#define MAX_NODOS	5

#define MOD_WORKSPACE	1
#define MOD_PDB		2
#define MOD_HS		3

#define PRIMARIO   1
#define	SECUNDARIO 2

typedef struct {
	int tipo_nodo;		// NOD_AUSENTE, NOD_SIMPLE, NOD_REDUND
	int nombre_logico;
	int nombre_primario;
	int nombre_secundario;
	int usar_primario;
} configuracion_nodo;


configuracion_nodo leerConfiguracion (string nodo, string nodo_p, string nodo_s);

int obtener_nombre_fisico (
	int max_nodos,
	configuracion_nodo* configuracion,
	int nodo);
Value preparar_para_envio (
	int mi_nodo,
	int mi_modulo,
	int max_nodos,
	configuracion_nodo* configuracion,
	Value msg);

int obtener_destino (
	Value &msg,
	int mi_nombre_fisico);

void cambiar_nombre_fisico(
	Value msg,
	configuracion_nodo* configuracion,
	int max_nodos );

//**************** PANTALLA

typedef struct {
	int nodo;
	int tag;
	int valor;
} datalink;

Value    procesar_datalink_para_salida ( datalink dl );
datalink procesar_datalink_que_ingresa ( Value v );
void     actualizar_datalink (
		datalink dl,
		datalink *datalinks,
		int cant_datalinks );

//**************** ETHERNET

int obtener_destino_mensaje_red ( Value pedido );

//**************** HSCLIENT

#define TAG_STATUS	15

Value obtener_poleo_hs ( int nodo_hs );
Value obtener_msg_nombre_logico( int nodo_logico, int rol );
int procesar_poleo_hs ( Value poleo );

//**************** HSSERVER

int es_master (int mi_nombre_fisico, configuracion_nodo mi_config );
Value actualizar_tag_status ( int nodo , int valor );

#endif // __DATOS_H
