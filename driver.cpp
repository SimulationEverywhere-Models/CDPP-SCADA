/*******************************************************************
*
*  DESCRIPTION: Atomic Model Driver
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
#include "driver.h"      // class Driver
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: Driver
* Description:
********************************************************************/
Driver::Driver( const string &name )
: Atomic( name )
, start( addInputPort( "start" ) )
, stop( addInputPort( "stop" ) )
, pedido( addInputPort( "pedido" ) )
, respuesta( addOutputPort( "respuesta" ) )
, tiempo_respuesta( 0, 0, 0, 500 )
{
	string time( MainSimulator::Instance().getParameter( description(), "tiempo_respuesta" ) ) ;

	if( time != "" )
		tiempo_respuesta = time ;
}

/*******************************************************************
* Function Name: initFunction
* Description: 
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Driver::initFunction()
{
	estado_driver = libre;
	pedido_actual.dispositivo = 0 ;
	pedido_actual.direccion = 0 ;
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &Driver::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == start )
	{
		estado_driver = libre ;
		pedido_actual.dispositivo = 0 ;
		pedido_actual.direccion = 0 ;
		if ( state() == active ) passivate () ;
	}

	if( msg.port() == stop )
	{
		estado_driver = inactivo ;
		pedido_actual.dispositivo = 0 ;
		pedido_actual.direccion = 0 ;
		if( state() == active ) passivate() ;
	}

        if( (msg.port() == pedido) && (estado_driver == libre) )
        {
                pedido_actual = dec_pedido_drv( msg.value() ) ;
		estado_driver = ocupado;
                holdIn( active, tiempo_respuesta ) ;
        }

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &Driver::internalFunction( const InternalMessage & )
{
	if ((state() == active) && (estado_driver == ocupado))
	{
		estado_driver = libre ;
                pedido_actual.dispositivo = 0 ;
                pedido_actual.direccion = 0 ;
		passivate () ;
	}
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &Driver::outputFunction( const InternalMessage &msg )
{
	respuesta_drv resp;

	if ((state() == active) && (estado_driver == ocupado))
	{
		resp.dispositivo = pedido_actual.dispositivo ;
		resp.direccion = pedido_actual.direccion;
		resp.valor = AVALUE;
		sendOutput( msg.time(), respuesta, cod_respuesta_drv(resp) ) ;
	}
	return *this ;
}


