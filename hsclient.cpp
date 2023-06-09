/*******************************************************************
*
*  DESCRIPTION: Atomic Model HSClient
*
*  AUTHOR: Ariel Gonzalez
*
*  EMAIL: mailto://egonzalz@dc.uba.ar
*         mailto://agonzalez@tecnet-ibermatica.com.ar
*
*  DATE: 26/9/2003
*
*******************************************************************/

/** include files **/
#include "hsclient.h"   // class HSClient
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include "strutil.h"    // str2Int( ... )

#define MAX_POLEOS	5

/** public functions **/

/*******************************************************************
* Function Name: HSClient
* Description:
********************************************************************/
HSClient::HSClient( const string &name )
: Atomic( name )
, start( addInputPort( "start" ) )
, stop( addInputPort( "stop" ) )
, red_in( addInputPort( "red_in" ) )
, nombre_logico( addOutputPort( "nombre_logico" ) )
, red_out( addOutputPort( "red_out" ) )
, poll_time( 0, 0, 5, 0 )
{
	string ptime( MainSimulator::Instance().getParameter( description(), "poll_time" ) ) ;
	string nodo( MainSimulator::Instance().getParameter( description(), "nodo_hs" ) ) ;
	string nodop( MainSimulator::Instance().getParameter( description(), "nodo_hs_prim" ) ) ;
	string nodos( MainSimulator::Instance().getParameter( description(), "nodo_hs_sec" ) ) ;

	if( ptime != "" ) poll_time = ptime ;
	nodo_hs = str2Int (nodo);
	nodo_hs_prim = str2Int (nodop);
	nodo_hs_sec = str2Int (nodos);
}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &HSClient::initFunction()
{
	estado_hsclient = hsc_inactivo;
	ultima_respuesta = 0;
	polear_primario = 1;
	cant_poleos_sin_respuesta = 0;
	//holdIn (active, poll_time);

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &HSClient::externalFunction( const ExternalMessage &msg )
{
	Time tiempo_cero (0,0,0,0);

	if (( state() == passive ) && ( msg.port() == start ))
	{
		estado_hsclient = hsc_poleando;
		ultima_respuesta = 0;
		holdIn (active, tiempo_cero);
	}

	if (( state() == active ) && ( msg.port() == stop )) {
		cout << msg.time() << " / CLIENTE / HOT-STANDBY DETENIDO.\n";
		passivate ();
	}

	if (( state() == active ) && ( msg.port() == red_in ))
	{
		ultima_respuesta = msg.value();
		estado_hsclient = hsc_recibiendo;
		holdIn (active, tiempo_cero);
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &HSClient::internalFunction( const InternalMessage & msg)
{
	Time tiempo_cero (0,0,0,0);

	if (( state() == active) && ( estado_hsclient == hsc_recibiendo )) {
		cant_poleos_sin_respuesta = 0;
		estado_hsclient = hsc_inactivo;
		holdIn (active, tiempo_cero);
	}

	if (( state() == active) && ( estado_hsclient == hsc_poleando )) {
		estado_hsclient = hsc_inactivo;
		if (cant_poleos_sin_respuesta < (MAX_POLEOS-1))
			cant_poleos_sin_respuesta ++ ;
		else {
			polear_primario = 1 - polear_primario;
			cant_poleos_sin_respuesta = 0;
			if (polear_primario==1)
				cout << msg.time() << " / CLIENTE / CAMBIANDO EL DESTINO DE POLEO A NODO PRIMARIO\n";
			else
				cout << msg.time() << " / CLIENTE / CAMBIANDO EL DESTINO DE POLEO A NODO SECUNDARIO\n";
		}

		holdIn (active, tiempo_cero);
	}

	if (( state() == active) && ( estado_hsclient == hsc_inactivo )) {
		estado_hsclient = hsc_poleando;
		holdIn (active, poll_time);
	}

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &HSClient::outputFunction( const InternalMessage &msg )
{
	Value poleo, nlog;
	int rol;
	int status_nodo_hs;

	if (( state() == active) && ( estado_hsclient == hsc_poleando )) {
		// envia un poleo
		if ( polear_primario == 1 )
		     poleo = obtener_poleo_hs ( nodo_hs_prim );
		else poleo = obtener_poleo_hs ( nodo_hs_sec );

		sendOutput( msg.time(), red_out, poleo ) ;
	}

	if (( state() == active) && ( estado_hsclient == hsc_recibiendo )) {
		// envia un nombre_logico
		status_nodo_hs = procesar_poleo_hs ( ultima_respuesta );
		cout << msg.time() << " / CLIENTE / RECIBE RESPUESTA CON ESTADO ";
		if ( status_nodo_hs == PRIM_MASTER) cout << "PRIM_MASTER\n";
		else if ( status_nodo_hs == PRIM_SLAVE) cout << "PRIM_SLAVE\n";
		else if ( status_nodo_hs == SEC_MASTER) cout << "SEC_MASTER\n";
		else if ( status_nodo_hs == SEC_SLAVE) cout << "SEC_SLAVE\n";
		else cout << "?????\n";

		if (( status_nodo_hs == PRIM_MASTER) ||
		    ( status_nodo_hs == SEC_SLAVE))
		{
			cout << msg.time() << " / CLIENTE /    Remapeando Nombre Lógico a Primario...\n";
			rol = PRIMARIO;
			nlog = obtener_msg_nombre_logico( nodo_hs, rol );
			sendOutput( msg.time(), nombre_logico, nlog ) ;
		}
		else if (( status_nodo_hs == SEC_MASTER) ||
		         ( status_nodo_hs == PRIM_SLAVE))
		{
			cout << msg.time() << " / CLIENTE /    Remapeando Nombre Lógico a Secundario...\n";
			rol = SECUNDARIO;
			nlog = obtener_msg_nombre_logico( nodo_hs, rol );
			sendOutput( msg.time(), nombre_logico, nlog ) ;
		}
	}

	return *this ;
}


