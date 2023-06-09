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

#ifndef __DRIVER_H
#define __DRIVER_H

#include "atomic.h"     // class Atomic
#include "datos.h"	// tipos de datos y conversiones

enum    estado_drv { ocupado, libre, inactivo };

class Driver : public Atomic
{
public:
	Driver( const string &name = "Driver" );			//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &start;
	const Port &stop;
	const Port &pedido;
	Port &respuesta;

	// parametros
	Time tiempo_respuesta;

	// variables de estado
	estado_drv estado_driver;
	pedido_drv pedido_actual;

};	// class Driver

// ** inline ** // 
inline
string Driver::className() const
{
	return "Driver" ;
}

#endif   //__DRIVER_H
