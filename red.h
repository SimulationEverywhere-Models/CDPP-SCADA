/*******************************************************************
*
*  DESCRIPTION: Atomic Model Red
*
*  AUTHOR: Ariel Gonzalez
*
*  EMAIL: mailto://egonzalz@dc.uba.ar
*         mailto://agonzalez@tecnet-ibermatica.com.ar
*
*  DATE: 28/9/2003
*
*******************************************************************/

#ifndef __RED_H
#define __RED_H

#include "atomic.h"     // class Atomic
#include "datos.h"	// tipos de datos y conversiones

enum    _estado_red { hay_pedido_aux, hay_pedido_pdb, hay_pedido_ws, hay_pedido_eth };

class Red : public Atomic
{
public:
	Red( const string &name = "Red" );			//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &pdb_in;
	const Port &ws_in;
	const Port &aux_in;
	const Port &eth_in;
	const Port &nombre_logico;
	Port &pdb_out;
	Port &ws_out;
	Port &aux_out;
	Port &eth_out;

	// parametros
	Time tiempo_respuesta;
	configuracion_nodo *configuracion;
	int mi_nombre_fisico;

	// variables de estado
	_estado_red estado_red;
	Value ultimo_pedido;

};	// class Red

// ** inline ** //
inline
string Red::className() const
{
	return "Red" ;
}

#endif   //__RED_H
