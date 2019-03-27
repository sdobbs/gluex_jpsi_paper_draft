// $Id$
//
//    File: DEventProcessor_jpsi_lp.h
// Created: Fri Oct 14 15:56:03 EDT 2016
// Creator: pentchev (on Linux ifarm1102 2.6.32-431.el6.x86_64 x86_64)
//

#ifndef _DEventProcessor_jpsi_lp_
#define _DEventProcessor_jpsi_lp_

#include <JANA/JEventProcessor.h>
#include <JANA/JApplication.h>

#include <ANALYSIS/DEventWriterROOT.h>
#include <HDDM/DEventWriterREST.h>
#include <ANALYSIS/DHistogramActions.h>

#include "DFactoryGenerator_jpsi_lp.h"

using namespace jana;
using namespace std;

class DEventProcessor_jpsi_lp : public jana::JEventProcessor
{
	public:
		const char* className(void){return "DEventProcessor_jpsi_lp";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop* locEventLoop, int32_t locRunNumber);	///< Called every time a new run number is detected.
		jerror_t evnt(jana::JEventLoop* locEventLoop, uint64_t locEventNumber);	///< Called every event.
		jerror_t erun(void);						///< Called every time run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		//For non-custom reaction-independent histograms, it is recommended that you simultaneously run the monitoring_hists plugin instead of defining them here

		//For making EventStore skims
		int Get_FileNumber(JEventLoop* locEventLoop) const;
		ofstream dEventStoreSkimStream;

		TH1F *dProton_DeltaT, *dElectron_DeltaT, *dPositron_DeltaT, *dTrack_DeltaT;
};

#endif // _DEventProcessor_jpsi_lp_

