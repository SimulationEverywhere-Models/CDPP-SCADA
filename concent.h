/*******************************************************************
*
*  DESCRIPTION: Atomic Model Concentrador
*
*  AUTHOR: Ariel Gonzalez
*
*  EMAIL: mailto://egonzalz@dc.uba.ar
*         mailto://agonzalez@tecnet-ibermatica.com.ar
*
*  DATE: 27/9/2003
*
*******************************************************************/

#ifndef __CONCE_H
#define __CONCE_H

#include "atomic.h"     // class Atomic
#include "datos.h"	// tipos de datos y conversiones

#define TAM_COLA	15

class Concentrador : public Atomic
{
public:
	Concentrador( const string &name = "Concentrador" );			//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &in1;
	const Port &in2;
	const Port &in;
	Port &out1;
	Port &out2;
	Port &out;

	// parametros
	Time timeout;
	Time ultimo_ingreso;
	Time tiempo_restante;

	// variables de estado
	typedef struct { int origen; int origen_red; Value msg; } Pedido;
	typedef list<Pedido> ColaConcentrador ;
	ColaConcentrador q ;
	int hay_pedido ;
	int respuesta_lista ;
	int respuesta_enviada ;
	Value respuesta;

};	// class Concentrador

// ** inline ** //
inline
string Concentrador::className() const
{
	return "Concentrador" ;
}

#endif   //__CONCE_H
