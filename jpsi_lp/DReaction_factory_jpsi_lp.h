// $Id$
//
//    File: DReaction_factory_jpsi_lp.h
// Created: Fri Oct 14 15:56:03 EDT 2016
// Creator: pentchev (on Linux ifarm1102 2.6.32-431.el6.x86_64 x86_64)
//

#ifndef _DReaction_factory_jpsi_lp_
#define _DReaction_factory_jpsi_lp_

#include <iostream>
#include <iomanip>

#include <JANA/JFactory.h>
#include <ANALYSIS/DReaction.h>
#include <ANALYSIS/DHistogramActions.h>
#include <ANALYSIS/DCutActions.h>

using namespace std;
using namespace jana;

class DReaction_factory_jpsi_lp : public jana::JFactory<DReaction>
{
	public:
		DReaction_factory_jpsi_lp()
		{
			// This is so that the created DReaction objects persist throughout the life of the program instead of being cleared each event. 
		  SetFactoryFlag(PERSISTANT);
		  //SetFactoryFlag(NOT_OBJECT_OWNER);
		  //ClearFactoryFlag(WRITE_TO_OUTPUT);
		}
		const char* Tag(void){return "jpsi_lp";}

	private:
		jerror_t brun(JEventLoop* locEventLoop, int32_t locRunNumber);
		jerror_t evnt(JEventLoop* locEventLoop, uint64_t locEventNumber);
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		double dBeamBunchPeriod;
		deque<DReactionStep*> dReactionStepPool; //to prevent memory leaks
};

#endif // _DReaction_factory_jpsi_lp_

