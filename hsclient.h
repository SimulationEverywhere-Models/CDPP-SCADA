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

#ifndef __HSCLIENT_H
#define __HSCLIENT_H

#include "atomic.h"     // class Atomic
#include "datos.h"	// tipos de datos y conversiones

enum    estado_hsc { hsc_inactivo, hsc_recibiendo, hsc_poleando };

class HSClient : public Atomic
{
public:
	HSClient( const string &name = "HSClient" );			//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &start;
	const Port &stop;
	const Port &red_in;
	Port &nombre_logico;
	Port &red_out;

	// parametros
	Time poll_time;
	int nodo_hs;
	int nodo_hs_prim;
	int nodo_hs_sec;

	// variables de estado
	int polear_primario;
	int cant_poleos_sin_respuesta;
	estado_hsc estado_hsclient;
	Value ultima_respuesta;

};	// class HSClient

// ** inline ** //
inline
string HSClient::className() const
{
	return "HSClient" ;
}

#endif   //__HSCLIENT_H
