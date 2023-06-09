/*******************************************************************
*
*  DESCRIPTION: Atomic Model Selector
*
*  AUTHOR: Ariel Gonzalez
*
*  EMAIL: mailto://egonzalz@dc.uba.ar
*         mailto://agonzalez@tecnet-ibermatica.com.ar
*
*  DATE: 28/9/2003
*
*******************************************************************/

#ifndef __SELEC_H
#define __SELEC_H

#include "atomic.h"     // class Atomic
#include "datos.h"	// tipos de datos y conversiones

enum    estado_sel { hay_pedido_pdb_local, hay_pedido_pant, hay_pedido_red };

class Selector : public Atomic
{
public:
	Selector( const string &name = "Selector" );			//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &pdb_in;
	const Port &pant_in;
	const Port &red_in;
	const Port &nombre_logico;
	Port &pdb_out;
	Port &pant_out;
	Port &red_out;

	// parametros
	Time tiempo_respuesta;
	int mi_nombre_fisico;
	configuracion_nodo mi_config;

	// variables de estado
	estado_sel estado_selector;
	Value ultimo_pedido;

};	// class Selector

// ** inline ** //
inline
string Selector::className() const
{
	return "Selector" ;
}

#endif   //__SELEC_H
