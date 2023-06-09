/*******************************************************************
*
*  DESCRIPTION: Atomic Model Selector
*
*  AUTHOR: Ariel Gonzalez
*
*  EMAIL: mailto://egonzalz@dc.uba.ar
*         mailto://agonzalez@tecnet-ibermatica.com.ar
*
*  DATE: 28/9/2003
*
*******************************************************************/
#include <stdio.h>
/** include files **/
#include "selec.h"      // class Selector
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include "datos.h"

/** public functions **/

/*******************************************************************
* Function Name: Selector
* Description:
********************************************************************/
Selector::Selector( const string &name )
: Atomic( name )
, pdb_in( addInputPort( "pdb_in" ) )
, pant_in( addInputPort( "pant_in" ) )
, red_in( addInputPort( "red_in" ) )
, nombre_logico( addInputPort( "nombre_logico" ) )
, pdb_out( addInputPort( "pdb_out" ) )
, pant_out( addInputPort( "pant_out" ) )
, red_out( addInputPort( "red_out" ) )
, tiempo_respuesta( 0, 0, 0, 10 )
{
	string time( MainSimulator::Instance().getParameter( description(), "tiempo_respuesta" ) ) ;
	if( time != "" ) tiempo_respuesta = time ;

	// Cargo mi nombre fisico, asi como la informacion sobre mi par
	// redundante si es que lo hay
	// Asi puedo saber si un pedido a mi nombre logico lo puedo enviar
	// a la PDB sin pasar por la RED

	string mi_nf   ( MainSimulator::Instance().getParameter( description(), "nodo_local" ) ) ;
	string mi_log  ( MainSimulator::Instance().getParameter( description(), "nodo_log" ) );
	string mi_prim ( MainSimulator::Instance().getParameter( description(), "nodo_prim" ) );
	string mi_sec  ( MainSimulator::Instance().getParameter( description(), "nodo_sec" ) );

	mi_nombre_fisico = str2Int ( mi_nf );
	mi_config = leerConfiguracion (mi_log, mi_prim, mi_sec);

}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Selector::initFunction()
{

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &Selector::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == nombre_logico )
	{
		cambiar_nombre_fisico(
			msg.value(),
			& mi_config,
			1);
//		printf( "\n<< SWITCH : P?=%i, T=%i, NL=%i, NP=%i, NS=%i >>\n",
//			mi_config.usar_primario,
//			mi_config.tipo_nodo,
//			mi_config.nombre_logico,
//			mi_config.nombre_primario,
//			mi_config.nombre_secundario);
		passivate();
	}

	if( msg.port() == pdb_in )
	{
		ultimo_pedido = msg.value();
		estado_selector = hay_pedido_pdb_local;
		holdIn ( active, tiempo_respuesta );
	}

	if( msg.port() == pant_in )
	{
		ultimo_pedido = msg.value();
		estado_selector = hay_pedido_pant;
		holdIn ( active, tiempo_respuesta );
	}

	if( msg.port() == red_in )
	{
		ultimo_pedido = msg.value();
		estado_selector = hay_pedido_red;
		holdIn ( active, tiempo_respuesta );
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &Selector::internalFunction( const InternalMessage & )
{
	if ( estado_selector == hay_pedido_pdb_local )      	    passivate();
	else if ( estado_selector == hay_pedido_pant )  passivate();
	else if ( estado_selector == hay_pedido_red )       passivate();

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &Selector::outputFunction( const InternalMessage &msg )
{
	Value p;

	if ( estado_selector == hay_pedido_pdb_local ) {
		// Los mensajes de la PDB vienen sin el Origen
		// asi que le agrego XY con X=mi_nodo_fisico e
		// Y=MOD_PDB
		p = ultimo_pedido * MAX_NETINT * MAX_NETINT +
		    mi_nombre_fisico * MAX_NETINT +
		    MOD_PDB;
		sendOutput( msg.time(), pant_out, p ) ;
	}
	else if ( estado_selector == hay_pedido_red ) {
		// Los mensajes de la RED ya tienen Origen...
		sendOutput( msg.time(), pant_out, ultimo_pedido ) ;
	}
	else if ( estado_selector == hay_pedido_pant ) {
		// Los mensajes de las PANTALLAS tiene el Destino
		// Si al examinarlo veo que tiene que salir por la RED,
		// lo mando asi como está. Si no, le quito el Destino y
		// lo envio a la PDB
		unsigned long destino = (unsigned long)ultimo_pedido % (MAX_NETINT * MAX_NETINT);
		unsigned long pedido = ((unsigned long)ultimo_pedido - destino) / (MAX_NETINT * MAX_NETINT);
		int mod_destino = destino % MAX_NETINT;
		int nodo_destino = ( destino - mod_destino ) / MAX_NETINT;
		int es_local = 0;

		if ( nodo_destino == mi_nombre_fisico ) es_local=1;
		if ( mi_config.tipo_nodo == NOD_REDUND ) {
			if (( mi_config.nombre_primario == nodo_destino) &&
			    ( mi_config.usar_primario == 1))
			    es_local = 1;
			if (( mi_config.nombre_secundario == nodo_destino) &&
			    ( mi_config.usar_primario == 0))
			    es_local = 1;
			if (( mi_config.nombre_logico == nodo_destino) &&
			    ( mi_config.usar_primario == 1) &&
			    ( mi_config.nombre_primario == mi_nombre_fisico))
			    es_local = 1;
			if (( mi_config.nombre_logico == nodo_destino) &&
			    ( mi_config.usar_primario == 0) &&
			    ( mi_config.nombre_secundario == mi_nombre_fisico))
			    es_local = 1;
		}

		if (es_local == 1)
			sendOutput( msg.time(), pdb_out, pedido ) ;
		else
			sendOutput( msg.time(), red_out, ultimo_pedido ) ;
	}

	return *this ;
}

