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

#ifndef __ETH_H
#define __ETH_H

#include "atomic.h"     // class Atomic
#include "datos.h"	// tipos de datos y conversiones

#define MAX_DATALINKS  20

class Ethernet : public Atomic
{
public:
	Ethernet( const string &name = "Ethernet" );			//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &in1;
	const Port &in2;
	const Port &in3;
	const Port &inhabilitar;
	const Port &habilitar;
	Port &out1;
	Port &out2;
	Port &out3;

	// parametros
	Time delay;
	int nodo_1;
	int nodo_2;
	int nodo_3;

	// variables de estado
	int hab1;
	int hab2;
	int hab3;
	int hay_pedido;
	Value pedido;

};	// class Ethernet

// ** inline ** //
inline
string Ethernet::className() const
{
	return "Ethernet" ;
}

#endif   //__ETH_H
