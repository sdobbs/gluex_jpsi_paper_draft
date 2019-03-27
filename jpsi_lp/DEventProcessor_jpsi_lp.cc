// $Id$
//
//    File: DEventProcessor_jpsi_lp.cc
// Created: Fri Oct 14 15:56:03 EDT 2016
// Creator: pentchev (on Linux ifarm1102 2.6.32-431.el6.x86_64 x86_64)
//

#include "DEventProcessor_jpsi_lp.h"

// Routine used to create our DEventProcessor

extern "C"
{
	void InitPlugin(JApplication *locApplication)
	{
		InitJANAPlugin(locApplication);
		locApplication->AddProcessor(new DEventProcessor_jpsi_lp()); //register this plugin
		locApplication->AddFactoryGenerator(new DFactoryGenerator_jpsi_lp()); //register the factory generator
	}
} // "C"

//------------------
// init
//------------------
jerror_t DEventProcessor_jpsi_lp::init(void)
{
	// This is called once at program startup.

	/*
	//OPTIONAL: Create an EventStore skim.  
	string locSkimFileName = "jpsi_lp.idxa";
	dEventStoreSkimStream.open(locSkimFileName.c_str());
	dEventStoreSkimStream << "IDXA" << endl;
	*/

  TH1F *dTrack_DeltaT = new TH1F("Track Delta T (proc)", "; t(track) - t(RF) at target (ns)", 100, -10, 10);
  //TH1F *dProton_DeltaT = new TH1F("Proton Delta T", "; t(Proton) - t(RF) at target (ns)", 100, -10, 10);
  //TH1F *dElectron_DeltaT = new TH1F("Electron Delta T", "; t(Electron) - t(RF) at target (ns)", 100, -10, 10);
  //TH1F *dPositron_DeltaT = new TH1F("Positron Delta T", "; t(Positron) - t(RF) at target (ns)", 100, -10, 10);

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t DEventProcessor_jpsi_lp::brun(jana::JEventLoop* locEventLoop, int32_t locRunNumber)
{

	// This is called whenever the run number changes

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DEventProcessor_jpsi_lp::evnt(jana::JEventLoop* locEventLoop, uint64_t locEventNumber)
{

        // cout<<" event no "<<locEventNumber<<endl;

	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// locEventLoop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	//
	// Here's an example:
	//
	// vector<const MyDataClass*> mydataclasses;
	// locEventLoop->Get(mydataclasses);
	//
	// japp->RootFillLock(this);
	//  ... fill historgrams or trees ...
	// japp->RootFillUnLock(this);

	// DOCUMENTATION:
	// ANALYSIS library: https://halldweb1.jlab.org/wiki/index.php/GlueX_Analysis_Software

  /*
  vector<const DEventRFBunch*> locEventRFBunch;
  locEventLoop->Get(locEventRFBunch);

  vector< const DChargedTrack *> locChargedTracks; 
  locEventLoop->Get(locChargedTracks);

  if(locChargedTracks.size() == 0)
    return NOERROR;

  cerr << "RF = " << locEventRFBunch[0]->dTime << endl;
  cerr << "count = " << locChargedTracks.size() << endl;

  for(int i=0; i<locChargedTracks.size(); i++) {
    if(locChargedTracks[i]->Get_BestTrackingFOM() == NULL) continue;
    cerr << locChargedTracks[i]->Get_BestTrackingFOM()->t0() << endl;
    dTrack_DeltaT->Fill(locChargedTracks[i]->Get_BestTrackingFOM()->t0() - locEventRFBunch[0]->dTime);
  }
  */

	/*********************************************************** REQUIRED ***********************************************************/

	//REQUIRED: To run an analysis, You MUST call one at least of the below code fragments. 
		//JANA is on-demand, so if you don't call one of these, then your analysis won't run. 

	///*
	//Recommended: Write surviving particle combinations (if any) to output ROOT TTree
		//If no cuts are performed by the analysis actions added to a DReaction, then this saves all of its particle combinations. 
		//The event writer gets the DAnalysisResults objects from JANA, performing the analysis. 
	// string is DReaction factory tag: will fill trees for all DReactions that are defined in the specified factory
	const DEventWriterROOT* locEventWriterROOT = NULL;
	locEventLoop->GetSingle(locEventWriterROOT);
	locEventWriterROOT->Fill_DataTrees(locEventLoop, "jpsi_lp");
	//*/

	///*
	//Optional: Get the analysis results for all DReactions. 
		//Getting these objects triggers the analysis, if it wasn't performed already. 
		//These objects contain the DParticleCombo objects that survived the DAnalysisAction cuts that were added to the DReactions
	vector<const DAnalysisResults*> locAnalysisResultsVector;
	locEventLoop->Get(locAnalysisResultsVector);
	//*/

	/************************************************** OPTIONAL: FURTHER ANALYSIS **************************************************/
	/*
	//Optional: Get all particle combinations for all DReactions. 
		//If kinematic fits were requested, these contain both the measured and kinematic-fit track parameters
		//No cuts from DAnalysisActions are placed on these combos
	vector<const DParticleCombo*> locParticleCombos;
	locEventLoop->Get(locParticleCombos);
	for(size_t loc_i = 0; loc_i < locParticleCombos.size(); ++loc_i)
	{
		const DParticleCombo* locParticleCombo = locParticleCombos[loc_i];
		if(locParticleCombo->Get_Reaction()->Get_ReactionName() != "jpsi_lp")
			continue; // particle combination was for a different reaction
		//perform further analysis steps here...
	}
	*/

	/*
	//Optional: Perform further cuts on the particle combos in the analysis results. 
	for(size_t loc_i = 0; loc_i < locAnalysisResultsVector.size(); ++loc_i)
	{
		const DAnalysisResults* locAnalysisResults = locAnalysisResultsVector[loc_i];
		if(locAnalysisResults->Get_Reaction()->Get_ReactionName() != "jpsi_lp")
			continue; // analysis results were for a different reaction
		//get the DParticleCombo objects for this DReaction that survived all of the DAnalysisAction cuts
		deque<const DParticleCombo*> locPassedParticleCombos;
		locAnalysisResults->Get_PassedParticleCombos(locPassedParticleCombos);
                cout<<" combo no ="<<locPassedParticleCombos.size()<<endl;
		for(size_t loc_j = 0; loc_j < locPassedParticleCombos.size(); ++loc_j)
		{
			const DParticleCombo* locPassedParticleCombo = locPassedParticleCombos[loc_j];
  const DParticleComboStep* locParticleComboStep = locPassedParticleCombo->Get_ParticleComboStep(0);
        const DChargedTrack* locChargedTrack = static_cast<const DChargedTrack*>(locParticleComboStep->Get_FinalParticle_SourceObject(1));
        if(locChargedTrack != NULL) {
        const DChargedTrackHypothesis* locChargedTrackHypothesis = locChargedTrack->Get_Hypothesis(Positron);
        if(locChargedTrackHypothesis != NULL) {
           const DTrackTimeBased* locTrackTimeBased = NULL;
           locChargedTrackHypothesis->GetSingleT(locTrackTimeBased);
        if(locTrackTimeBased != NULL) {
        double locP = locTrackTimeBased->momentum().Mag();

            cout<<" positron momentum ="<<locP<<endl;

        //DFCALShowerMatchParams locFCALShowerMatchParams;
        //DBCALShowerMatchParams locBCALShowerMatchParams;

DBCALShowerMatchParams* locBCALShowerMatchParams = (DBCALShowerMatchParams*)locChargedTrackHypothesis->Get_BCALShowerMatchParams();
DFCALShowerMatchParams* locFCALShowerMatchParams = (DFCALShowerMatchParams*)locChargedTrackHypothesis->Get_FCALShowerMatchParams();


        if(locFCALShowerMatchParams!=NULL){
            const DFCALShower* locFCALShower = dynamic_cast<const DFCALShower*>(locFCALShowerMatchParams->dFCALShower); //dShowerObject
            double locFCALEnergy = locFCALShower->getEnergy();

                double FCAL_EP = locFCALEnergy/locP;
                if(FCAL_EP<0.6){
                  cout<<" false fcal "<<FCAL_EP<<" "<<locFCALEnergy<<" "<<locP<<endl;
                } else {
                  cout<<" true fcal "<<FCAL_EP<<" "<<locFCALEnergy<<" "<<locP<<endl;
                }
        }

        if(locBCALShowerMatchParams!=NULL){
            const DBCALShower* locBCALShower = dynamic_cast<const DBCALShower*>(locBCALShowerMatchParams->dBCALShower); //dShowerObject
            double locBCALEnergy = locBCALShower->E;

                double BCAL_EP = locBCALEnergy/locP;
                 if(BCAL_EP<0.6){
                  cout<<" false bcal "<<BCAL_EP<<" "<<locBCALEnergy<<" "<<locP<<endl;
                 } else {
                  cout<<" true bcal "<<BCAL_EP<<" "<<locBCALEnergy<<" "<<locP<<endl;
                 }

        }


        }
        }
        }

			//perform further analysis steps here...
		}
	}
	*/

	/******************************************************** OPTIONAL: SKIMS *******************************************************/

	/*
	//Optional: Save event to output REST file. Use this to create physical skims.
	const DEventWriterREST* locEventWriterREST = NULL;
	locEventLoop->GetSingle(locEventWriterREST);
	for(size_t loc_i = 0; loc_i < locAnalysisResultsVector.size(); ++loc_i)
	{
		const DAnalysisResults* locAnalysisResults = locAnalysisResultsVector[loc_i];
		if(locAnalysisResults->Get_Reaction()->Get_ReactionName() != "jpsi_lp")
			continue; // analysis results were for a different reaction

		//get the DParticleCombo objects for this DReaction that survived all of the DAnalysisAction cuts
		deque<const DParticleCombo*> locPassedParticleCombos;
		locAnalysisResults->Get_PassedParticleCombos(locPassedParticleCombos);

		if(!locPassedParticleCombos.empty())
			locEventWriterREST->Write_RESTEvent(locEventLoop, "jpsi_lp"); //string is part of output file name
	}
	*/

	/*
	//Optional: Create an EventStore skim. 

	// See whether this is MC data or real data
	vector<const DMCThrown*> locMCThrowns;
	locEventLoop->Get(locMCThrowns);

	unsigned int locRunNumber = locEventLoop->GetJEvent().GetRunNumber();
	unsigned int locUniqueID = locMCThrowns.empty() ? 1 : Get_FileNumber(locEventLoop);

	// If a particle combo passed the cuts, save the event info in the output file
	for(size_t loc_i = 0; loc_i < locAnalysisResultsVector.size(); ++loc_i)
	{
		const DAnalysisResults* locAnalysisResults = locAnalysisResultsVector[loc_i];
		if(locAnalysisResults->Get_Reaction()->Get_ReactionName() != "jpsi_lp")
			continue; // analysis results were for a different reaction
		if(locAnalysisResults->Get_NumPassedParticleCombos() == 0)
			continue; // no combos passed

		//MUST LOCK AROUND MODIFICATION OF MEMBER VARIABLES IN brun() or evnt().
		japp->WriteLock("jpsi_lp.idxa"); //Lock is unique to this output file
		{
			dEventStoreSkimStream << locRunNumber << " " << locEventNumber << " " << locUniqueID << endl;
		}
		japp->Unlock("jpsi_lp.idxa");
	}
	*/

	return NOERROR;
}

int DEventProcessor_jpsi_lp::Get_FileNumber(JEventLoop* locEventLoop) const
{
	//Assume that the file name is in the format: *_X.ext, where:
		//X is the file number (a string of numbers of any length)
		//ext is the file extension (probably .evio or .hddm)

	//get the event source
	JEventSource* locEventSource = locEventLoop->GetJEvent().GetJEventSource();
	if(locEventSource == NULL)
		return -1;

	//get the source file name (strip the path)
	string locSourceFileName = locEventSource->GetSourceName();

	//find the last "_" & "." indices
	size_t locUnderscoreIndex = locSourceFileName.rfind("_");
	size_t locDotIndex = locSourceFileName.rfind(".");
	if((locUnderscoreIndex == string::npos) || (locDotIndex == string::npos))
		return -1;

	size_t locNumberLength = locDotIndex - locUnderscoreIndex - 1;
	string locFileNumberString = locSourceFileName.substr(locUnderscoreIndex + 1, locNumberLength);

	int locFileNumber = -1;
	istringstream locFileNumberStream(locFileNumberString);
	locFileNumberStream >> locFileNumber;

	return locFileNumber;
}

//------------------
// erun
//------------------
jerror_t DEventProcessor_jpsi_lp::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DEventProcessor_jpsi_lp::fini(void)
{
	// Called before program exit after event processing is finished.
	if(dEventStoreSkimStream.is_open())
		dEventStoreSkimStream.close();
	return NOERROR;
}

