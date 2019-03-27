// $Id$
//
//    File: DFactoryGenerator_jpsi_lp.h
// Created: Fri Oct 14 15:56:03 EDT 2016
// Creator: pentchev (on Linux ifarm1102 2.6.32-431.el6.x86_64 x86_64)
//

#ifndef _DFactoryGenerator_jpsi_lp_
#define _DFactoryGenerator_jpsi_lp_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_jpsi_lp.h"

class DFactoryGenerator_jpsi_lp : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_jpsi_lp";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_jpsi_lp());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_jpsi_lp_

