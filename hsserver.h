/*******************************************************************
*
*  DESCRIPTION: Atomic Model HSServer
*
reg*  AUTHOR: Ariel Gonzalez
*
*  EMAIL: mailto://egonzalz@dc.uba.ar
*         mailto://agonzalez@tecnet-ibermatica.com.ar
*
*  DATE: 26/9/2003
*
*******************************************************************/

#ifndef __HSSERVER_H
#define __HSSERVER_H

#include "atomic.h"     // class Atomic
#include "datos.h"	// tipos de datos y conversiones

enum    estado_hss { hss_inactivo, hss_inicio, hss_recibiendo, hss_poleando, hss_switcheando, hss_switcheando_1 };

class HSServer : public Atomic
{
public:
	HSServer( const string &name = "HSServer" );			//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &start;
	const Port &stop;
	const Port &failover_master;
	const Port &failover_slave;
	const Port &red_in;
	Port &start_drv;
	Port &stop_drv;
	Port &reload_pdb;
	Port &nombre_logico;
	Port &red_out;

	// parametros
	Time poll_time;
	int max_reintentos;
	int mi_nombre_fisico;
	configuracion_nodo mi_config;

	// variables de estado
	estado_hss estado_hsserver;
	int hay_poleo_pendiente;
	int poleos_fallidos;
	int proximo_estado;
	Value ultima_respuesta;

};	// class HSServer

// ** inline ** //
inline
string HSServer::className() const
{
	return "HSServer" ;
}

#endif   //__HSSERVER_H
