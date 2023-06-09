/*******************************************************************
*
*  DESCRIPTION: Atomic Model HSServer
*
*  AUTHOR: Ariel Gonzalez
*
*  EMAIL: mailto://egonzalz@dc.uba.ar
*         mailto://agonzalez@tecnet-ibermatica.com.ar
*
*  DATE: 26/9/2003
*
*******************************************************************/

/** include files **/
#include "hsserver.h"   // class HSServer
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: HSServer
* Description:
********************************************************************/
HSServer::HSServer( const string &name )
: Atomic( name )
, start( addInputPort( "start" ) )
, stop( addInputPort( "stop" ) )
, failover_master( addInputPort( "failover_master" ) )
, failover_slave( addInputPort( "failover_slave" ) )
, red_in( addInputPort( "red_in" ) )
, start_drv( addOutputPort( "start_drv" ) )
, stop_drv( addOutputPort( "stop_drv" ) )
, reload_pdb( addOutputPort( "reload_pdb" ) )
, nombre_logico( addOutputPort( "nombre_logico" ) )
, red_out( addOutputPort( "red_out" ) )
, poll_time( 0, 0, 1, 0 )
{
	string ptime( MainSimulator::Instance().getParameter( description(), "poll_time" ) ) ;
	string maxr( MainSimulator::Instance().getParameter( description(), "max_reintentos" ) ) ;
	string mi_nf   ( MainSimulator::Instance().getParameter( description(), "nodo_local" ) ) ;
	string mi_log  ( MainSimulator::Instance().getParameter( description(), "nodo_log" ) );
	string mi_prim ( MainSimulator::Instance().getParameter( description(), "nodo_prim" ) );
	string mi_sec  ( MainSimulator::Instance().getParameter( description(), "nodo_sec" ) );

	max_reintentos = str2Int ( maxr );
	mi_nombre_fisico = str2Int ( mi_nf );
	mi_config = leerConfiguracion (mi_log, mi_prim, mi_sec);
	if( ptime != "" ) poll_time = ptime ;
}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &HSServer::initFunction()
{

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &HSServer::externalFunction( const ExternalMessage &msg )
{
	Time tiempo_cero (0,0,0,0);

	if ( (state() == passive) && (msg.port() == start)) {
		hay_poleo_pendiente = 0;
		poleos_fallidos = 0;
		estado_hsserver = hss_inicio;
		holdIn (active, tiempo_cero);
	}

	if ( (state() == active) && ( msg.port() == stop ) ) {
		hay_poleo_pendiente = 0;
		poleos_fallidos = 0;
		estado_hsserver = hss_inactivo;
		cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / HOT-STANDBY DETENIDO.\n";
		passivate();
	}

	if ( (state() == active) && ( msg.port() == red_in ) ) {
		ultima_respuesta = msg.value();
		estado_hsserver = hss_recibiendo;
		holdIn (active, tiempo_cero);
	}

	if ( (state() == active) && ( msg.port () == failover_master ) ) {
		if ( es_master (mi_nombre_fisico, mi_config ) ) {
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / SWITCHEO MANUAL A MASTER: IGNORADO\n";
			hay_poleo_pendiente = 0;
			poleos_fallidos = 0;
			estado_hsserver = hss_inactivo;
			holdIn (active, poll_time);
		}
		else
		{
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / SWITCHEO MANUAL A MASTER: SWITCHEANDO\n";
			hay_poleo_pendiente = 0;
			poleos_fallidos = 0;
			estado_hsserver = hss_switcheando;
			if ( mi_nombre_fisico == mi_config.nombre_primario)
				proximo_estado = PRIM_MASTER; // nodo primario
			if ( mi_nombre_fisico == mi_config.nombre_secundario)
				proximo_estado = SEC_MASTER; // nodo secundario
			holdIn (active, tiempo_cero);
		}
	}

	if ( (state() == active) && ( msg.port () == failover_slave ) ) {
		if ( es_master (mi_nombre_fisico, mi_config ) ) {
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / SWITCHEO MANUAL A SLAVE: SWITCHEANDO\n";
			hay_poleo_pendiente = 0;
			poleos_fallidos = 0;
			estado_hsserver = hss_switcheando;
			if ( mi_nombre_fisico == mi_config.nombre_primario)
				proximo_estado = PRIM_SLAVE; // nodo primario
			if ( mi_nombre_fisico == mi_config.nombre_secundario)
				proximo_estado = SEC_SLAVE; // nodo secundario
			holdIn (active, tiempo_cero);
		}
		else
		{
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / SWITCHEO MANUAL A SLAVE: IGNORADO\n";
			hay_poleo_pendiente = 0;
			poleos_fallidos = 0;
			estado_hsserver = hss_inactivo;
			holdIn (active, poll_time);
		}
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &HSServer::internalFunction( const InternalMessage &msg)
{
	Time tiempo_cero (0,0,0,0);

	if ( estado_hsserver == hss_inicio ) {
		// para asegurar que todo comienza bien, verifico si soy la
		// maquina primaria, si es asi switcheo a PRIM_MASTER, en otro
		// caso, switcheo a SEC_SLAVE
		hay_poleo_pendiente = 0;
		poleos_fallidos = 0;
		estado_hsserver = hss_switcheando;

		if ( mi_nombre_fisico == mi_config.nombre_primario) {
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / ARRANCANDO COMO PRIM_MASTER\n";
			proximo_estado = PRIM_MASTER;
		}
		if ( mi_nombre_fisico == mi_config.nombre_secundario) {
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / ARRANCANDO COMO SEC_SLAVE\n";
			proximo_estado = SEC_SLAVE;
		}

		holdIn (active, tiempo_cero);
	}

	else if ( estado_hsserver == hss_inactivo ) {
		if (hay_poleo_pendiente == 1) poleos_fallidos++;
		else poleos_fallidos = 0;
		hay_poleo_pendiente = 1;
		estado_hsserver = hss_poleando;
		holdIn (active, tiempo_cero);
	}

	else if ( estado_hsserver == hss_poleando ) {
		if ( poleos_fallidos >= (max_reintentos-1) ) {
			if ( !es_master (mi_nombre_fisico, mi_config )) {
				cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / SLAVE NO SE COMUNICA CON MASTER: SWITCHEANDO A MASTER\n";
				//// DEBERIA SWITCHEAR A "MASTER"
				hay_poleo_pendiente = 0;
				poleos_fallidos = 0;
				estado_hsserver = hss_switcheando;
				if ( mi_nombre_fisico == mi_config.nombre_primario)
					proximo_estado = PRIM_MASTER; // nodo primario
				if ( mi_nombre_fisico == mi_config.nombre_secundario)
					proximo_estado = SEC_MASTER; // nodo secundario
				holdIn (active, tiempo_cero);
			}
			else
			{
				hay_poleo_pendiente = 0;
				poleos_fallidos = 0;
				estado_hsserver = hss_inactivo;
				holdIn (active, poll_time);
			}
		}
		else
		{
			//if ( !es_master (mi_nombre_fisico, mi_config ))
			//	poleos_fallidos = 0;
			estado_hsserver = hss_inactivo;
			holdIn (active, poll_time);
		}
	}

	else if ( estado_hsserver == hss_recibiendo ) {
		int status_nodo_hs = procesar_poleo_hs ( ultima_respuesta );
		cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / RECIBE RESPUESTA CON ESTADO ";
		if ( status_nodo_hs == PRIM_MASTER) cout << "PRIM_MASTER\n";
		else if ( status_nodo_hs == PRIM_SLAVE) cout << "PRIM_SLAVE\n";
		else if ( status_nodo_hs == SEC_MASTER) cout << "SEC_MASTER\n";
		else if ( status_nodo_hs == SEC_SLAVE) cout << "SEC_SLAVE\n";
		else cout << "?????\n";

		//cout << "[" << status_nodo_hs << "]\n";
		if ( (( status_nodo_hs == PRIM_MASTER) || ( status_nodo_hs == SEC_MASTER))
		   && es_master ( mi_nombre_fisico, mi_config ) )
		// ambos nodos son MASTER
		{
			if ( mi_nombre_fisico == mi_config.nombre_primario) {
			// si soy el nodo primario, no hago nada...
				estado_hsserver = hss_inactivo;
				holdIn (active, poll_time);
			}

			if ( mi_nombre_fisico == mi_config.nombre_secundario) {
			// si soy el nodo secundario, me paso a "SLAVE"...
				cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / HAY 2 MASTER: SECUNDARIO SWITCHEA A SLAVE\n";
				hay_poleo_pendiente = 0;
				poleos_fallidos = 0;
				estado_hsserver = hss_switcheando;
				proximo_estado = SEC_SLAVE; // nodo secundario
				holdIn (active, tiempo_cero);
			}
		}

		else if ( (( status_nodo_hs == PRIM_MASTER) || ( status_nodo_hs == SEC_MASTER))
		   && !es_master ( mi_nombre_fisico, mi_config ) )
 		// soy SLAVE y el otro nodo es MASTER, OK reseteo el contador
		{
			hay_poleo_pendiente = 0;
			poleos_fallidos = 0;
			estado_hsserver = hss_inactivo;
			holdIn (active, poll_time);
		}

		else if ( (( status_nodo_hs == PRIM_SLAVE) || ( status_nodo_hs == SEC_SLAVE))
		   && !es_master ( mi_nombre_fisico, mi_config ) )
		// soy SLAVE y el otro nodo tambien es SLAVE
		{
			if ( mi_nombre_fisico == mi_config.nombre_primario) {
			// si soy el nodo primario, me paso a "MASTER"...
				cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / NO HAY MASTER: PRIMARIO SWITCHEA A MASTER\n";
				hay_poleo_pendiente = 0;
				poleos_fallidos = 0;
				estado_hsserver = hss_switcheando;
				proximo_estado = PRIM_MASTER; // nodo primario
				holdIn (active, tiempo_cero);
			}

			if ( mi_nombre_fisico == mi_config.nombre_secundario) {
			// si soy el nodo secundario, no hago nada...
				estado_hsserver = hss_inactivo;
				holdIn (active, poll_time);
			}
		}
		else
		{
			estado_hsserver = hss_inactivo;
			holdIn (active, poll_time);
		}
	}

	else if ( estado_hsserver == hss_switcheando ) {
		hay_poleo_pendiente = 0;
		poleos_fallidos = 0;
		estado_hsserver = hss_switcheando_1;

		if ( proximo_estado == PRIM_MASTER )
			mi_config.usar_primario = 1;
		else if ( proximo_estado == SEC_MASTER )
			mi_config.usar_primario = 0;
		else if ( proximo_estado == PRIM_SLAVE )
			mi_config.usar_primario = 0;
		else if ( proximo_estado == SEC_SLAVE )
			mi_config.usar_primario = 1;

		holdIn (active, poll_time);
	}
	else if ( estado_hsserver == hss_switcheando_1 ) {
		estado_hsserver = hss_inactivo;
		holdIn (active, tiempo_cero);
	}
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &HSServer::outputFunction( const InternalMessage &msg )
{
	Value poleo;

	if ( estado_hsserver == hss_inactivo ) {
		// envia un poleo... (a PRIM si soy SEC, o a SEC si soy PRIM)
		if ( mi_config.nombre_primario == mi_nombre_fisico )
			poleo = obtener_poleo_hs ( mi_config.nombre_secundario );
		if ( mi_config.nombre_secundario == mi_nombre_fisico )
			poleo = obtener_poleo_hs ( mi_config.nombre_primario );
		sendOutput( msg.time(), red_out, poleo ) ;
	}

	if ( estado_hsserver == hss_switcheando_1 ) {
		if ( proximo_estado == PRIM_MASTER ) {
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / PROX ESTADO: PRIM_MASTER\n";
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Iniciando Driver...\n";
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    PDB local es DB_MASTER...\n";
			sendOutput (msg.time(), start_drv, 1);
			//sendOutput (msg.time(), reload_pdb, DB_MASTER);
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Remapeando Nombre Lógico a Primario...\n";
			sendOutput (msg.time(),
					nombre_logico,
					obtener_msg_nombre_logico( mi_config.nombre_logico, PRIMARIO ));
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Actualizando TAG_STATUS...\n";
			sendOutput (msg.time(),
					red_out,
					actualizar_tag_status( mi_nombre_fisico, proximo_estado ));

		}
		else if ( proximo_estado == SEC_MASTER ) {
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / PROX ESTADO: SEC_MASTER\n";
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    PDB local es DB_MASTER...\n";
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Iniciando Driver...\n";
			sendOutput (msg.time(), start_drv, 1);
			//sendOutput (msg.time(), reload_pdb, DB_MASTER);
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Remapeando Nombre Lógico a Secundario...\n";
			sendOutput (msg.time(),
					nombre_logico,
					obtener_msg_nombre_logico( mi_config.nombre_logico, SECUNDARIO ));
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Actualizando TAG_STATUS...\n";
			sendOutput (msg.time(),
					red_out,
					actualizar_tag_status( mi_nombre_fisico, proximo_estado ));
		}
		else if ( proximo_estado == PRIM_SLAVE ) {
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / PROX ESTADO: PRIM_SLAVE\n";
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    PDB local es DB_SLAVE...\n";
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Deteniendo Driver...\n";
			sendOutput (msg.time(), stop_drv, 1);
			//sendOutput (msg.time(), reload_pdb, DB_SLAVE);
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Remapeando Nombre Lógico a Secundario...\n";
			sendOutput (msg.time(),
					nombre_logico,
					obtener_msg_nombre_logico( mi_config.nombre_logico, SECUNDARIO ));
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Actualizando TAG_STATUS...\n";
			sendOutput (msg.time(),
					red_out,
					actualizar_tag_status( mi_nombre_fisico, proximo_estado ));
		}
		else if ( proximo_estado == SEC_SLAVE ) {
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " / PROX ESTADO: SEC_SLAVE\n";
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    PDB local es DB_SLAVE...\n";
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Deteniendo Driver...\n";
			sendOutput (msg.time(), stop_drv, 1);
			//sendOutput (msg.time(), reload_pdb, DB_SLAVE);
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Remapeando Nombre Lógico a Primario...\n";
			sendOutput (msg.time(),
					nombre_logico,
					obtener_msg_nombre_logico( mi_config.nombre_logico, PRIMARIO ));
			cout << msg.time() << " / NODO #" << mi_nombre_fisico << " /    Actualizando TAG_STATUS...\n";
			sendOutput (msg.time(),
					red_out,
					actualizar_tag_status( mi_nombre_fisico, proximo_estado ));
		}

	}

	if ( estado_hsserver == hss_switcheando ) {
		if ( proximo_estado == PRIM_MASTER ) {
			sendOutput (msg.time(), reload_pdb, DB_MASTER);
		}
		else if ( proximo_estado == SEC_MASTER ) {
			sendOutput (msg.time(), reload_pdb, DB_MASTER);
		}
		else if ( proximo_estado == PRIM_SLAVE ) {
			sendOutput (msg.time(), reload_pdb, DB_SLAVE);
		}
		else if ( proximo_estado == SEC_SLAVE ) {
			sendOutput (msg.time(), reload_pdb, DB_SLAVE);
		}

	}

	return *this ;
}


