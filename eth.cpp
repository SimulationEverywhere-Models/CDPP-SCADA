/*******************************************************************
*
*  DESCRIPTION: Atomic Model Ethernet
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
#include "eth.h"        // class Ethernet
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include "strutil.h"	// str2Int

/** public functions **/

/*******************************************************************
* Function Name: Ethernet
* Description:
********************************************************************/
Ethernet::Ethernet( const string &name )
: Atomic( name )
, in1( addInputPort( "in1" ) )
, in2( addInputPort( "in2" ) )
, in3( addInputPort( "in3" ) )
, inhabilitar( addInputPort( "inhabilitar" ) )
, habilitar( addInputPort( "habilitar" ) )
, out1( addOutputPort( "out1" ) )
, out2( addOutputPort( "out2" ) )
, out3( addOutputPort( "out3" ) )
, delay( 0, 0, 0, 10 )
{
	string time( MainSimulator::Instance().getParameter( description(), "delay" ) ) ;

	if( time != "" )
		delay = time ;
}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Ethernet::initFunction()
{
	string nf1( MainSimulator::Instance().getParameter( description(), "nodo_1" ) ) ;
	string nf2( MainSimulator::Instance().getParameter( description(), "nodo_2" ) ) ;
	string nf3( MainSimulator::Instance().getParameter( description(), "nodo_3" ) ) ;

	nodo_1 = str2Int( nf1 );
	nodo_2 = str2Int( nf2 );
	nodo_3 = str2Int( nf3 );

	hab1 = 1;
	hab2 = 1;
	hab3 = 1;
	hay_pedido = 0;

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &Ethernet::externalFunction( const ExternalMessage &msg )
{
	// si el evento llega por un puerto IN que no esta
	// habilitado, no hago nada...
	if ( ( state() == passive ) &&
	    (( msg.port() == in1 ) && ( hab1 == 1 )) ||
	    (( msg.port() == in2 ) && ( hab2 == 1 )) ||
	    (( msg.port() == in3 ) && ( hab3 == 1 )))
	    {
		pedido = msg.value();
		hay_pedido = 1;

		holdIn (active, delay);
	    }

	if ( ( state() == passive ) &&
	     ( msg.port() == inhabilitar ) )
	{
		cout << msg.time() << " / ETH / Deshabilitando Boca #" << msg.value() << "\n";
		if ( msg.value() == 1 ) hab1 = 0;
		if ( msg.value() == 2 ) hab2 = 0;
		if ( msg.value() == 3 ) hab3 = 0;
		hay_pedido = 0;

		holdIn (active, delay);
	}

	if ( ( state() == passive ) &&
	     ( msg.port() == habilitar ) )
	{
		cout << msg.time() << " / ETH / Habilitando Boca #" << msg.value() << "\n";
		if ( msg.value() == 1 ) hab1 = 1;
		if ( msg.value() == 2 ) hab2 = 1;
		if ( msg.value() == 3 ) hab3 = 1;
		hay_pedido = 0;

		holdIn (active, delay);
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &Ethernet::internalFunction( const InternalMessage &msg )
{
	hay_pedido = 0;
	passivate();
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &Ethernet::outputFunction( const InternalMessage &msg )
{
	if (hay_pedido == 1)
	{
	    int destino = obtener_destino_mensaje_red (pedido);

	    if (( destino == nodo_1 ) && ( hab1 == 1 ))
	         sendOutput( msg.time(), out1, pedido ) ;

	    if (( destino == nodo_2 ) && ( hab2 == 1 ))
	         sendOutput( msg.time(), out2, pedido ) ;

	    if (( destino == nodo_3 ) && ( hab3 == 1 ))
	         sendOutput( msg.time(), out3, pedido ) ;
	}

	return *this ;
}


