/*******************************************************************
*
*  DESCRIPTION: Simulator::registerNewAtomics()
*
*  AUTHOR: Amir Barylko & Jorge Beyoglonian
*          Ariel Gonzalez
*
*  EMAIL: mailto://amir@dc.uba.ar
*         mailto://jbeyoglo@dc.uba.ar
*  	  mailto://egonzalz@dc.uba.ar
*         mailto://agonzalez@tecnet-ibermatica.com.ar
*  DATE: 26/9/2003
*
*******************************************************************/

#include "modeladm.h"
#include "mainsimu.h"
#include "queue.h"      // class Queue
#include "generat.h"    // class Generator
#include "cpu.h"        // class CPU
#include "transduc.h"   // class Transducer
#include "trafico.h"    // class Trafico

#include "driver.h"     // class Driver
#include "database.h"   // class Database
#include "concent.h"    // class Concentrador
#include "red.h"        // class Red
#include "selec.h"      // class Selector
#include "pant.h"       // class Pantalla
#include "eth.h"        // class Ethernet
#include "hsclient.h"   // class HSClient
#include "hsserver.h"   // class HSServer


void MainSimulator::registerNewAtomics()
{
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Queue>() , "Queue" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Generator>() , "Generator" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<CPU>() , "CPU" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Transducer>() , "Transducer" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Trafico>() , "Trafico" ) ;

	//////////////// TP1
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Driver>() , "Driver" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Database>() , "Database" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Concentrador>() , "Concentrador" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Red>() , "Red" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Selector>() , "Selector" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Pantalla>() , "Pantalla" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Ethernet>() , "Ethernet" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<HSClient>() , "HSClient" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<HSServer>() , "HSServer" ) ;

}
