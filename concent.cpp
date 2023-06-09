/*******************************************************************
*
*  DESCRIPTION: Atomic Model Concentrador
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
#include "concent.h"      // class Concentrador
#include "message.h"      // class ExternalMessage, InternalMessage
#include "mainsimu.h"     // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: Concentrador
* Description:
********************************************************************/
Concentrador::Concentrador( const string &name )
: Atomic( name )
, in1( addInputPort( "in1" ) )
, in2( addInputPort( "in2" ) )
, in( addInputPort( "in" ) )
, out1( addOutputPort( "out1" ) )
, out2( addOutputPort( "out2" ) )
, out( addOutputPort( "out" ) )
, timeout( 0, 0, 1, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "timeout" ) ) ;

	if( time != "" )
		timeout = time ;

}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Concentrador::initFunction()
{
	q.erase( q.begin(), q.end() );
	respuesta_lista = 0;
	respuesta_enviada = 0;
	hay_pedido = 0;

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &Concentrador::externalFunction( const ExternalMessage &msg )
{
	Pedido pedido;
	Time tiempo_cero (0,0,0,0);

	if (state() == active)
		tiempo_restante = timeout - (msg.time() - ultimo_ingreso);
	else {
		ultimo_ingreso = msg.time();
		tiempo_restante = timeout;
	}

	if( (msg.port() == in1) && (q.size() < TAM_COLA) )
	{
		// La entrada 1 es diferente de la 2, en que los mensajes que
		// vienen por ahi, provienen de la red, y tienen la informacion
		// del origen, que se quita, se guarda, y cuando se tiene la
		// respuesta, se vuelve a agregar
		pedido.origen = 1;
		pedido.origen_red = obtener_origen_pedido_db ( msg.value() );
		pedido.msg = obtener_pedido_db ( msg.value() );
		q.push_back( pedido );
		if ( q.size() == 1 ) {
			hay_pedido = 1;
			holdIn( active, tiempo_cero ) ;
		}
		else holdIn( active, tiempo_restante ) ;
	}

	if( (msg.port() == in2) && (q.size() < TAM_COLA) )
	{
		pedido.origen = 2;
		pedido.msg = msg.value();
		q.push_back( pedido );
		if ( q.size() == 1 ) {
			hay_pedido = 1;
			holdIn( active, tiempo_cero ) ;
		}
		else holdIn( active, tiempo_restante ) ;
	}

	if( msg.port() == in )
	{
		if ( state() == passive )
			passivate ();
	 	else {
			ultimo_ingreso = msg.time();
			respuesta_lista = 1;
			respuesta_enviada = 0;
			respuesta = msg.value();
			holdIn ( active , tiempo_cero );
		}
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &Concentrador::internalFunction( const InternalMessage & )
{
	Time tiempo_cero (0,0,0,0);

	if ( (respuesta_lista == 1) && (respuesta_enviada == 0) )
	{
		q.pop_front();
		respuesta_enviada = 1;
		holdIn ( active, tiempo_cero );
	}
	else if ( (respuesta_lista == 1) && (respuesta_enviada == 1) )
	{
		// llego una respuesta por IN
		// si no hay mas pedidos en la cola, se pasiva
		respuesta_lista = 0;
		respuesta_enviada = 0;
		if ( q.size() == 0 )
			passivate () ;
		else
			holdIn( active, timeout );
	}
	else if (hay_pedido == 1)
	{
		hay_pedido = 0;
		holdIn (active, tiempo_restante);
	}
	else
	{
		q.pop_front();
		if ( q.size() == 0 )
			passivate () ;
		else {
			hay_pedido = 1;
			tiempo_restante = timeout;
			holdIn( active, tiempo_cero );
		}
	}

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &Concentrador::outputFunction( const InternalMessage &msg )
{
	Pedido p; 

	if ( (respuesta_lista == 1) && (respuesta_enviada == 1) )
	{ if (q.size() != 0) {
		p = q.front();
		sendOutput( msg.time(), out, p.msg ) ;
	} }
	else if ((respuesta_lista == 1) && (respuesta_enviada == 0))
	{
		p = q.front();
		if (p.origen == 1) {
			// En este caso, tengo que reagregar la informacion
			// del origen de red, que ahora pasa a ser el destino
			Value msg_respuesta =
				agregar_origen_pedido_db(
					respuesta,
					p.origen_red
				);

			sendOutput( msg.time(), out1, msg_respuesta ) ;
		}
		else if (p.origen == 2)
			sendOutput( msg.time(), out2, respuesta ) ;
	}
	else if ( hay_pedido == 1) {
		p = q.front();
		sendOutput( msg.time(), out, p.msg ) ;
	}

	return *this ;
}


