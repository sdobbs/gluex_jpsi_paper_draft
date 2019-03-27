// macro to process analysis TTree with DSelector
#include <iostream> 
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
   
void runDSelector(bool proof = 1, string path = "") 
{
	// Load DSelector library
	gROOT->ProcessLine(".x $(ROOT_ANALYSIS_HOME)/scripts/Load_DSelector.C");
	int proof_Nthreads = 6;
	//int proof_Nthreads = 15;
	//int proof_Nthreads = 45;

	// open ROOT files and TTree
	TChain *chain = new TChain("jpsi_lp_Tree");
	ifstream inf;
    //chain->Add("tree_jpsi_lp-photos-noepcut-nokinfitconverge.root");
    //chain->Add("tree_jpsi_lp-nophotos-noepcut-nokinfitconverge.root");
    //chain->Add("tree_jpsi_lp-nophotos-noepcut.root");
    //chain->Add("tree_jpsi_lp-photos-noepcut.root");
    //chain->Add("tree_jpsi_lp-photos.root");
    //chain->Add("tree_jpsi_lp-nophotos.root");

    //chain->Add("tree_jpsi_lp-photos-more.root");
    
    //chain->Add("tree_jpsi_lp-photos-m1.0.root");
    //chain->Add("tree_jpsi_lp-photos-m1.5.root");
    //chain->Add("tree_jpsi_lp-photos-m2.0.root");
    //chain->Add("tree_jpsi_lp-photos-m2.5.root");
    //chain->Add("tree_jpsi_lp-photos-m3.1.root");
    
    //chain->Add("tree_jpsi_lp-nophotos-m1.0.root");
    //chain->Add("tree_jpsi_lp-nophotos-m1.5.root");
    //chain->Add("tree_jpsi_lp-nophotos-m2.0.root");
    //chain->Add("tree_jpsi_lp-nophotos-m2.5.root");
    //chain->Add("tree_jpsi_lp-nophotos-m3.1.root");


    //chain->Add("tree_jpsi_lp-photos-bh-m1.0.root");
    //chain->Add("tree_jpsi_lp-photos-bh-m1.5.root");
    //chain->Add("tree_jpsi_lp-photos-bh-m2.0.root");
    //chain->Add("tree_jpsi_lp-photos-bh-m2.5.root");
    
    //chain->Add("tree_jpsi_lp-nophotos-bh-m1.0.root");
    //chain->Add("tree_jpsi_lp-nophotos-bh-m1.5.root");
    //chain->Add("tree_jpsi_lp-nophotos-bh-m2.0.root");
    //chain->Add("tree_jpsi_lp-nophotos-bh-m2.5.root");



    
	//inf.open("root_trees.2016");
	//inf.open("root_trees");
	//inf.open("jpsi_lp_root");
	//inf.open("jpsi_lp_root.11145");
	//inf.open("jpsi_lp_root.11448");
	inf.open("mc-new-2016");
	//inf.open("mc-new-2017");
	string line;
	int nfile = 0;
	while(inf >> line) {
		//cout << line << endl;
		//if(nfile++ > 20)
		//	break;
		//if(nfile < 5)
		//	continue;

		cout << line << endl;
		chain->Add(line.c_str());
	}
    

	//string options = sample;
	string options = "";
	if(proof) { // add TTree to chain and use PROOFLiteManager
		string outputHistFileName = Form("hist_jpsi.root");
		string outputTreeFileName = Form("tree_jpsi.root");
		DPROOFLiteManager::Process_Chain(chain, "DSelector_jpsi.C+g",  proof_Nthreads, outputHistFileName, outputTreeFileName, options);
	}
	else { // get TTree and use standard TTree::Process
		//TFile *f = TFile::Open(fileName);
		//TTree *tree = (TTree*)f->Get("omega_skim_Tree");
		chain->Process("DSelector_jpsi.C+", options.data());
	}
	
	return;
}
