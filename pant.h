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

#ifndef __PANT_H
#define __PANT_H

#include "atomic.h"     // class Atomic
#include "datos.h"	// tipos de datos y conversiones

#define MAX_DATALINKS  20

class Pantalla : public Atomic
{
public:
	Pantalla( const string &name = "Pantalla" );			//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &in;
	Port &out;

	// parametros
	Time refresh_rate;

	int cant_datalinks;
	datalink *datalinks;

	// variables de estado
	int cur_datalink;

};	// class Pantalla

// ** inline ** //
inline
string Pantalla::className() const
{
	return "Pantalla" ;
}

#endif   //__PANT_H
