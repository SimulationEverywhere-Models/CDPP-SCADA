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

#ifndef __DATABASE_H
#define __DATABASE_H

#include "atomic.h"     // class Atomic
#include "datos.h"	// tipos de datos y conversiones

#define MAX_RECORDS	25

enum    estado_db { leyendo, recargando, scanning, dblibre };

class Database : public Atomic
{
public:
	Database( const string &name = "Database" );			//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &recargar;
	const Port &leer;
	const Port &drv_respuesta;
	Port &registro_pedido;
	Port &drv_pedido;

	// parametros
	Time tiempo_respuesta;
	Time tiempo_recarga;
	Time tiempo_scan;
	int archivo_inicial;
	int nodo_local; // solo para el testing

	// variables de estado
	estado_db estado_database;
	Value db[MAX_RECORDS];  // usar cod_registro_db/dec_registro_db
	int scan;
	pedido_db pedido_actual;

	// funciones de la base de datos
	int db_count;
	void db_cargar(int archivo);
	int db_leer(int tag);
	void db_escribir(int tag, int valor);
	pedido_drv db_obtener_pedido_drv(int num_registro);
	void db_procesar_respuesta_drv(respuesta_drv r);

};	// class Database

// ** inline ** //
inline
string Database::className() const
{
	return "Database" ;
}

#endif   //__DATABASE_H

