#ifndef DSelector_jpsi_h
#define DSelector_jpsi_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"

class DSelector_jpsi : public DSelector
{
	public:

		DSelector_jpsi(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_jpsi(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

		// ANALYZE CUT ACTIONS
		// // Automatically makes mass histograms where one cut is missing
		DHistogramAction_AnalyzeCutActions* dAnalyzeCutActions;

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dProtonWrapper;
		DChargedTrackHypothesis* dElectronWrapper;
		DChargedTrackHypothesis* dPositronWrapper;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TH1I* dHist_MissingMassSquared;
		TH1I* dHist_BeamEnergy;
		
		TH1F* dHist_NumGoodEvents;
		
		TH1F* dHist_RFTime;
		TH1F* dHist_MJpsi;
		TH1F* dHist_MJpsi_withcombos;
		TH1F* dHist_MJpsi_acc;
		TH1F* dHist_MJpsi_nofit;
		TH1F* dHist_MJpsi_acc_nofit;

		TH1F* dHist_MJpsi_Pq_all;
		TH1F* dHist_MJpsi_Pq;

		TH1F* dHist_MJpsi_Pq_tpcut1;
		TH1F* dHist_MJpsi_Pq_tpcut2;
		TH1F* dHist_MJpsi_Pq_tpcut3;

		TH1F* dHist_MJpsi_Pq_tcut1;
		TH1F* dHist_MJpsi_Pq_tcut2;
		TH1F* dHist_MJpsi_Pq_tcut3;
		TH1F* dHist_MJpsi_Pq_tcut4;
		TH1F* dHist_MJpsi_Pq_tcut5;

		TH1F* dHist_MJpsi_Pqside1_tcut1;
		TH1F* dHist_MJpsi_Pqside1_tcut2;
		TH1F* dHist_MJpsi_Pqside1_tcut3;
		TH1F* dHist_MJpsi_Pqside1_tcut4;
		TH1F* dHist_MJpsi_Pqside1_tcut5;

		TH1F* dHist_MJpsi_Pqside2_tcut1;
		TH1F* dHist_MJpsi_Pqside2_tcut2;
		TH1F* dHist_MJpsi_Pqside2_tcut3;
		TH1F* dHist_MJpsi_Pqside2_tcut4;
		TH1F* dHist_MJpsi_Pqside2_tcut5;
		
		TH2F* dHist_Ebeam_t;
		TH2F* dHist_Ebeam_tp;
		
		TH1F* dHist_MJpsi_final;
		TH1F* dHist_MJpsi_acc_final;

		TH1F* dHist_MJpsi_Pq_all_final;
		TH1F* dHist_MJpsi_Pq_final;
		
		TH1F* dHist_MJpsi_Pq_tcut1_final;
		TH1F* dHist_MJpsi_Pq_tcut2_final;
		TH1F* dHist_MJpsi_Pq_tcut3_final;
		TH1F* dHist_MJpsi_Pq_tcut4_final;
		TH1F* dHist_MJpsi_Pq_tcut5_final;

		TH1F* dHist_MJpsi_Pqside1_tcut1_final;
		TH1F* dHist_MJpsi_Pqside1_tcut2_final;
		TH1F* dHist_MJpsi_Pqside1_tcut3_final;
		TH1F* dHist_MJpsi_Pqside1_tcut4_final;
		TH1F* dHist_MJpsi_Pqside1_tcut5_final;

		TH1F* dHist_MJpsi_Pqside2_tcut1_final;
		TH1F* dHist_MJpsi_Pqside2_tcut2_final;
		TH1F* dHist_MJpsi_Pqside2_tcut3_final;
		TH1F* dHist_MJpsi_Pqside2_tcut4_final;
		TH1F* dHist_MJpsi_Pqside2_tcut5_final;
		
		TH2F* dHist_Ebeam_t_final;
		
		vector< pair<double,double> > dBinRange, dPQBinRange;

		vector<TH1F*>  dHist_MJpsi_bins;
		vector<TH1F*>  dHist_MJpsi_bins_final;
		vector<TH1F*>  dHist_MJpsi_bins_withcombos;
		
		vector<TH1F*>  dHist_MJpsi_bins_tcut1;
		vector<TH1F*>  dHist_MJpsi_bins_tcut2;
		vector<TH1F*>  dHist_MJpsi_bins_tcut3;


	ClassDef(DSelector_jpsi, 0);
};

void DSelector_jpsi::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(0));
	dElectronWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));
	dPositronWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(2));
}

#endif // DSelector_jpsi_h
