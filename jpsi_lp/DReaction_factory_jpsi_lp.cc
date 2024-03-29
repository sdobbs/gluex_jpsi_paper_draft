// $Id$
//
//    File: DReaction_factory_jpsi_lp.cc
// Created: Fri Oct 14 15:56:03 EDT 2016
// Creator: pentchev (on Linux ifarm1102 2.6.32-431.el6.x86_64 x86_64)
//


#include "DReaction_factory_jpsi_lp.h"
//#include "DCustomAction_jpsi_hists.h"
#include "DCustomAction_p2pi_hists.h"


//------------------
// brun
//------------------
jerror_t DReaction_factory_jpsi_lp::brun(JEventLoop* locEventLoop, int32_t locRunNumber)
{
	vector<double> locBeamPeriodVector;
	locEventLoop->GetCalib("PHOTON_BEAM/RF/beam_period", locBeamPeriodVector);
	dBeamBunchPeriod = locBeamPeriodVector[0];

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DReaction_factory_jpsi_lp::evnt(JEventLoop* locEventLoop, uint64_t locEventNumber)
{
	// Make as many DReaction objects as desired
	DReactionStep* locReactionStep = NULL;
	DReaction* locReaction = NULL; //create with a unique name for each DReaction object. CANNOT (!) be "Thrown"

	// DOCUMENTATION:
	// ANALYSIS library: https://halldweb1.jlab.org/wiki/index.php/GlueX_Analysis_Software
	// DReaction factory: https://halldweb1.jlab.org/wiki/index.php/Analysis_DReaction

	/************************************************** jpsi_lp Reaction Definition *************************************************/

	locReaction = new DReaction("jpsi_lp");

	//Required: DReactionSteps to specify the channel and decay chain you want to study
		//Particles are of type Particle_t, an enum defined in sim-recon/src/libraries/include/particleType.h

	//Example: g, p -> pi+, pi-, pi0, (p)
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Gamma);
	locReactionStep->Set_TargetParticleID(Proton);
	//locReactionStep->Add_FinalParticleID(Jpsi);
	locReactionStep->Add_FinalParticleID(Proton); //true: proton missing
	locReactionStep->Add_FinalParticleID(Electron);
	locReactionStep->Add_FinalParticleID(Positron);
	//locReactionStep->Add_FinalParticleID(Proton, true); //true: proton missing
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	/*
	// Jpsi->e+e-
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Jpsi);
	locReactionStep->Add_FinalParticleID(Electron);
	locReactionStep->Add_FinalParticleID(Positron);
	locReactionStep->Set_KinFitConstrainInitMassFlag(false); //Jpsi mass not constrained 
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak
	*/

	/**************************************************** jpsi_lp Control Settings ****************************************************/

	// Highly Recommended: Set EventStore skim query (use with "eventstore" source)
		// This will skip creating particle combos for events that aren't in the skims you list
		// Query should be comma-separated list of skims to boolean-AND together
	//locReaction->Set_EventStoreSkims("myskim1,myskim2,myskim3"); //boolean-AND of skims

	// Recommended: Type of kinematic fit to perform (default is d_NoFit)
		//fit types are of type DKinFitType, an enum defined in sim-recon/src/libraries/ANALYSIS/DReaction.h
		//Options: d_NoFit (default), d_P4Fit, d_VertexFit, d_P4AndVertexFit
		//P4 fits automatically constrain decaying particle masses, unless they are manually disabled
	//cok locReaction->Set_KinFitType(d_P4AndVertexFit);
	locReaction->Set_KinFitType(d_P4AndVertexFit);

	// Highly Recommended: When generating particle combinations, reject all beam photons that match to a different RF bunch
	//cok locReaction->Set_MaxPhotonRFDeltaT(1.5*dBeamBunchPeriod); //should be minimum cut value
	locReaction->Set_MaxPhotonRFDeltaT(3.5*dBeamBunchPeriod); //should be minimum cut value

	// Optional: When generating particle combinations, reject all photon candidates with a PID confidence level < 5.73303E-7 (+/- 5-sigma)
	// Make sure PID errors are calculated correctly before using. 
	//locReaction->Set_MinPhotonPIDFOM(5.73303E-7);

	// Optional: When generating particle combinations, reject all charged track candidates with a PID confidence level < 5.73303E-7 (+/- 5-sigma)
	// Make sure PID errors are calculated correctly before using. 
	//locReaction->Set_MinChargedPIDFOM(5.73303E-7);

	// Highly Recommended: Cut on number of extra "good" tracks. "Good" tracks are ones that survive the "PreSelect" (or user custom) factory.
		// Important: Keep cut large: Can have many ghost and accidental tracks that look "good"
	//cok locReaction->Set_MaxExtraGoodTracks(6);
	locReaction->Set_MaxExtraGoodTracks(6);

	// Highly Recommended: Enable ROOT TTree output for this DReaction
	// string is file name (must end in ".root"!!): doen't need to be unique, feel free to change
	//locReaction->Enable_TTreeOutput("tree_jpsi_lp.root", false); //true/false: do/don't save unused hypotheses
	//cold locReaction->Enable_TTreeOutput("tree_jpsi_lp.root", true); //true/false: do/don't save unused hypotheses
	locReaction->Enable_TTreeOutput("tree_jpsi_lp.root", false); //true/false: do/don't save unused hypotheses

	/************************************************** jpsi_lp Pre-Combo Custom Cuts *************************************************/

	// Highly Recommended: Very loose invariant mass cuts, applied during DParticleComboBlueprint construction
	// Example: pi0 -> g, g cut
	//cok locReaction->Set_InvariantMassCut(Jpsi, 0.5, 4.0);
	locReaction->Set_InvariantMassCut(Jpsi, 0.5, 4.0);

	// Highly Recommended: Very loose DAnalysisAction cuts, applied just after creating the combination (before saving it)
	// Example: Missing mass of proton
	//cok locReaction->Add_ComboPreSelectionAction(new DCutAction_MissingMass(locReaction, false, -0.1, 0.1));
        locReaction->Add_ComboPreSelectionAction(new DCutAction_MissingMassSquared(locReaction, false, -0.25, 0.25));

	/**************************************************** jpsi_lp Analysis Actions ****************************************************/

	///*
	// Recommended: Analysis actions automatically performed by the DAnalysisResults factories to histogram useful quantities.
		//These actions are executed sequentially, and are executed on each surviving (non-cut) particle combination 
		//Pre-defined actions can be found in ANALYSIS/DHistogramActions_*.h and ANALYSIS/DCutActions.h
		//If a histogram action is repeated, it should be created with a unique name (string) to distinguish them


	locReaction->Add_AnalysisAction(new DCustomAction_p2pi_hists(locReaction, false, "BeforeTimingCut_Measured"));


	// HISTOGRAM PID
	locReaction->Add_AnalysisAction(new DHistogramAction_PID(locReaction));

	// CUT PID
	// SYS_TOF, SYS_BCAL, SYS_FCAL, ...: DetectorSystem_t: Defined in libraries/include/GlueX.h
	// locReaction->Add_AnalysisAction(new DCutAction_EachPIDFOM(locReaction, 5.73303E-7));
	// locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 1.0, Proton, SYS_TOF)); //cut at delta-t +/- 1.0 //false: measured data

        ///*cok
        locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction,false, 2.5, Proton, SYS_TOF)); //false: measured data, 
        locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction,false, 2.5, Proton, SYS_BCAL));
        locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction,false, 2.5, Proton, SYS_FCAL));

        locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction,false, 1.0, Electron, SYS_TOF)); //false: measured data,
        locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction,false, 2.5, Electron, SYS_BCAL));
        locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction,false, 2.5, Electron, SYS_FCAL));

        locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction,false, 1.0, Positron, SYS_TOF)); //false: measured data,
        locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction,false, 2.5, Positron, SYS_BCAL));
        locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction,false, 2.5, Positron, SYS_FCAL));


	locReaction->Add_AnalysisAction(new DCustomAction_p2pi_hists(locReaction, false, "BeforeTimingCut_Measured"));
	locReaction->Add_AnalysisAction(new DCustomAction_p2pi_hists(locReaction, true, "BeforeTimingCut"));


        //*/

	// locReaction->Add_AnalysisAction(new DCutAction_PIDTimingBeta(locReaction, 0.0, 0.9, Neutron, SYS_BCAL)); //min/max beta cut for neutrons
	// locReaction->Add_AnalysisAction(new DCutAction_NoPIDHit(locReaction, KPlus)); //for K+ candidates, cut tracks with no PID hit


        //locReaction->Add_AnalysisAction(new DHistogramAction_MissingTransverseMomentum(locReaction, false ,1000, 0.0, 1.0)); //false: fill histograms with measured particle data
        //cok locReaction->Add_AnalysisAction(new DCutAction_TransverseMomentum(locReaction, 0.4)); //Max Missing Pt of 0.4 GeV
        locReaction->Add_AnalysisAction(new DCutAction_TransverseMomentum(locReaction, 0.5)); //Max Missing Pt of 0.4 GeV



	// KINEMATIC FIT

        //locReaction->Add_AnalysisAction(new DHistogramAction_KinFitResults(locReaction,0.05)); //See what results look like pre-KF
        //locReaction->Add_AnalysisAction(new DCutAction_KinFitFOM(locReaction, 0)); //Requires KF to converge

	// locReaction->Add_AnalysisAction(new DHistogramAction_KinFitResults(locReaction, 0.05)); //5% confidence level cut on pull histograms only
	// locReaction->Add_AnalysisAction(new DCutAction_KinFitFOM(locReaction, 0.0)); //0% confidence level cut //require kinematic fit converges

	// HISTOGRAM MASSES //false/true: measured/kinfit data
	//locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, Pi0, false, 600, 0.0, 0.3, "Pi0_PostKinFit"));
	//locReaction->Add_AnalysisAction(new DHistogramAction_MissingMass(locReaction, false, 1000, 0.7, 1.2, "PostKinFit"));

	// Kinematics
	//locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, false)); //false: measured data
	// locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, true, "KinFit")); //true: kinematic-fit data
	//locReaction->Add_AnalysisAction(new DHistogramAction_TrackVertexComparison(locReaction));
	//*/

        /*colda using measured P
        locReaction->Add_AnalysisAction(new DCutAction_TrackShowerEOverP(locReaction, false, SYS_FCAL, Positron, 0.6, "E/p cut"));
        locReaction->Add_AnalysisAction(new DCutAction_TrackShowerEOverP(locReaction, false, SYS_BCAL, Positron, 0.6, "E/p cut"));
        locReaction->Add_AnalysisAction(new DCutAction_TrackShowerEOverP(locReaction, false, SYS_FCAL, Electron, 0.6, "E/p cut"));
        locReaction->Add_AnalysisAction(new DCutAction_TrackShowerEOverP(locReaction, false, SYS_BCAL, Electron, 0.6, "E/p cut"));
        */

        locReaction->Add_AnalysisAction(new DCutAction_TrackShowerEOverP(locReaction, true , SYS_FCAL, Positron, 0.7, "E/p cut"));
        locReaction->Add_AnalysisAction(new DCutAction_TrackShowerEOverP(locReaction, true , SYS_BCAL, Positron, 0.7, "E/p cut"));
        locReaction->Add_AnalysisAction(new DCutAction_TrackShowerEOverP(locReaction, true , SYS_FCAL, Electron, 0.7, "E/p cut"));
        locReaction->Add_AnalysisAction(new DCutAction_TrackShowerEOverP(locReaction, true , SYS_BCAL, Electron, 0.7, "E/p cut"));

	_data.push_back(locReaction); //Register the DReaction with the factory

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DReaction_factory_jpsi_lp::fini(void)
{
	for(size_t loc_i = 0; loc_i < dReactionStepPool.size(); ++loc_i)
		delete dReactionStepPool[loc_i]; //cleanup memory
	return NOERROR;
}

