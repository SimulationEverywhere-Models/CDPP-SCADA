/*******************************************************************
*
*  DESCRIPTION: Atomic Model Red
*
*  AUTHOR: Ariel Gonzalez
*
*  EMAIL: mailto://egonzalz@dc.uba.ar
*         mailto://agonzalez@tecnet-ibermatica.com.ar
*
*  DATE: 28/9/2003
*
*******************************************************************/

/** include files **/
#include "red.h"        // class Red
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include "datos.h"

/** public functions **/

/*******************************************************************
* Function Name: Red
* Description:
********************************************************************/
Red::Red( const string &name )
: Atomic( name )
, pdb_in( addInputPort( "pdb_in" ) )
, ws_in( addInputPort( "ws_in" ) )
, aux_in( addInputPort( "aux_in" ) )
, eth_in( addInputPort( "eth_in" ) )
, nombre_logico( addInputPort( "nombre_logico" ) )
, pdb_out( addInputPort( "pdb_out" ) )
, ws_out( addInputPort( "ws_out" ) )
, aux_out( addInputPort( "aux_out" ) )
, eth_out( addInputPort( "eth_out" ) )
, tiempo_respuesta( 0, 0, 0, 10 )
{
	string time( MainSimulator::Instance().getParameter( description(), "tiempo_respuesta" ) ) ;
	if( time != "" ) tiempo_respuesta = time ;

	string mi_nf( MainSimulator::Instance().getParameter( description(), "nodo_local" ) ) ;
	mi_nombre_fisico = str2Int ( mi_nf );

	//Grrrrrrrrrrr....
	configuracion = new configuracion_nodo[MAX_NODOS];

	// Ahora leo la configuración de la red.

	string nodo1   ( MainSimulator::Instance().getParameter( description(), "nodo_1" ) );
	string nodo1_p ( MainSimulator::Instance().getParameter( description(), "nodo_1_prim" ) );
	string nodo1_s ( MainSimulator::Instance().getParameter( description(), "nodo_1_sec" ) );
	string nodo2   ( MainSimulator::Instance().getParameter( description(), "nodo_2" ) );
	string nodo2_p ( MainSimulator::Instance().getParameter( description(), "nodo_2_prim" ) );
	string nodo2_s ( MainSimulator::Instance().getParameter( description(), "nodo_2_sec" ) );
	string nodo3   ( MainSimulator::Instance().getParameter( description(), "nodo_3" ) );
	string nodo3_p ( MainSimulator::Instance().getParameter( description(), "nodo_3_prim" ) );
	string nodo3_s ( MainSimulator::Instance().getParameter( description(), "nodo_3_sec" ) );
	string nodo4   ( MainSimulator::Instance().getParameter( description(), "nodo_4" ) );
	string nodo4_p ( MainSimulator::Instance().getParameter( description(), "nodo_4_prim" ) );
	string nodo4_s ( MainSimulator::Instance().getParameter( description(), "nodo_4_sec" ) );
	string nodo5   ( MainSimulator::Instance().getParameter( description(), "nodo_5" ) );
	string nodo5_p ( MainSimulator::Instance().getParameter( description(), "nodo_5_prim" ) );
	string nodo5_s ( MainSimulator::Instance().getParameter( description(), "nodo_5_sec" ) );

	configuracion[0] = leerConfiguracion (nodo1, nodo1_p, nodo1_s);
	configuracion[1] = leerConfiguracion (nodo2, nodo2_p, nodo2_s);
	configuracion[2] = leerConfiguracion (nodo3, nodo3_p, nodo3_s);
	configuracion[3] = leerConfiguracion (nodo4, nodo4_p, nodo4_s);
	configuracion[4] = leerConfiguracion (nodo5, nodo5_p, nodo5_s);

}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Red::initFunction()
{

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &Red::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == nombre_logico )
	{
		cambiar_nombre_fisico(
			msg.value(),
			configuracion,
			MAX_NODOS);
		passivate();
	}

	if( msg.port() == aux_in )
	{
		ultimo_pedido = msg.value();
		estado_red = hay_pedido_aux;
		holdIn ( active, tiempo_respuesta );
	}

	if( msg.port() == pdb_in )
	{
		ultimo_pedido = msg.value();
		estado_red = hay_pedido_pdb;
		holdIn ( active, tiempo_respuesta );
	}

	if( msg.port() == ws_in )
	{
		ultimo_pedido = msg.value();
		estado_red = hay_pedido_ws;
		holdIn ( active, tiempo_respuesta );
	}

	if( msg.port() == eth_in )
	{
		ultimo_pedido = msg.value();
		estado_red = hay_pedido_eth;
		holdIn ( active, tiempo_respuesta );
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &Red::internalFunction( const InternalMessage & )
{
	if ( estado_red == hay_pedido_pdb )      passivate();
	else if ( estado_red == hay_pedido_aux )  passivate();
	else if ( estado_red == hay_pedido_ws )  passivate();
	else if ( estado_red == hay_pedido_eth ) passivate();

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &Red::outputFunction( const InternalMessage &msg )
{
	Value p, tmp;
	int dest;

	if ( estado_red == hay_pedido_pdb ) {
		p = preparar_para_envio (
			mi_nombre_fisico,
			MOD_PDB,
			MAX_NODOS,
			configuracion,
			ultimo_pedido );
		if (p != 0)
			sendOutput( msg.time(), eth_out, p ) ;
	}
	else if ( estado_red == hay_pedido_ws ) {
		p = preparar_para_envio (
			mi_nombre_fisico,
			MOD_WORKSPACE,
			MAX_NODOS,
			configuracion,
			ultimo_pedido );
		if (p != 0)
			sendOutput( msg.time(), eth_out, p ) ;
	}
	else if ( estado_red == hay_pedido_aux ) {
		p = preparar_para_envio (
			mi_nombre_fisico,
			MOD_HS,
			MAX_NODOS,
			configuracion,
			ultimo_pedido );
		if (p != 0) {
			tmp = p;
			dest = obtener_destino (
				tmp,
				mi_nombre_fisico );
			// si vino por aux, y es para este nodo, seguro
			// es para la pdb local...
			if ( dest == mi_nombre_fisico )
			     sendOutput( msg.time(), pdb_out, tmp ) ;
			else sendOutput( msg.time(), eth_out, p ) ;
		}
	}
	else if ( estado_red == hay_pedido_eth ) {
		p = ultimo_pedido;
		// ojo!! p se modifica por efecto colateral
		dest = obtener_destino (
			p,
			mi_nombre_fisico );
		if (dest == MOD_WORKSPACE) sendOutput( msg.time(), ws_out, p ) ;
		if (dest == MOD_PDB) sendOutput( msg.time(), pdb_out, p ) ;
		if (dest == MOD_HS) sendOutput( msg.time(), aux_out, p ) ;
	}

	return *this ;
}

