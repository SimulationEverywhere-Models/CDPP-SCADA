/*******************************************************************
*
*  DESCRIPTION: Atomic Model Pantalla
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
#include "pant.h"       // class Pantalla
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: Pantalla
* Description:
********************************************************************/
Pantalla::Pantalla( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )
, out( addOutputPort( "out" ) )
, refresh_rate( 0, 0, 0, 500 )
{
	string time( MainSimulator::Instance().getParameter( description(), "refresh_rate" ) ) ;

	if( time != "" )
		refresh_rate = time ;



	datalinks = new datalink [MAX_DATALINKS];

	cant_datalinks = 4;
	datalinks[0].nodo = 5; datalinks[0].tag = 15;
	datalinks[1].nodo = 1; datalinks[1].tag = 80;
	datalinks[2].nodo = 2; datalinks[2].tag = 81;
	datalinks[3].nodo = 3; datalinks[3].tag = 82;
}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Pantalla::initFunction()
{
	cur_datalink = 0;
	holdIn (active, refresh_rate);
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &Pantalla::externalFunction( const ExternalMessage &msg )
{
	// atiendo la llegada de un mensaje de la pdb
	datalink dl = procesar_datalink_que_ingresa ( msg.value() );
	actualizar_datalink ( dl, datalinks, cant_datalinks );

	//holdIn (active, refresh_rate - (msg.time() - lastChange()));
	holdIn (active, refresh_rate);

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &Pantalla::internalFunction( const InternalMessage &msg )
{
	cur_datalink ++ ;
	if ( cur_datalink == cant_datalinks ) cur_datalink = 0;
	//ultimo_scan = msg.time();
	holdIn (active, refresh_rate);

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &Pantalla::outputFunction( const InternalMessage &msg )
{
	// envio otro pedido a la pdb
	Value pedido = procesar_datalink_para_salida ( datalinks [cur_datalink] );
	if ( cur_datalink < cant_datalinks )
		sendOutput( msg.time(), out, pedido );

	return *this ;
}


