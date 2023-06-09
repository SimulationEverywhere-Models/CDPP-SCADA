/*******************************************************************
*
*  DESCRIPTION: Atomic Model Database
*
*  AUTHOR: Ariel Gonzalez
*
*  EMAIL: mailto://egonzalz@dc.uba.ar
*         mailto://agonzalez@tecnet-ibermatica.com.ar
*
*  DATE: 26/9/2003
*
*******************************************************************/

#include <stdio.h>

/** include files **/
#include "database.h"      // class Database
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include "strutil.h"

/** public functions **/

/*******************************************************************
* Function Name: Database
* Description:
********************************************************************/
Database::Database( const string &name )
: Atomic( name )
, recargar( addInputPort( "recargar" ) )
, leer( addInputPort( "leer" ) )
, drv_respuesta( addInputPort( "drv_respuesta" ) )
, registro_pedido( addOutputPort( "registro_pedido" ) )
, drv_pedido( addOutputPort( "drv_pedido" ) )
, tiempo_respuesta( 0, 0, 0, 100 )
, tiempo_recarga( 0, 0, 5, 0 )
, tiempo_scan( 0, 0, 0, 500 )
{
	string time1( MainSimulator::Instance().getParameter( description(), "tiempo_respuesta" ) ) ;
	string time2( MainSimulator::Instance().getParameter( description(), "tiempo_recarga" ) ) ;
	string time3( MainSimulator::Instance().getParameter( description(), "tiempo_scan" ) ) ;
	string arini( MainSimulator::Instance().getParameter( description(), "archivo_inicial" ) ) ;
	string nl( MainSimulator::Instance().getParameter( description(), "nodo_local" ) ) ;

	nodo_local = str2Int (nl);

	if( time1 != "" ) tiempo_respuesta = time1 ;
	if( time2 != "" ) tiempo_recarga = time2;
	if( time3 != "" ) tiempo_scan = time3;

	archivo_inicial = DB_MASTER;
	if( arini == "DB_MASTER" ) archivo_inicial = DB_MASTER;
	if( arini == "DB_SLAVE" ) archivo_inicial = DB_SLAVE;
}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Database::initFunction()
{
	estado_database = recargando;
	pedido_actual.tag = 0 ;
	pedido_actual.escribir = 0 ;
	pedido_actual.valor = NOVALUE ;
	scan = 0;
	db_cargar(archivo_inicial);
	// activo la base, y simulo que esta cargando
        holdIn( active, tiempo_recarga ) ;

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &Database::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == recargar )
	{
		estado_database = recargando ;
		pedido_actual.tag = 0 ;
		pedido_actual.escribir = 0 ;
		pedido_actual.valor = NOVALUE ;
		scan = 0;
		db_cargar(msg.value());
                holdIn( active, tiempo_recarga ) ;
	}

	else if( (msg.port() == leer) &&
	  ( (estado_database == dblibre) || (estado_database == scanning) ))
	{
		estado_database = leyendo ;
		pedido_actual = dec_pedido_db( msg.value() );
		holdIn( active, tiempo_respuesta ) ;
	}

        else if( (msg.port() == drv_respuesta))
        {
		db_procesar_respuesta_drv( dec_respuesta_drv(msg.value()) );
                holdIn( active, tiempo_scan ) ; ////
        }

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &Database::internalFunction( const InternalMessage & )
{
	Time tiempo_cero (0,0,0,0);

	if (estado_database == leyendo)
	{
		if (pedido_actual.escribir == 1)
			db_escribir (pedido_actual.tag, pedido_actual.valor);
		pedido_actual.tag = 0 ;
		pedido_actual.escribir = 0 ;
		pedido_actual.valor = NOVALUE ;
		estado_database = dblibre ;
                holdIn( active, tiempo_scan ) ;
	}

	else if (estado_database == recargando)
	{
		pedido_actual.tag = 0 ;
		pedido_actual.escribir = 0 ;
		pedido_actual.valor = NOVALUE ;
                holdIn( active, tiempo_scan ) ;
		estado_database = dblibre ;
	}

	else if (estado_database == dblibre)  ////
	{
		scan ++ ;
		if (scan >= db_count) scan = 0;
		estado_database = scanning ;
		holdIn (active, tiempo_cero);
	}

	else if (estado_database == scanning) ////
	{
		estado_database = dblibre ;
		holdIn (active, tiempo_scan);
	}

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &Database::outputFunction( const InternalMessage &msg )
{
	respuesta_db r;
	pedido_drv p;

	if (estado_database == leyendo)
	{
		r.tag = pedido_actual.tag ;
		// Si bien la transicion interna escribe en la base de datos,
		// para cuando se llame a esta función esa escritura aun no
		// se realizó. Por lo tanto, en lugar de responder con el valor
		// de la base de datos, respondo con el del pedido
		if (pedido_actual.escribir == 1) {
			cout << msg.time() << " / NODO #" << nodo_local << " /    TAG_STATUS = ";
			if ( pedido_actual.valor == PRIM_MASTER) cout << "PRIM_MASTER\n";
			else if ( pedido_actual.valor == PRIM_SLAVE) cout << "PRIM_SLAVE\n";
			else if ( pedido_actual.valor == SEC_MASTER) cout << "SEC_MASTER\n";
			else if ( pedido_actual.valor == SEC_SLAVE) cout << "SEC_SLAVE\n";
			else cout << "?????\n";

			r.valor = pedido_actual.valor;
			//sendOutput( msg.time(), registro_pedido, cod_respuesta_db(r) ) ;
		}
		else {
			r.valor = db_leer (pedido_actual.tag);
			sendOutput( msg.time(), registro_pedido, cod_respuesta_db(r) ) ;
		}
	}

	if (estado_database == scanning)
	{
		p = db_obtener_pedido_drv (scan);
		if (p.dispositivo != -1) sendOutput( msg.time(), drv_pedido, cod_pedido_drv (p) ) ;
	}

	return *this ;
}


/*******************************************************************
* Function Name: db_cargar
* Description:
********************************************************************/
void Database::db_cargar(int archivo)
{
	registro_db r;
	int idx = 0;
	db_count = 0;
	if (archivo == DB_MASTER) {

		r.dispositivo = 8;
		r.valor = NOVALUE;

		r.tag = 80+idx; r.direccion = 80+idx; db[idx++] = cod_registro_db ( r );
		r.tag = 80+idx; r.direccion = 80+idx; db[idx++] = cod_registro_db ( r );
		r.tag = 80+idx; r.direccion = 80+idx; db[idx++] = cod_registro_db ( r );

		// El siguiente debe estar presente para que funcione bien el HotStandby
		r.tag = TAG_STATUS;
		r.dispositivo = 0; r.direccion = 0;
		r.valor = PRIM_MASTER;
		db[idx++] = cod_registro_db ( r );

		db_count = idx;

	} else if (archivo == DB_SLAVE) {

		r.dispositivo = 3;
		r.valor = NOVALUE;

		// El siguiente debe estar presente para que funcione bien el HotStandby
		r.tag = TAG_STATUS;
		r.dispositivo = 0; r.direccion = 0;
		r.valor = SEC_SLAVE;
		db[idx++] = cod_registro_db ( r );

		db_count = idx;

	}
}

/*******************************************************************
* Function Name: db_escribir
* Description:
********************************************************************/
void Database::db_escribir(int tag, int valor)
{
	int i=0;
	registro_db reg;

	while (i < db_count) {
		reg = dec_registro_db (db[i]);
		if (reg.tag == tag) {
			reg.valor = valor;
			db[i] = cod_registro_db (reg);
		}
		i++;
	}
}

/*******************************************************************
* Function Name: db_leer
* Description:
********************************************************************/
int Database::db_leer(int tag)
{
	int i=0;
	registro_db reg;

	while (i < db_count) {
		reg = dec_registro_db (db[i]);
		if (reg.tag == tag) {
			return reg.valor;
		}
		i++;
	}

	return NOTAG;
}

/*******************************************************************
* Function Name: db_obtener_pedido_drv
* Description:
********************************************************************/
pedido_drv Database::db_obtener_pedido_drv(int num_registro)
{
	pedido_drv p;
	registro_db reg = dec_registro_db (db[num_registro]);
	p.dispositivo = reg.dispositivo;
	p.direccion = reg.direccion;

	if (reg.tag == TAG_STATUS) p.dispositivo = -1;

	return p;
}

/*******************************************************************
* Function Name: db_procesar_respuesta_drv
* Description:
********************************************************************/
void Database::db_procesar_respuesta_drv(respuesta_drv r)
{
	for (int idx=0; idx<db_count; idx++) {
		registro_db reg = dec_registro_db (db[idx]);
		if ((reg.dispositivo == r.dispositivo) &&
		    (reg.direccion == r.direccion) &&
		    (reg.tag != TAG_STATUS))  // evito que el driver me pise su valor...
		    {
		    	// me asegure de que la respuesta que me llega es la que yo esperaba
			// ya que si sale un pedido al driver, y en el interin se recarga la
			// base de datos, puede que reciba una respuesta errada.
			reg.valor = r.valor;
			db[idx] = cod_registro_db (reg);
		    }
	}
}
