// Analysis of Z-> nuN -> nu l qq
// with N decaylength = 100mm and in mass range [30;80]GeV

#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"

// This is the minimal interface needed to access FastJet.
// A more sophisticated interface is demonstrated in main72.cc.
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

#include "TLorentzVector.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TRandom.h" // use normally TRandom3! Only for Integer() function
#include "TRandom3.h"

#include "MyUtils.h"    // Cusomised functions to identify particles
using namespace MyUtils;

#include <iostream>
#include <fstream>
using namespace std;


////////////////////////////////////////////////////////////////////////////
// read and analyse data produced by pythia
int main(int argc, char **argv) {
  // Definitions:

  TString histfname;
  std::string dataPath;
  std::string Bkgfile = "BkgEvents_Zqq.txt";
  //std::string Bkgfile_Ztautau = "BkgEvents_Ztautau.txt";
  TRandom3 myRandomNumber(1);
  TRandom myRandomArray(1);

  //////////////////////////////////////
  // Detector parametrisation, misidentification, ME-signal
  bool detector = true;
  bool misID = true;
  bool NewSignal = true;
  //////////////////////////////////////

  if ( argc<2 ) {
    printf("Must must enter option for process!\n");
    return 0;
  }

  ////////////////////////////////////////////////////////////////////////////
  // pion misID:
  ////////////////////////////////////////////////////////////////////////////
  // Background ////////////////////////////////////////////////////////
  // Z -> qq
  if (misID){
    if ( strcmp(argv[1],"Z")==0 ) {
      histfname = "./misID_histos/histosZ_misID.root"; ///////////
      if (detector) {
        histfname = "./misID_histos/histosZ_det_misID.root";
      }
      //dataPath = "/scr/bay/pion_hepmcout_Zqq.dat";
      dataPath = "/scr/bay/testSample_pion_hepmcout_Zqq.dat";


      // Tau Tau
    } else if ( strcmp(argv[1],"Ztautau")==0 ) {
      histfname = "./misID_histos/histosZtautau_misID.root";
      //dataPath = "/scr/bay/pion_hepmcout_Ztautau.dat";
      dataPath = "/scr/bay/testSample_pion_hepmcout_Ztautau.dat";

    // mu mu
    } else if ( strcmp(argv[1],"Zmumu")==0 ) { 
      histfname = "./misID_histos/histosZmumu_misID.root";
      //dataPath = "/scr/bay/pion_hepmcout_Zmumu.dat";
      dataPath = "/scr/bay/testSample_pion_hepmcout_Zmumu.dat";

      // four fermion process
    } else if ( strcmp(argv[1],"4f")==0 ) {
      histfname = "./misID_histos/histos_whiz_4fermion.root";
      dataPath = "/scr/bay/pion_w2p2hepmcout_4fermion.dat";

      // two photon processes:
      // ee->eeuU
    } else if ( strcmp(argv[1],"2phot_uU")==0 ) {
      histfname = "./misID_histos/histos_whiz_2phot_uU.root";
      dataPath = "/scr/bay/pion_w2p2hepmcout_2phot_uU.dat";

      // ee->eedD
    } else if ( strcmp(argv[1],"2phot_dD")==0 ) {
      histfname = "./misID_histos/histos_whiz_2phot_dD.root";
      dataPath = "/scr/bay/pion_w2p2hepmcout_2phot_dD.dat";

      // ee->eecC
    } else if ( strcmp(argv[1],"2phot_cC")==0 ) {
      dataPath = "/scr/bay/pion_w2p2hepmcout_2phot_cC.dat";

      histfname = "./misID_histos/histos_whiz_2phot_cC.root";
      // ee->eesS
    } else if ( strcmp(argv[1],"2phot_sS")==0 ) {
      histfname = "./misID_histos/histos_whiz_2phot_sS.root";
      dataPath = "/scr/bay/pion_w2p2hepmcout_2phot_sS.dat";

      // ee->eebB
    } else if ( strcmp(argv[1],"2phot_bB")==0 ) {
      histfname = "./misID_histos/histos_whiz_2phot_bB.root";
      dataPath = "/scr/bay/pion_w2p2hepmcout_2phot_bB.dat";


      // Signal ///////////////////////////////////////////////////////////
    } else if ( argv[1][0] == 'N' ) {
      // New Signal
      if (NewSignal){
        if (detector) {
          char hist[] = "./misID_histos/histosHNLxx_mu_det_NewSignal_misID_tau0.root";
          hist[24] = argv[1][1];
          hist[25] = argv[1][2];
          histfname = hist;
        }
        else {
          char hist[] = "./misID_histos/histosHNLxx_mu_NewSignal_misID.root";
          hist[9] = argv[1][1];
          hist[10] = argv[1][2];
          histfname = hist;
        }
       // char data[] = "/scr/bay/testSample_newSignal_hepmcout_HNLxx_mu.dat";
        char data[] = "/scr/bay/testSample_newSignal_hepmcout_HNLxx_mu_tau0.dat";
        data[42] = argv[1][1];
        data[43] = argv[1][2];
        dataPath = data;
      }
      // Old Signal
      if (NewSignal == false){
        if (detector) {
          char hist[] = "./misID_histos/histosHNLxx_mu_det_misID.root";
          hist[24] = argv[1][1];
          hist[25] = argv[1][2];
          histfname = hist;
        }
        else {
          char hist[] = "./misID_histos/histosHNLxx_mu_misID.root";
          hist[24] = argv[1][1];
          hist[25] = argv[1][2];
          histfname = hist;
        }
        char data[] = "/scr/bay/pion_hepmcout_HNLxx_mu.dat";
        //char data[] = "../../Whizard/eeZnn.hepmc"; //Test with Olivers eeZnn
        data[26] = argv[1][1];
        data[27] = argv[1][2];
        dataPath = data;
      }
    } else {
      printf("Wrong option. Stopping\n");
      return 0;
    }

  }

  ////////////////////////////////////////////////////////////////////////////
  // WITHOUT pion misID:
  ////////////////////////////////////////////////////////////////////////////
  // Background ////////////////////////////////////////////////////////
  // Z -> qq
  if (misID == false) {
    if ( strcmp(argv[1],"Z")==0 ) {
      histfname = "histosZ.root"; ///////////
      if (detector) {
        histfname = "histosZ_det.root";
      }
      dataPath = "/scr/bay/hepmcout_Zqq.dat";

      // Tau Tau
    } else if ( strcmp(argv[1],"Ztautau")==0 ) {
      histfname = "histosZtautau.root";
      dataPath = "/scr/bay/hepmcout_Ztautau.dat";

    // mu mu
    } else if ( strcmp(argv[1],"Zmumu")==0 ) { 
      histfname = "histosZmumu.root";
      dataPath = "/scr/bay/hepmcout_Zmumu.dat";

      // four fermion process
    } else if ( strcmp(argv[1],"4f")==0 ) {
      histfname = "histos_whiz_4fermion.root";
      dataPath = "/scr/bay/w2p2hepmcout_4fermion.dat";

      // two photon processes:
      // ee->eeuU
    } else if ( strcmp(argv[1],"2phot_uU")==0 ) {
      histfname = "histos_whiz_2phot_uU.root";
      dataPath = "/scr/bay/w2p2hepmcout_2phot_uU.dat";

      // ee->eedD
    } else if ( strcmp(argv[1],"2phot_dD")==0 ) {
      histfname = "histos_whiz_2phot_dD.root";
      dataPath = "/scr/bay/w2p2hepmcout_2phot_dD.dat";

      // ee->eecC
    } else if ( strcmp(argv[1],"2phot_cC")==0 ) {
      dataPath = "/scr/bay/w2p2hepmcout_2phot_cC.dat";

      histfname = "histos_whiz_2phot_cC.root";
      // ee->eesS
    } else if ( strcmp(argv[1],"2phot_sS")==0 ) {
      histfname = "histos_whiz_2phot_sS.root";
      dataPath = "/scr/bay/w2p2hepmcout_2phot_sS.dat";

      // ee->eebB
    } else if ( strcmp(argv[1],"2phot_bB")==0 ) {
      histfname = "histos_whiz_2phot_bB.root";
      dataPath = "/scr/bay/w2p2hepmcout_2phot_bB.dat";


      // Signal ///////////////////////////////////////////////////////////
    } else if ( argv[1][0] == 'N' ) {
      // New Signal 
      if (NewSignal){
        if (detector) {
          char hist[] = "histosHNLxx_mu_det_NewSignal.root";
          hist[9] = argv[1][1];
          hist[10] = argv[1][2];
          histfname = hist;
        }
        else {
          char hist[] = "histosHNLxx_mu_NewSignal.root";
          hist[9] = argv[1][1];
          hist[10] = argv[1][2];
          histfname = hist;
        }
        char data[] = "/scr/bay/newSignal_hepmcout_HNLxx_mu.dat";
        //char data[] = "../../Whizard/eeZnn.hepmc"; //Test with Olivers eeZnn
        data[31] = argv[1][1];
        data[32] = argv[1][2];
        dataPath = data;
      } 
      //Old Signal
      if (NewSignal == false){
        if (detector) {
          char hist[] = "histosHNLxx_mu_det.root";
          hist[9] = argv[1][1];
          hist[10] = argv[1][2];
          histfname = hist;
        }
        else {
          char hist[] = "histosHNLxx_mu.root";
          hist[9] = argv[1][1];
          hist[10] = argv[1][2];
          histfname = hist;
        }
        char data[] = "/scr/bay/hepmcout_HNLxx_mu.dat";
        //char data[] = "../../Whizard/eeZnn.hepmc"; //Test with Olivers eeZnn
        data[21] = argv[1][1];
        data[22] = argv[1][2];
        dataPath = data;
      }
    } else {
      printf("Wrong option. Stopping\n");
      return 0;
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////
  // Define Histograms

  TFile *histFile = new TFile(histfname,"RECREATE");

  TH1F* h_Enu = new TH1F("h_Enu","E_{nu} ;E_{nu} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_Eunseen = new TH1F("h_Eunseen","E_{unseen} ;E_{unseen} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_nPart = new TH1F("h_nPart","#Particles ;#Particles ;Events / 1",120,0.,120.);
  TH1F* h_nCharg = new TH1F("h_nCharg","#Charged ;#Charged ;Events / 1",60,0.,60.);
  TH1F* h_ETot = new TH1F("h_ETot","E_{tot} ;E_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_pzTot = new TH1F("h_pzTot","pz_{tot} ;pz_{tot} [GeV] ;Events / 1 GeV",100,-50.,50.);
  TH1F* h_nChargSeen = new TH1F("h_nChargSeen","#Charged seen ;#Charged seen ;Events / 1",60,0.,60.);
  TH1F* h_nLepton = new TH1F("h_nLepton","# Muons ;# Muons ;Events / 1",7,0.,7.);

  TH1F* h_pMiss = new TH1F("h_pMiss","#slash{p} ; #slash{p} [GeV] ;Events / 1 GeV",60,0.,60.);
  TH1F* h_pMiss_absCost = new TH1F("h_pMiss_absCost"," |cos(#theta)_{#slash{p}}| ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_pTMiss = new TH1F("h_pTMiss","#slash{p}_{T} ;#slash{p}_{T} [GeV] ;Events / 1 GeV",50,0.,50.);
  TH1F* h_pSum = new TH1F("h_pSum","p_{sum} ;p_{sum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_ESum = new TH1F("h_ESum","E_{sum} ;E_{sum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_mtot = new TH1F("h_mtot","m_{tot} ;m_{tot} [GeV] ;Events / 1 GeV",150,0.,150.);
  TH1F* h_EMiss = new TH1F("h_EMiss","#slash{E} ;#slash{E} [GeV] ;Events / 1 GeV",60,0.,60.);
  TH1F* h_mMiss = new TH1F("h_mMiss","#slash{m} ;#slash{m} [GeV] ;Events / 1 GeV",60,0.,60.);

  TH2F* h2_mtot_pMiss = new TH2F("h2_mtot_pMiss", "m_{tot} vs #slash{p} ;m_{tot} [GeV] ;#slash{p} [GeV] ", 100,0.,100., 60,0.,60.);

  // 2 jets
  TH1F* h_ymerge = new TH1F("h_ymerge","ymerge ;ymerge ;Events / 0.002",100,0.,0.2);
  TH1F* h_jet_nCharg_min = new TH1F("h_jet_nCharg_min","jet_nCharg ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_jet_nCharg_max = new TH1F("h_jet_nCharg_max","jet_nCharg ;nCharg_{jet,max} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_jetMass_min = new TH1F("h_jetMass_min","jetMass ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_jetMass_max = new TH1F("h_jetMass_max","jetMass ;m_{jet,max} [GeV] ;Events / 1 GeV",60,0.,60.);
  TH1F* h_jetSumMass = new TH1F("h_jetSumMass","jetSumMass ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_jetSumP = new TH1F("h_jetSumP", "jetSum Momentum ;p_{jetSum} [GeV] ;Events,", 100, 0, 100.);
  TH1F* h_jetE_min = new TH1F("h_jetE_min","jetE_min ;E_{min,jet} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_jetE_max = new TH1F("h_jetE_max","jetE_max ;E_{max,jet} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cos_jj = new TH1F("h_cos_jj","cos(j,j) ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cos_pMissLep = new TH1F("h_cos_pMissLep","cos(#slash{p},#mu) ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cos_jlep_min = new TH1F("h_cos_jlep_min","cos(j,#mu)_{min} ;cos(j,#mu)_{min} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cos_jlep_max = new TH1F("h_cos_jlep_max","cos(j,#mu)_{max} ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cos_jpMiss_min = new TH1F("h_cos_jpMiss_min","cos(j,#slash{p})_{min} ;cos(j,#slash{p})_{min} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cos_jpMiss_max = new TH1F("h_cos_jpMiss_max","cos(j,#slash{p})_{max} ;cos(j,#slash{p})_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_jet_absCost = new TH1F("h_jet_absCost","|cos(#theta)_{jet}| ;|cos(#theta)_{jet}| ;Events / 0.01",100,0.,1.);

  // 3 jets
  TH1F* h_ymerge_3 = new TH1F("h_ymerge_3","ymerge for 3 forced jets ;ymerge_{3} ;Events / 0.002",100,0.,0.2);
  TH1F* h_jet_nCharg_min_3 = new TH1F("h_jet_nCharg_min_3","jet_nCharg ;nCharg_{jet,min_3} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_jet_nCharg_max_3 = new TH1F("h_jet_nCharg_max_3","jet_nCharg ;nCharg_{jet,max_3} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_jetMass_3 = new TH1F("h_jetMass_3","jetMass for 3 forced jets ;m_{jet3} [GeV] ;Events / 1 GeV",60,0.,60.);
  TH1F* h_jetSumMass_3 = new TH1F("h_jetSumMass_3","jetSumMass for 3 forced jets ;m_{jet3Sum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_jetE_min_3 = new TH1F("h_jetE_min_3","jetE_min for 3 forced jets ;E_{min,jet3} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_jetE_max_3 = new TH1F("h_jetE_max_3","jetE_max for 3 forced jets ;E_{max,jet3} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cos_jj_min_3 = new TH1F("h_cos_jj_min_3","cos(j,j)_{3,min} ;cos(j,j)_{3,min} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cos_jj_max_3 = new TH1F("h_cos_jj_max_3","cos(j,j)_{3,max} ;cos(j,j)_{3,max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cos_jlep_min_3 = new TH1F("h_cos_jlep_min_3","cos(j,#mu)_{3,min} ;cos(j,#mu)_{3,min} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cos_jlep_max_3 = new TH1F("h_cos_jlep_max_3","cos(j,#mu)_{max} ;cos(j,#mu)_{3,max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cos_jpMiss_min_3 = new TH1F("h_cos_jpMiss_min_3","cos(j,#slash{p})_{3,min} ;cos(j,#slash{p})_{3,min} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cos_jpMiss_max_3 = new TH1F("h_cos_jpMiss_max_3","cos(j,#slash{p})_{3,max} ;cos(j,#slash{p})_{3,max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_thetaSum_jj_3 = new TH1F("h_thetaSum_jj_3", "#theta_{sum_jj_3} ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);

  // CUT-FLOW ///////////////////////////////////////////
  // cuts from pythia:
  // 1) min 3 charged particles
  // 2) min 1 lepton w/ p > 3GeV
  // 3) pT > 5GeV

  TH1F* h_cutFlow = new TH1F("h_cutFlow","cutFlow ;cutFlow ;Events / 1",30,0.,30.);
  TH1F* h_testCount = new TH1F("h_testCount","testCount ; testCount ;Events / 1 GeV",12,0.,12.);


  TH1F* h_cut0_nLepton = new TH1F("h_cut0_nLepton","# Muons ;# Muons ;Events / 1",7,0.,7.);
  TH1F* h_cut0_nLepton_cut_true = new TH1F("h_cut0_nLepton_cut_true","# trueMuons with p > 3GeV ;# #mu _{true} ;Events / 1",7,0.,7.);
  TH1F* h_cut0_nLepton_cut_fake = new TH1F("h_cut0_nLepton_cut_fake","# fakeMuons with p > 3GeV ;# #mu _{fake} ;Events / 1",7,0.,7.);
  TH1F* h_cut0_nLepton_cut = new TH1F("h_cut0_nLepton_cut","# Muons with p > 3GeV ;# #mu ;Events / 1",7,0.,7.);
  TH1F* h_cut0_nPion_cut = new TH1F("h_cut0_nPion_cut","# Pions with p > 3GeV ;# Pions_{cut} ;Events / 1",20,0.,20.);

  TH1F* h_cut1_nLepton = new TH1F("h_cut1_nLepton","# Muons ;# Muons ;Events / 1",7,0.,7.);
  TH1F* h_cut1_nLepton_cut_true = new TH1F("h_cut1_nLepton_cut_true","# trueMuons with p > 3GeV ;# #mu _{true} ;Events / 1",7,0.,7.);
  TH1F* h_cut1_nLepton_cut_fake = new TH1F("h_cut1_nLepton_cut_fake","# fakeMuons with p > 3GeV ;# #mu _{fake} ;Events / 1",7,0.,7.);
  TH1F* h_cut1_nLepton_cut = new TH1F("h_cut1_nLepton_cut","# Muons with p > 3GeV ;# #mu ;Events / 1",7,0.,7.);
  TH1F* h_cut1_nPion_cut = new TH1F("h_cut1_nPion_cut","# Pions with p > 3GeV ;# Pions_{cut} ;Events / 1",20,0.,20.);
  TH1F* h_cut1_pMiss = new TH1F("h_cut1_pMiss","#slash{p}, cut1 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut1_pMiss_absCost = new TH1F("h_cut1_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut1 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut1_cos_pMissLep = new TH1F("h_cut1_cos_pMissLep","cos(#slash{p},#mu), cut1 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut1_mtot = new TH1F("h_cut1_mtot","m_{tot} [GeV], cut1 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut1_nChargSeen = new TH1F("h_cut1_nChargSeen","#charged seen, cut1 ;#charged seen ;Events / 1",60,0.,60.);

  TH1F* h_cut2_nLepton = new TH1F("h_cut2_nLepton","# Muons ;# Muons ;Events / 1",7,0.,7.);
  TH1F* h_cut2_nLepton_cut_true = new TH1F("h_cut2_nLepton_cut_true","# trueMuons with p > 3GeV ;# #mu _{true} ;Events / 1",7,0.,7.);
  TH1F* h_cut2_nLepton_cut_fake = new TH1F("h_cut2_nLepton_cut_fake","# fakeMuons with p > 3GeV ;# #mu _{fake} ;Events / 1",7,0.,7.);
  TH1F* h_cut2_nLepton_cut = new TH1F("h_cut2_nLepton_cut","# Muons with p > 3GeV ;# #mu ;Events / 1",7,0.,7.);
  TH1F* h_cut2_nPion_cut = new TH1F("h_cut2_nPion_cut","# Pions with p > 3GeV ;# Pions_{cut} ;Events / 1",20,0.,20.);
  TH1F* h_cut2_pMiss = new TH1F("h_cut2_pMiss","#slash{p}, cut2 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut2_pMiss_absCost = new TH1F("h_cut2_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut2 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut2_cos_pMissLep = new TH1F("h_cut2_cos_pMissLep","cos(#slash{p},#mu), cut2 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut2_mtot = new TH1F("h_cut2_mtot","m_{tot} [GeV], cut2 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut2_nChargSeen = new TH1F("h_cut2_nChargSeen","#charged seen, cut2 ;#charged seen ;Events / 1",60,0.,60.);

  TH1F* h_cut3_nLepton = new TH1F("h_cut3_nLepton","# Muons ;# Muons ;Events / 1",7,0.,7.);
  TH1F* h_cut3_nLepton_cut_true = new TH1F("h_cut3_nLepton_cut_true","# trueMuons with p > 3GeV ;# #mu _{true} ;Events / 1",7,0.,7.);
  TH1F* h_cut3_nLepton_cut_fake = new TH1F("h_cut3_nLepton_cut_fake","# fakeMuons with p > 3GeV ;# #mu _{fake} ;Events / 1",7,0.,7.);
  TH1F* h_cut3_nLepton_cut = new TH1F("h_cut3_nLepton_cut","# Muons with p > 3GeV ;# #mu ;Events / 1",7,0.,7.);
  TH1F* h_cut3_nPion_cut = new TH1F("h_cut3_nPion_cut","# Pions with p > 3GeV ;# Pions_{cut} ;Events / 1",20,0.,20.);
  TH1F* h_cut3_pMiss = new TH1F("h_cut3_pMiss","#slash{p}, cut3 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut3_pMiss_absCost = new TH1F("h_cut3_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut3 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut3_cos_pMissLep = new TH1F("h_cut3_cos_pMissLep","cos(#slash{p},#mu), cut3 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut3_mtot = new TH1F("h_cut3_mtot","m_{tot} [GeV], cut3 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut3_nChargSeen = new TH1F("h_cut3_nChargSeen","#charged seen, cut3 ;#charged seen ;Events / 1",60,0.,60.);

  TH1F* h_cut4_pMiss = new TH1F("h_cut4_pMiss","#slash{p}, cut4 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut4_pMiss_absCost = new TH1F("h_cut4_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut4 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut4_cos_pMissLep = new TH1F("h_cut4_cos_pMissLep","cos(#slash{p},#mu), cut4 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut4_mtot = new TH1F("h_cut4_mtot","m_{tot} [GeV], cut4 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut4_nChargSeen = new TH1F("h_cut4_nChargSeen","#charged seen, cut4 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut4_ymerge = new TH1F("h_cut4_ymerge","ymerge ;ymerge ;Events / 0.002",100,0.,0.2);
  TH1F* h_cut4_jet_nCharg_min = new TH1F("h_cut4_jet_nCharg_min","jet_nCharg ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut4_jet_nCharg_max = new TH1F("h_cut4_jet_nCharg_max","jet_nCharg ;nCharg_{jet,max} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut4_jetE_min = new TH1F("h_cut4_jetE_min","jetE_min ;E_{min,jet} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut4_jetE_max = new TH1F("h_cut4_jetE_max","jetE_max ;E_{max,jet} [GeV] ;Events / 1 GeV",100,0.,100.);

  TH1F* h_cut5_mtot = new TH1F("h_cut5_mtot","m_{tot} [GeV], cut5 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut5_pMiss = new TH1F("h_cut5_pMiss","#slash{p}, cut5 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut5_nChargSeen = new TH1F("h_cut5_nChargSeen","#charged seen, cut5 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut5_pMiss_absCost = new TH1F("h_cut5_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut5 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut5_cos_pMissLep = new TH1F("h_cut5_cos_pMissLep","cos(#slash{p},#mu), cut5 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut5_jetSumMass = new TH1F("h_cut5_jetSumMass","m_{jetSum} [GeV], cut5 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut5_jetSumP = new TH1F("h_cut5_jetSumP", "p_{jetSum} [GeV], cut5 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut5_jet_nCharg_min = new TH1F("h_cut5_jet_nCharg_min","jet_nCharg, cut5 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut5_jetMass_min = new TH1F("h_cut5_jetMass_min","jetMass_min, cut5 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut5_cos_jj = new TH1F("h_cut5_cos_jj","cos(j,j), cut5 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut5_cos_jlep_max = new TH1F("h_cut5_cos_jlep_max","cos(j,#mu)_{max}, cut5 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut5_cos_jlep_max_3 = new TH1F("h_cut5_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut5 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut5_cos_jlep_max_NOT3 = new TH1F("h_cut5_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut5 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut5_thetaSum_jj_3 = new TH1F("h_cut5_thetaSum_jj_3", "#theta_{sum_jj_3} , cut5 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);

  TH1F* h_cut6_mtot = new TH1F("h_cut6_mtot","m_{tot} [GeV], cut6 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut6_pMiss = new TH1F("h_cut6_pMiss","#slash{p}, cut6 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut6_nChargSeen = new TH1F("h_cut6_nChargSeen","#charged seen, cut6 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut6_pMiss_absCost = new TH1F("h_cut6_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut6 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut6_cos_pMissLep = new TH1F("h_cut6_cos_pMissLep","cos(#slash{p},#mu), cut6 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut6_jetSumMass = new TH1F("h_cut6_jetSumMass","m_{jetSum} [GeV], cut6 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut6_jetSumP = new TH1F("h_cut6_jetSumP", "p_{jetSum} [GeV], cut6 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut6_jet_nCharg_min = new TH1F("h_cut6_jet_nCharg_min","jet_nCharg, cut6 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut6_jetMass_min = new TH1F("h_cut6_jetMass_min","jetMass_min, cut6 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut6_cos_jj = new TH1F("h_cut6_cos_jj","cos(j,j), cut6 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut6_cos_jlep_max = new TH1F("h_cut6_cos_jlep_max","cos(j,#mu)_{max}, cut6 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut6_cos_jlep_max_3 = new TH1F("h_cut6_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut6 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut6_cos_jlep_max_NOT3 = new TH1F("h_cut6_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut6 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut6_thetaSum_jj_3 = new TH1F("h_cut6_thetaSum_jj_3", "#theta_{sum_jj_3} , cut6 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);

  TH1F* h_cut7_mtot = new TH1F("h_cut7_mtot","m_{tot} [GeV], cut7 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut7_pMiss = new TH1F("h_cut7_pMiss","#slash{p}, cut7 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut7_nChargSeen = new TH1F("h_cut7_nChargSeen","#charged seen, cut7 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut7_pMiss_absCost = new TH1F("h_cut7_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut7 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut7_cos_pMissLep = new TH1F("h_cut7_cos_pMissLep","cos(#slash{p},#mu), cut7 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut7_jetSumMass = new TH1F("h_cut7_jetSumMass","m_{jetSum} [GeV], cut7 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut7_jetSumP = new TH1F("h_cut7_jetSumP", "p_{jetSum} [GeV], cut7 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut7_jet_nCharg_min = new TH1F("h_cut7_jet_nCharg_min","jet_nCharg, cut7 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut7_jetMass_min = new TH1F("h_cut7_jetMass_min","jetMass_min, cut7 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut7_cos_jj = new TH1F("h_cut7_cos_jj","cos(j,j), cut7 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut7_cos_jlep_max = new TH1F("h_cut7_cos_jlep_max","cos(j,#mu)_{max}, cut7 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut7_cos_jlep_max_3 = new TH1F("h_cut7_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut7 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut7_cos_jlep_max_NOT3 = new TH1F("h_cut7_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut7 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut7_thetaSum_jj_3 = new TH1F("h_cut7_thetaSum_jj_3", "#theta_{sum_jj_3} , cut7 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);
  
  TH1F* h_cut8_mtot = new TH1F("h_cut8_mtot","m_{tot} [GeV], cut8 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut8_pMiss = new TH1F("h_cut8_pMiss","#slash{p}, cut8 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut8_nChargSeen = new TH1F("h_cut8_nChargSeen","#charged seen, cut8 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut8_pMiss_absCost = new TH1F("h_cut8_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut8 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut8_cos_pMissLep = new TH1F("h_cut8_cos_pMissLep","cos(#slash{p},#mu), cut8 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut8_jetSumMass = new TH1F("h_cut8_jetSumMass","m_{jetSum} [GeV], cut8 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut8_jetSumP = new TH1F("h_cut8_jetSumP", "p_{jetSum} [GeV], cut8 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut8_jet_nCharg_min = new TH1F("h_cut8_jet_nCharg_min","jet_nCharg, cut8 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut8_jetMass_min = new TH1F("h_cut8_jetMass_min","jetMass_min, cut8 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut8_cos_jj = new TH1F("h_cut8_cos_jj","cos(j,j), cut8 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut8_cos_jlep_max = new TH1F("h_cut8_cos_jlep_max","cos(j,#mu)_{max}, cut8 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut8_cos_jlep_max_3 = new TH1F("h_cut8_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut8 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut8_cos_jlep_max_NOT3 = new TH1F("h_cut8_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut8 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut8_thetaSum_jj_3 = new TH1F("h_cut8_thetaSum_jj_3", "#theta_{sum_jj_3} , cut8 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);

  TH1F* h_cut9_mtot = new TH1F("h_cut9_mtot","m_{tot} [GeV], cut9 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut9_pMiss = new TH1F("h_cut9_pMiss","#slash{p}, cut9 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut9_nChargSeen = new TH1F("h_cut9_nChargSeen","#charged seen, cut9 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut9_pMiss_absCost = new TH1F("h_cut9_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut9 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut9_cos_pMissLep = new TH1F("h_cut9_cos_pMissLep","cos(#slash{p},#mu), cut9 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut9_jetSumMass = new TH1F("h_cut9_jetSumMass","m_{jetSum} [GeV], cut9 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut9_jetSumP = new TH1F("h_cut9_jetSumP", "p_{jetSum} [GeV], cut9 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut9_jet_nCharg_min = new TH1F("h_cut9_jet_nCharg_min","jet_nCharg, cut9 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut9_jetMass_min = new TH1F("h_cut9_jetMass_min","jetMass_min, cut9 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut9_cos_jj = new TH1F("h_cut9_cos_jj","cos(j,j), cut9 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut9_cos_jlep_max = new TH1F("h_cut9_cos_jlep_max","cos(j,#mu)_{max}, cut9 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut9_cos_jlep_max_3 = new TH1F("h_cut9_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut9 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut9_cos_jlep_max_NOT3 = new TH1F("h_cut9_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut9 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut9_thetaSum_jj_3 = new TH1F("h_cut9_thetaSum_jj_3", "#theta_{sum_jj_3} , cut9 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);
  

  // Sliding cuts:
  TH1F* h_cut10_N30_mtot = new TH1F("h_cut10_N30_mtot","m_{tot} [GeV], cut10_N30 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N30_pMiss = new TH1F("h_cut10_N30_pMiss","#slash{p}, cut10_N30 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut10_N30_nChargSeen = new TH1F("h_cut10_N30_nChargSeen","#charged seen, cut10_N30 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut10_N30_pMiss_absCost = new TH1F("h_cut10_N30_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut10_N30 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut10_N30_cos_pMissLep = new TH1F("h_cut10_N30_cos_pMissLep","cos(#slash{p},#mu), cut10_N30 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N30_jetSumMass = new TH1F("h_cut10_N30_jetSumMass","m_{jetSum} [GeV], cut10_N30 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N30_jetSumP = new TH1F("h_cut10_N30_jetSumP", "p_{jetSum} [GeV], cut10_N30 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut10_N30_jet_nCharg_min = new TH1F("h_cut10_N30_jet_nCharg_min","jet_nCharg, cut10_N30 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut10_N30_jetMass_min = new TH1F("h_cut10_N30_jetMass_min","jetMass_min, cut10_N30 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut10_N30_cos_jj = new TH1F("h_cut10_N30_cos_jj","cos(j,j), cut10_N30 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N30_cos_jlep_max = new TH1F("h_cut10_N30_cos_jlep_max","cos(j,#mu)_{max}, cut10_N30 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N30_cos_jlep_max_3 = new TH1F("h_cut10_N30_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut10_N30 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N30_cos_jlep_max_NOT3 = new TH1F("h_cut10_N30_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut10_N30 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N30_thetaSum_jj_3 = new TH1F("h_cut10_N30_thetaSum_jj_3", "#theta_{sum_jj_3} , cut10_N30 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);
  
  TH1F* h_cut11_N30_mtot = new TH1F("h_cut11_N30_mtot","m_{tot} [GeV], cut11_N30 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N30_pMiss = new TH1F("h_cut11_N30_pMiss","#slash{p}, cut11_N30 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut11_N30_nChargSeen = new TH1F("h_cut11_N30_nChargSeen","#charged seen, cut11_N30 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut11_N30_pMiss_absCost = new TH1F("h_cut11_N30_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut11_N30 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut11_N30_cos_pMissLep = new TH1F("h_cut11_N30_cos_pMissLep","cos(#slash{p},#mu), cut11_N30 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N30_jetSumMass = new TH1F("h_cut11_N30_jetSumMass","m_{jetSum} [GeV], cut11_N30 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N30_jetSumP = new TH1F("h_cut11_N30_jetSumP", "p_{jetSum} [GeV], cut11_N30 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut11_N30_jet_nCharg_min = new TH1F("h_cut11_N30_jet_nCharg_min","jet_nCharg, cut11_N30 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut11_N30_jetMass_min = new TH1F("h_cut11_N30_jetMass_min","jetMass_min, cut11_N30 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut11_N30_cos_jj = new TH1F("h_cut11_N30_cos_jj","cos(j,j), cut11_N30 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N30_cos_jlep_max = new TH1F("h_cut11_N30_cos_jlep_max","cos(j,#mu)_{max}, cut11_N30 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N30_cos_jlep_max_3 = new TH1F("h_cut11_N30_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut11_N30 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N30_cos_jlep_max_NOT3 = new TH1F("h_cut11_N30_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut11_N30 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N30_thetaSum_jj_3 = new TH1F("h_cut11_N30_thetaSum_jj_3", "#theta_{sum_jj_3} , cut11_N30 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);


  TH1F* h_cut10_N40_mtot = new TH1F("h_cut10_N40_mtot","m_{tot} [GeV], cut10_N40 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N40_pMiss = new TH1F("h_cut10_N40_pMiss","#slash{p}, cut10_N40 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut10_N40_nChargSeen = new TH1F("h_cut10_N40_nChargSeen","#charged seen, cut10_N40 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut10_N40_pMiss_absCost = new TH1F("h_cut10_N40_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut10_N40 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut10_N40_cos_pMissLep = new TH1F("h_cut10_N40_cos_pMissLep","cos(#slash{p},#mu), cut10_N40 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N40_jetSumMass = new TH1F("h_cut10_N40_jetSumMass","m_{jetSum} [GeV], cut10_N40 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N40_jetSumP = new TH1F("h_cut10_N40_jetSumP", "p_{jetSum} [GeV], cut10_N40 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut10_N40_jet_nCharg_min = new TH1F("h_cut10_N40_jet_nCharg_min","jet_nCharg, cut10_N40 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut10_N40_jetMass_min = new TH1F("h_cut10_N40_jetMass_min","jetMass_min, cut10_N40 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut10_N40_cos_jj = new TH1F("h_cut10_N40_cos_jj","cos(j,j), cut10_N40 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N40_cos_jlep_max = new TH1F("h_cut10_N40_cos_jlep_max","cos(j,#mu)_{max}, cut10_N40 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N40_cos_jlep_max_3 = new TH1F("h_cut10_N40_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut10_N40 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N40_cos_jlep_max_NOT3 = new TH1F("h_cut10_N40_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut10_N40 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N40_thetaSum_jj_3 = new TH1F("h_cut10_N40_thetaSum_jj_3", "#theta_{sum_jj_3} , cut10_N40 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);
  
  TH1F* h_cut11_N40_mtot = new TH1F("h_cut11_N40_mtot","m_{tot} [GeV], cut11_N40 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N40_pMiss = new TH1F("h_cut11_N40_pMiss","#slash{p}, cut11_N40 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut11_N40_nChargSeen = new TH1F("h_cut11_N40_nChargSeen","#charged seen, cut11_N40 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut11_N40_pMiss_absCost = new TH1F("h_cut11_N40_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut11_N40 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut11_N40_cos_pMissLep = new TH1F("h_cut11_N40_cos_pMissLep","cos(#slash{p},#mu), cut11_N40 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N40_jetSumMass = new TH1F("h_cut11_N40_jetSumMass","m_{jetSum} [GeV], cut11_N40 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N40_jetSumP = new TH1F("h_cut11_N40_jetSumP", "p_{jetSum} [GeV], cut11_N40 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut11_N40_jet_nCharg_min = new TH1F("h_cut11_N40_jet_nCharg_min","jet_nCharg, cut11_N40 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut11_N40_jetMass_min = new TH1F("h_cut11_N40_jetMass_min","jetMass_min, cut11_N40 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut11_N40_cos_jj = new TH1F("h_cut11_N40_cos_jj","cos(j,j), cut11_N40 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N40_cos_jlep_max = new TH1F("h_cut11_N40_cos_jlep_max","cos(j,#mu)_{max}, cut11_N40 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N40_cos_jlep_max_3 = new TH1F("h_cut11_N40_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut11_N40 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N40_cos_jlep_max_NOT3 = new TH1F("h_cut11_N40_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut11_N40 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N40_thetaSum_jj_3 = new TH1F("h_cut11_N40_thetaSum_jj_3", "#theta_{sum_jj_3} , cut11_N40 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);


  TH1F* h_cut10_N50_mtot = new TH1F("h_cut10_N50_mtot","m_{tot} [GeV], cut10_N50 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N50_pMiss = new TH1F("h_cut10_N50_pMiss","#slash{p}, cut10_N50 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut10_N50_nChargSeen = new TH1F("h_cut10_N50_nChargSeen","#charged seen, cut10_N50 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut10_N50_pMiss_absCost = new TH1F("h_cut10_N50_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut10_N50 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut10_N50_cos_pMissLep = new TH1F("h_cut10_N50_cos_pMissLep","cos(#slash{p},#mu), cut10_N50 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N50_jetSumMass = new TH1F("h_cut10_N50_jetSumMass","m_{jetSum} [GeV], cut10_N50 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N50_jetSumP = new TH1F("h_cut10_N50_jetSumP", "p_{jetSum} [GeV], cut10_N50 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut10_N50_jet_nCharg_min = new TH1F("h_cut10_N50_jet_nCharg_min","jet_nCharg, cut10_N50 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut10_N50_jetMass_min = new TH1F("h_cut10_N50_jetMass_min","jetMass_min, cut10_N50 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut10_N50_cos_jj = new TH1F("h_cut10_N50_cos_jj","cos(j,j), cut10_N50 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N50_cos_jlep_max = new TH1F("h_cut10_N50_cos_jlep_max","cos(j,#mu)_{max}, cut10_N50 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N50_cos_jlep_max_3 = new TH1F("h_cut10_N50_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut10_N50 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N50_cos_jlep_max_NOT3 = new TH1F("h_cut10_N50_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut10_N50 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N50_thetaSum_jj_3 = new TH1F("h_cut10_N50_thetaSum_jj_3", "#theta_{sum_jj_3} , cut10_N50 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);
  
  TH1F* h_cut11_N50_mtot = new TH1F("h_cut11_N50_mtot","m_{tot} [GeV], cut11_N50 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N50_pMiss = new TH1F("h_cut11_N50_pMiss","#slash{p}, cut11_N50 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut11_N50_nChargSeen = new TH1F("h_cut11_N50_nChargSeen","#charged seen, cut11_N50 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut11_N50_pMiss_absCost = new TH1F("h_cut11_N50_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut11_N50 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut11_N50_cos_pMissLep = new TH1F("h_cut11_N50_cos_pMissLep","cos(#slash{p},#mu), cut11_N50 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N50_jetSumMass = new TH1F("h_cut11_N50_jetSumMass","m_{jetSum} [GeV], cut11_N50 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N50_jetSumP = new TH1F("h_cut11_N50_jetSumP", "p_{jetSum} [GeV], cut11_N50 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut11_N50_jet_nCharg_min = new TH1F("h_cut11_N50_jet_nCharg_min","jet_nCharg, cut11_N50 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut11_N50_jetMass_min = new TH1F("h_cut11_N50_jetMass_min","jetMass_min, cut11_N50 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut11_N50_cos_jj = new TH1F("h_cut11_N50_cos_jj","cos(j,j), cut11_N50 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N50_cos_jlep_max = new TH1F("h_cut11_N50_cos_jlep_max","cos(j,#mu)_{max}, cut11_N50 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N50_cos_jlep_max_3 = new TH1F("h_cut11_N50_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut11_N50 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N50_cos_jlep_max_NOT3 = new TH1F("h_cut11_N50_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut11_N50 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N50_thetaSum_jj_3 = new TH1F("h_cut11_N50_thetaSum_jj_3", "#theta_{sum_jj_3} , cut11_N50 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);


  TH1F* h_cut10_N60_mtot = new TH1F("h_cut10_N60_mtot","m_{tot} [GeV], cut10_N60 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N60_pMiss = new TH1F("h_cut10_N60_pMiss","#slash{p}, cut10_N60 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut10_N60_nChargSeen = new TH1F("h_cut10_N60_nChargSeen","#charged seen, cut10_N60 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut10_N60_pMiss_absCost = new TH1F("h_cut10_N60_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut10_N60 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut10_N60_cos_pMissLep = new TH1F("h_cut10_N60_cos_pMissLep","cos(#slash{p},#mu), cut10_N60 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N60_jetSumMass = new TH1F("h_cut10_N60_jetSumMass","m_{jetSum} [GeV], cut10_N60 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N60_jetSumP = new TH1F("h_cut10_N60_jetSumP", "p_{jetSum} [GeV], cut10_N60 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut10_N60_jet_nCharg_min = new TH1F("h_cut10_N60_jet_nCharg_min","jet_nCharg, cut10_N60 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut10_N60_jetMass_min = new TH1F("h_cut10_N60_jetMass_min","jetMass_min, cut10_N60 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut10_N60_cos_jj = new TH1F("h_cut10_N60_cos_jj","cos(j,j), cut10_N60 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N60_cos_jlep_max = new TH1F("h_cut10_N60_cos_jlep_max","cos(j,#mu)_{max}, cut10_N60 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N60_cos_jlep_max_3 = new TH1F("h_cut10_N60_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut10_N60 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N60_cos_jlep_max_NOT3 = new TH1F("h_cut10_N60_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut10_N60 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N60_thetaSum_jj_3 = new TH1F("h_cut10_N60_thetaSum_jj_3", "#theta_{sum_jj_3} , cut10_N60 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);
  
  TH1F* h_cut11_N60_mtot = new TH1F("h_cut11_N60_mtot","m_{tot} [GeV], cut11_N60 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N60_pMiss = new TH1F("h_cut11_N60_pMiss","#slash{p}, cut11_N60 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut11_N60_nChargSeen = new TH1F("h_cut11_N60_nChargSeen","#charged seen, cut11_N60 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut11_N60_pMiss_absCost = new TH1F("h_cut11_N60_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut11_N60 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut11_N60_cos_pMissLep = new TH1F("h_cut11_N60_cos_pMissLep","cos(#slash{p},#mu), cut11_N60 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N60_jetSumMass = new TH1F("h_cut11_N60_jetSumMass","m_{jetSum} [GeV], cut11_N60 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N60_jetSumP = new TH1F("h_cut11_N60_jetSumP", "p_{jetSum} [GeV], cut11_N60 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut11_N60_jet_nCharg_min = new TH1F("h_cut11_N60_jet_nCharg_min","jet_nCharg, cut11_N60 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut11_N60_jetMass_min = new TH1F("h_cut11_N60_jetMass_min","jetMass_min, cut11_N60 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut11_N60_cos_jj = new TH1F("h_cut11_N60_cos_jj","cos(j,j), cut11_N60 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N60_cos_jlep_max = new TH1F("h_cut11_N60_cos_jlep_max","cos(j,#mu)_{max}, cut11_N60 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N60_cos_jlep_max_3 = new TH1F("h_cut11_N60_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut11_N60 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N60_cos_jlep_max_NOT3 = new TH1F("h_cut11_N60_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut11_N60 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N60_thetaSum_jj_3 = new TH1F("h_cut11_N60_thetaSum_jj_3", "#theta_{sum_jj_3} , cut11_N60 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);


  TH1F* h_cut10_N70_mtot = new TH1F("h_cut10_N70_mtot","m_{tot} [GeV], cut10_N70 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N70_pMiss = new TH1F("h_cut10_N70_pMiss","#slash{p}, cut10_N70 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut10_N70_nChargSeen = new TH1F("h_cut10_N70_nChargSeen","#charged seen, cut10_N70 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut10_N70_pMiss_absCost = new TH1F("h_cut10_N70_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut10_N70 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut10_N70_cos_pMissLep = new TH1F("h_cut10_N70_cos_pMissLep","cos(#slash{p},#mu), cut10_N70 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N70_jetSumMass = new TH1F("h_cut10_N70_jetSumMass","m_{jetSum} [GeV], cut10_N70 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N70_jetSumP = new TH1F("h_cut10_N70_jetSumP", "p_{jetSum} [GeV], cut10_N70 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut10_N70_jet_nCharg_min = new TH1F("h_cut10_N70_jet_nCharg_min","jet_nCharg, cut10_N70 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut10_N70_jetMass_min = new TH1F("h_cut10_N70_jetMass_min","jetMass_min, cut10_N70 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut10_N70_cos_jj = new TH1F("h_cut10_N70_cos_jj","cos(j,j), cut10_N70 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N70_cos_jlep_max = new TH1F("h_cut10_N70_cos_jlep_max","cos(j,#mu)_{max}, cut10_N70 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N70_cos_jlep_max_3 = new TH1F("h_cut10_N70_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut10_N70 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N70_cos_jlep_max_NOT3 = new TH1F("h_cut10_N70_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut10_N70 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N70_thetaSum_jj_3 = new TH1F("h_cut10_N70_thetaSum_jj_3", "#theta_{sum_jj_3} , cut10_N70 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);
  
  TH1F* h_cut11_N70_mtot = new TH1F("h_cut11_N70_mtot","m_{tot} [GeV], cut11_N70 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N70_pMiss = new TH1F("h_cut11_N70_pMiss","#slash{p}, cut11_N70 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut11_N70_nChargSeen = new TH1F("h_cut11_N70_nChargSeen","#charged seen, cut11_N70 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut11_N70_pMiss_absCost = new TH1F("h_cut11_N70_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut11_N70 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut11_N70_cos_pMissLep = new TH1F("h_cut11_N70_cos_pMissLep","cos(#slash{p},#mu), cut11_N70 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N70_jetSumMass = new TH1F("h_cut11_N70_jetSumMass","m_{jetSum} [GeV], cut11_N70 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N70_jetSumP = new TH1F("h_cut11_N70_jetSumP", "p_{jetSum} [GeV], cut11_N70 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut11_N70_jet_nCharg_min = new TH1F("h_cut11_N70_jet_nCharg_min","jet_nCharg, cut11_N70 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut11_N70_jetMass_min = new TH1F("h_cut11_N70_jetMass_min","jetMass_min, cut11_N70 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut11_N70_cos_jj = new TH1F("h_cut11_N70_cos_jj","cos(j,j), cut11_N70 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N70_cos_jlep_max = new TH1F("h_cut11_N70_cos_jlep_max","cos(j,#mu)_{max}, cut11_N70 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N70_cos_jlep_max_3 = new TH1F("h_cut11_N70_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut11_N70 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N70_cos_jlep_max_NOT3 = new TH1F("h_cut11_N70_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut11_N70 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N70_thetaSum_jj_3 = new TH1F("h_cut11_N70_thetaSum_jj_3", "#theta_{sum_jj_3} , cut11_N70 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);


  TH1F* h_cut10_N80_mtot = new TH1F("h_cut10_N80_mtot","m_{tot} [GeV], cut10_N80 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N80_pMiss = new TH1F("h_cut10_N80_pMiss","#slash{p}, cut10_N80 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut10_N80_nChargSeen = new TH1F("h_cut10_N80_nChargSeen","#charged seen, cut10_N80 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut10_N80_pMiss_absCost = new TH1F("h_cut10_N80_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut10_N80 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut10_N80_cos_pMissLep = new TH1F("h_cut10_N80_cos_pMissLep","cos(#slash{p},#mu), cut10_N80 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N80_jetSumMass = new TH1F("h_cut10_N80_jetSumMass","m_{jetSum} [GeV], cut10_N80 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut10_N80_jetSumP = new TH1F("h_cut10_N80_jetSumP", "p_{jetSum} [GeV], cut10_N80 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut10_N80_jet_nCharg_min = new TH1F("h_cut10_N80_jet_nCharg_min","jet_nCharg, cut10_N80 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut10_N80_jetMass_min = new TH1F("h_cut10_N80_jetMass_min","jetMass_min, cut10_N80 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut10_N80_cos_jj = new TH1F("h_cut10_N80_cos_jj","cos(j,j), cut10_N80 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N80_cos_jlep_max = new TH1F("h_cut10_N80_cos_jlep_max","cos(j,#mu)_{max}, cut10_N80 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N80_cos_jlep_max_3 = new TH1F("h_cut10_N80_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut10_N80 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N80_cos_jlep_max_NOT3 = new TH1F("h_cut10_N80_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut10_N80 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut10_N80_thetaSum_jj_3 = new TH1F("h_cut10_N80_thetaSum_jj_3", "#theta_{sum_jj_3} , cut10_N80 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);
  
  TH1F* h_cut11_N80_mtot = new TH1F("h_cut11_N80_mtot","m_{tot} [GeV], cut11_N80 ;m_{tot} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N80_pMiss = new TH1F("h_cut11_N80_pMiss","#slash{p}, cut11_N80 ;#slash{p} [GeV];Events / 1 GeV",60,0.,60.);
  TH1F* h_cut11_N80_nChargSeen = new TH1F("h_cut11_N80_nChargSeen","#charged seen, cut11_N80 ;#charged seen ;Events / 1",60,0.,60.);
  TH1F* h_cut11_N80_pMiss_absCost = new TH1F("h_cut11_N80_pMiss_absCost","|cos(#theta)_{#slash{p}}|, cut11_N80 ;|cos(#theta)_{#slash{p}}| ;Events / 0.01",100,0.,1.);
  TH1F* h_cut11_N80_cos_pMissLep = new TH1F("h_cut11_N80_cos_pMissLep","cos(#slash{p},#mu), cut11_N80 ;cos(#slash{p},#mu) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N80_jetSumMass = new TH1F("h_cut11_N80_jetSumMass","m_{jetSum} [GeV], cut11_N80 ;m_{jetSum} [GeV] ;Events / 1 GeV",100,0.,100.);
  TH1F* h_cut11_N80_jetSumP = new TH1F("h_cut11_N80_jetSumP", "p_{jetSum} [GeV], cut11_N80 ;p_{jetSum} [GeV] ;Events / 1 GeV", 100, 0, 100.);
  TH1F* h_cut11_N80_jet_nCharg_min = new TH1F("h_cut11_N80_jet_nCharg_min","jet_nCharg, cut11_N80 ;nCharg_{jet,min} [GeV] ;Events / 1",20,0.,20.);
  TH1F* h_cut11_N80_jetMass_min = new TH1F("h_cut11_N80_jetMass_min","jetMass_min, cut11_N80 ;m_{jet,min} [GeV] ;Events / 1 GeV",25,0.,25.);
  TH1F* h_cut11_N80_cos_jj = new TH1F("h_cut11_N80_cos_jj","cos(j,j), cut11_N80 ;cos(j,j) ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N80_cos_jlep_max = new TH1F("h_cut11_N80_cos_jlep_max","cos(j,#mu)_{max}, cut11_N80 ;cos(j,#mu)_{max} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N80_cos_jlep_max_3 = new TH1F("h_cut11_N80_cos_jlep_max_3","cos(j,#mu)_{max,3}, cut11_N80 ;cos(j,#mu)_{max,3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N80_cos_jlep_max_NOT3 = new TH1F("h_cut11_N80_cos_jlep_max_NOT3","cos(j,#mu)_{max,NOT3}, cut11_N80 ;cos(j,#mu)_{max,NOT3} ;Events / 0.02",100,-1.,1.);
  TH1F* h_cut11_N80_thetaSum_jj_3 = new TH1F("h_cut11_N80_thetaSum_jj_3", "#theta_{sum_jj_3} , cut11_N80 ;#theta _{sum_jj_3} [#circ ];Events / 5#circ", 72, 0., 360.);
  


  ////////////////////////////////////////////////////////////////////////////////////////
  // Fastjet analysis - select algorithm and parameters
  fastjet::JetDefinition *jetDef = NULL;
  jetDef = new fastjet::JetDefinition(fastjet::ee_kt_algorithm);

  // Fastjet input
  fastjet::PseudoJet fjInput;
  std::vector <fastjet::PseudoJet> fjInputs;

  // begin reading ascii_in input file
  HepMC::IO_GenEvent ascii_in( dataPath,std::ios::in );

  // To write Bkg events to HepMC:
  HepMC::IO_GenEvent ascii_io( Bkgfile, std::ios::out );

  ////////////////////////////////////////////////////////////////////////////////////////
  // EVENT LOOP
  ////////////////////////////////////////////////////////////////////////////////////////

  int nEvent=10000;
  int icount=0;
  double eCM = 91.2;



  std::cout << argc << std::endl;
  if ( argc==3 ) {
    sscanf (argv[2],"%d",&nEvent);
  }

  HepMC::GenEvent* evt = 0;
  bool firstEvent = true;

  //  HepMC::ParticleDataTable prt = new ParticleDataTable();

  while ( icount<nEvent ) {
    // delete previous event
    if ( evt ) delete evt;

    // Read next event
    evt = ascii_in.read_next_event();
    if ( !evt ) break;

    icount++;
    if ( icount%1000==1 ) std::cout << "Processing Event Number " << icount
				    << " its # " << evt->event_number() << std::endl;

    ////////////////////////////////////////////////////////////////////////////////////////
    // 1. particle loop //////////////////////////////////////////////////
    int nPart = 0;
    int nCharg = 0;
    int nLepton = 0;
    int nLepton_cut_1 = 0;
    int lep_ip = 0;
    int nPion_cut_1 = 0;
    std::vector<int> vec_pion_ip_1;
    double Eunseen = 0.;
    double Enu = 0.;
    TLorentzVector TL_lep_1 = TLorentzVector(0.,0.,0.,0.);
    TLorentzVector TL_p4Prt_1 = TLorentzVector(0.,0.,0.,0.);
    TLorentzVector TL_p4Sum_1 = TLorentzVector(0.,0.,0.,0.);

    int ip_1 = 0;
    for ( HepMC::GenEvent::particle_const_iterator p = evt->particles_begin();
	  p != evt->particles_end(); ++p, ip_1++){

      int pid = (*p)->pdg_id();
      double px = (*p)->momentum().x();
      double py = (*p)->momentum().y();
      double pz = (*p)->momentum().z();
      double ee = (*p)->momentum().t();
      double pp = sqrt( px*px + py*py + pz*pz );
      double cost = pz/pp;

      // Skip if not a final state particle
      if ( (*p)->status() != 1 )          continue;

      nPart++;
      if ( MyParticles::IsChargedParticle(pid) ) nCharg++;

      // Skip neutrinos
      if ( MyParticles::IsNeutrino(pid) ) {
        Enu += (*p)->momentum().t();
        continue;
      }

      // Skip particles close to beam pipe
      if ( fabs(cost) > 0.99 ) {
        Eunseen += ee;
        continue;
      }

      // Skip soft particles
      if ( pp < 0.1 ) {
        Eunseen += ee;
        continue;
      }


      TL_p4Prt_1.SetPxPyPzE((*p)->momentum().x(),
			    (*p)->momentum().y(),
			    (*p)->momentum().z(),
			    (*p)->momentum().t() );
      TL_p4Sum_1 += TL_p4Prt_1;


      // Find lepton with higest momentum and save pid
      if ( MyParticles::IsMuon(pid) ) {
        nLepton++;
        if ( pp > 3){
          nLepton_cut_1++;
          if ( pp > TL_lep_1.Rho() ) {
            lep_ip = ip_1;
            TL_lep_1.SetPxPyPzE((*p)->momentum().x(),
				                        (*p)->momentum().y(),
				                        (*p)->momentum().z(),
				                        (*p)->momentum().t() );
          }
        }
      }

      // Count pions with p>3GeV and save pid
      if ( MyParticles::IsChargedPion(pid) ) {
        if ( pp > 3.){
          nPion_cut_1++;
          vec_pion_ip_1.push_back(ip_1);
        }
      }


    } // end of 1. loop

    double pT = sqrt( TL_p4Sum_1.Px()*TL_p4Sum_1.Px() + TL_p4Sum_1.Py()*TL_p4Sum_1.Py() );

    ///////////////////////////////////////////////////////////////
    // pion misID:
    int pion_ip_2 = -1;
    if (misID){
      // no leptons in sample -> fake leptons from pre-selection
      if ( nLepton_cut_1 == 0 ) { 
        // choose random pion with p>3GeV
        int i_pion = myRandomArray.Integer(nPion_cut_1);  // pick random pion of N_Pions
        pion_ip_2 = vec_pion_ip_1[ i_pion ]; // find its ip number
      }
    }
    ///////////////////////////////////////////////////////////////

    // 2. particle loop //////////////////////////////////////////////////
    int nChargSeen = 0;
    int nLepton_cut = 0;
    int nLepton_cut_true = 0;
    int nLepton_cut_fake = 0;
    int nLepton_cut_det = 0;
    int nChargSeen_det = 0;
    double Etot = 0.;
    double pztot = 0.;
    TLorentzVector TL_p4Prt;
    TLorentzVector TL_p4Prt_det;
    TLorentzVector TL_p4Sum = TLorentzVector(0.,0.,0.,0.);
    TLorentzVector TL_lep = TLorentzVector(0.,0.,0.,0.);

    // Reset Fastjet input
    fjInputs.resize(0);

    // Loop over pythia particles
    int ip = 0;
    for ( HepMC::GenEvent::particle_const_iterator p = evt->particles_begin();
	  p != evt->particles_end(); ++p, ip++){

      int pid = (*p)->pdg_id();
      double px = (*p)->momentum().x();
      double py = (*p)->momentum().y();
      double pz = (*p)->momentum().z();
      double ee = (*p)->momentum().t();
      double pp = sqrt( px*px + py*py + pz*pz );
      double cost = pz/pp;

      // Initial cuts again: //////////////////////
      // Skip if not a final state particle
      if ( (*p)->status() != 1 )                    continue;

      // Skip neutrinos
      if ( MyParticles::IsNeutrino(pid) )           continue;

      // Skip particles close to beam pipe
      if ( fabs(cost) > 0.99 )                      continue;

      // Skip soft particles
      if ( pp < 0.1 )                               continue;
      /////////////////////////////////////////////

      Etot  += ee;
      pztot += pz;

      if ( MyParticles::IsChargedParticle(pid) ) nChargSeen++;

      TL_p4Prt.SetPxPyPzE((*p)->momentum().x(),
			  (*p)->momentum().y(),
			  (*p)->momentum().z(),
			  (*p)->momentum().t() );


      // Detector: //////////////////////////////////////
      if (detector) {
        // Acceptance
        if ( MyDetector::NotAccepted( TL_p4Prt.Eta() ) )           continue;

        // Tracker
        if ( MyParticles::IsChargedParticle(pid) ) {
          // Efficiency
          if ( MyDetector::NotDetected() )                         continue;
          TL_p4Prt_det = TL_p4Prt * MyDetector::Tracker( pT );
        }
        // Ecal
        else if ( MyParticles::IsGamma(pid) ) {
          TL_p4Prt_det = TL_p4Prt * MyDetector::Ecal( pT );
        }
        // Hcal
        else {
          TL_p4Prt_det = TL_p4Prt * MyDetector::Hcal( pT );
        }
      }

      if (detector == false) {
        TL_p4Prt_det = TL_p4Prt;
      }

      TL_p4Sum += TL_p4Prt_det;
      px = TL_p4Prt_det.Px();
      py = TL_p4Prt_det.Py();
      pz = TL_p4Prt_det.Pz();
      ee = TL_p4Prt_det.E();

      /////////////////////////////////////////
      // Requirements again after detector sim
      if (detector){
        // if (MyParticles::IsMuon(pid)) {
        //   if ( TL_p4Prt.Rho() > 3 ) {
        //     nLepton_cut_det++;
        //   }
        // }
        if (MyParticles::IsChargedParticle(pid)) { 
          nChargSeen_det++;
        }
      }
      ///////////////////////////////////////////////////////////////


      // Save true leptons with p>3GeV
      if (MyParticles::IsMuon(pid)) {
        if (TL_p4Prt_det.Rho() > 3){
          nLepton_cut_true++;
          nLepton_cut++;
        }
      }

      ///////////////////////////////////////////////////////////////
      // Save and skip lepton with highest momentum for jet recon
      if ( lep_ip == ip ) {
        TL_lep.SetPxPyPzE( px, py, pz, ee);
        continue;
      }

      /////////////////////////////////////////////////////////////////
      // pion misID:
      if (misID){
        // no leptons in sample -> fake leptons from pre-selection
        if ( nLepton_cut_1 == 0 ) { 
          // save the chosen pion as the selected lepton:
          if ( pion_ip_2 == ip ) {
            TL_lep.SetPxPyPzE( px, py, pz, ee);
            nLepton_cut_fake++;
            nLepton_cut++;
            continue;
          }
        }

        // misidentify rest of the pions
        if (MyParticles::IsChargedPion(pid)){
          if (TL_p4Prt_det.Rho() > 3){
            //if ( myRandomNumber.Rndm() < nPion_cut_1*0.0025 ) {              
            if ( myRandomNumber.Rndm() < 0.0025 ) {              	
             nLepton_cut_fake++;
             nLepton_cut++;   // count the pion as a muon
            }
          }
        }

      } 
      /////////////////////////////////////////////////////////////////
      // Form input for fastjet
      fjInput = fastjet::PseudoJet( px, py, pz, ee );
      fjInput.set_user_index( pid );
      //printf("pid=%d\n",pid);
      fjInputs.push_back( fjInput );

      ////////////////////////////////////////////////////////////////////////////////////////
    } // end loop over pythia particles
    ////////////////////////////////////////////////////////////////////////////////////////


    h_testCount -> Fill(0);
    h_cutFlow -> Fill(0);

    h_cut0_nPion_cut->Fill(nPion_cut_1);
    h_cut0_nLepton->Fill(nLepton);
    h_cut0_nLepton_cut_true -> Fill (nLepton_cut_true);
    h_cut0_nLepton_cut_fake -> Fill (nLepton_cut_fake);
    h_cut0_nLepton_cut -> Fill (nLepton_cut);


    ///////////////////////////////////////////////
    // Requirements again after detector sim
    if (detector){
      // min 1 leptons w/ p>3GeV
      //if (nLepton_cut_det != 1)            continue;
      if (nLepton_cut < 1)               continue;
      h_testCount -> Fill(1);

      if (TL_lep.Rho() == 0.)             continue;
      h_testCount -> Fill(2);
      
      // Number charged particles
      if (nChargSeen_det < 3)             continue;
      h_testCount -> Fill(3);

      // pMiss > 5GeV
      if (TL_p4Sum.Rho() < 5)             continue;
      h_testCount -> Fill(4);
    }
    ///////////////////////////////////////////////

    


    // Missing energy, momentum and mass
    TLorentzVector TL_pMiss = - TL_p4Sum;
    double pMiss = TL_p4Sum.Rho();
    double pMiss_absCost = fabs( cos( TL_pMiss.Theta() ));
    double pTMiss = pT;
    double pSum = TL_p4Sum.Rho();
    double ESum = TL_p4Sum.E();
    double mtot = TL_p4Sum.M();
    double EMiss = eCM - ESum;
    double mMiss2 = pow(EMiss,2)-pow(pMiss,2);
    double mMiss = sqrt(mMiss2);
    double cos_pMissLep = MyFuncs::CosT( TL_pMiss, TL_lep );

    // Fill histograms
    h_nPart->Fill(nPart);
    h_nCharg->Fill(nCharg);
    h_ETot->Fill(Etot);
    h_pzTot->Fill(pztot);
    h_nChargSeen->Fill(nChargSeen);

    // if ( nChargSeen > 10 )
    //   printf("nChargSeen = %d, evtNum: %d\n", nChargSeen, evt->event_number() );

    h_pMiss->Fill(pMiss);
    h_pMiss_absCost -> Fill( pMiss_absCost );
    h_pTMiss->Fill(pTMiss);
    h_pSum->Fill(pSum);
    h_ESum->Fill(ESum);
    h_mtot->Fill(mtot);
    h_EMiss->Fill(EMiss);
    h_mMiss->Fill(mMiss);
    h_Enu->Fill(Enu);
    h_Eunseen->Fill(Eunseen);
    h_cos_pMissLep -> Fill(cos_pMissLep);

    h2_mtot_pMiss->Fill(mtot,pMiss);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Cut in eeqq for invariant mass over 50 GeV to minimize double counting from Zqq
    if ( strcmp(argv[1],"2phot_uU")==0  || strcmp(argv[1],"2phot_dD")==0 ||
      strcmp(argv[1],"2phot_cC")==0 || strcmp(argv[1],"2phot_sS")==0 ||
      strcmp(argv[1],"2phot_bB")==0) {

      if (mtot > 50)                 continue;
    }

    
    h_cutFlow -> Fill(1);


    // CUT 1 // **********************************************************
    // Excactly 1 muon
    h_cut1_nPion_cut->Fill(nPion_cut_1);
    h_cut1_nLepton->Fill(nLepton);
    h_cut1_nLepton_cut_true -> Fill (nLepton_cut_true);
    h_cut1_nLepton_cut_fake -> Fill (nLepton_cut_fake);
    h_cut1_nLepton_cut -> Fill (nLepton_cut);
    h_cut1_pMiss -> Fill(pMiss);
    h_cut1_pMiss_absCost -> Fill(pMiss_absCost);
    h_cut1_cos_pMissLep -> Fill(cos_pMissLep);
    h_cut1_mtot -> Fill(mtot);
    h_cut1_nChargSeen -> Fill(nChargSeen);
    if (nLepton_cut != 1)            continue;
    h_cutFlow -> Fill(2);
    // *******************************************************************



    // CUT 2 // **********************************************************
    // pMiss_absCost < 0.94
    h_cut2_nPion_cut->Fill(nPion_cut_1);
    h_cut2_nLepton->Fill(nLepton);
    h_cut2_nLepton_cut_true -> Fill (nLepton_cut_true);
    h_cut2_nLepton_cut_fake -> Fill (nLepton_cut_fake);
    h_cut2_nLepton_cut -> Fill (nLepton_cut);
    h_cut2_pMiss -> Fill(pMiss);
    h_cut2_pMiss_absCost -> Fill(pMiss_absCost);
    h_cut2_cos_pMissLep -> Fill(cos_pMissLep);
    h_cut2_mtot -> Fill(mtot);
    h_cut2_nChargSeen -> Fill(nChargSeen);
    if (pMiss_absCost > 0.94)            continue;
    h_cutFlow -> Fill(3);
    // *******************************************************************


    // CUT 3 // **********************************************************
    // cos_pMissLep < 0.8 
    h_cut3_nPion_cut->Fill(nPion_cut_1);
    h_cut3_nLepton->Fill(nLepton);
    h_cut3_nLepton_cut_true -> Fill (nLepton_cut_true);
    h_cut3_nLepton_cut_fake -> Fill (nLepton_cut_fake);
    h_cut3_nLepton_cut -> Fill (nLepton_cut);
    h_cut3_pMiss -> Fill(pMiss);
    h_cut3_pMiss_absCost -> Fill(pMiss_absCost);
    h_cut3_cos_pMissLep -> Fill(cos_pMissLep);
    h_cut3_mtot -> Fill(mtot);
    h_cut3_nChargSeen -> Fill(nChargSeen);
    if (cos_pMissLep > 0.8 )            continue;
    h_cutFlow -> Fill(4);
    // *******************************************************************





    // Run Fastjet algorithm ///////////////////////////////////////////////////////////
    std::vector <fastjet::PseudoJet> exclusiveJets;
    std::vector <fastjet::PseudoJet> exclusiveJets_3;
    std::vector <fastjet::PseudoJet> jetConstituents;
    fastjet::ClusterSequence clustSeq(fjInputs, *jetDef);
    double ymerge;
    double ymerge_3;

    // For the first event, print the FastJet details
    if (firstEvent) {
      std::cout << "Ran " << jetDef->description() << std::endl;
      std::cout << "Strategy adopted by FastJet was "
		<< clustSeq.strategy_string()
		<< std::endl << std::endl;
      firstEvent = false;
    }

    ////////////////////////////////////////////////////////////////////////
    // Force into exactly 2 jets
    ////////////////////////////////////////////////////////////////////////

    if ( fjInputs.size() < 2 ) {
      std::cout << " Less than 2 particles for jet, skipping event"
		<< std::endl << std::endl;
      continue;
    }


    h_testCount -> Fill(5);

    exclusiveJets.resize(0);
    exclusiveJets = clustSeq.exclusive_jets(2);
    ymerge = clustSeq.exclusive_ymerge(2);
    int nJet = exclusiveJets.size();
    int jet_nCharg[2];
    jet_nCharg[0]= 0;
    jet_nCharg[1]= 0;

    

    if ( nJet != 2 ) {
      printf("Expected 2 jets, found %d. Skipping event!\n", nJet );
      continue;
    }

    h_testCount -> Fill(6);


    TLorentzVector TL_jet[2];
    for ( int i=0; i<nJet; i++ ) {
      TL_jet[i].SetPxPyPzE(exclusiveJets[i].px(),
                           exclusiveJets[i].py(),
                           exclusiveJets[i].pz(),
                           exclusiveJets[i].E() );
      // get the constituents of the jet
      std::vector<fastjet::PseudoJet> constituents = exclusiveJets[i].constituents();
      for (int j=0; j<constituents.size(); j++){
        int jet_pid = constituents[j].user_index();
        //printf("i=%d, j=%d, jet_pid =  %4d \n", i, j, jet_pid) ;
        if (MyParticles::IsChargedParticle(jet_pid))
          jet_nCharg[i]++;
      }
    }
    //printf( "nCharg1 =   %4u \n", jet_nCharg[0] );
    //printf( "nCharg2 =   %4u \n", jet_nCharg[1] );


    double jet_nCharg_min = MyFuncs::Min( jet_nCharg[0], jet_nCharg[1] );
    double jet_nCharg_max = MyFuncs::Max( jet_nCharg[0], jet_nCharg[1] );
    double jetE_min = MyFuncs::Min( TL_jet[0].E(), TL_jet[1].E() );
    double jetE_max = MyFuncs::Max( TL_jet[0].E(), TL_jet[1].E() );



  // CUT 4 // **********************************************************
    // Skip events if jet energy < 3 GeV  &&  Skip if less than 1 charged track
    h_cut4_pMiss -> Fill(pMiss);
    h_cut4_pMiss_absCost -> Fill(pMiss_absCost);
    h_cut4_cos_pMissLep -> Fill(cos_pMissLep);
    h_cut4_mtot -> Fill(mtot);
    h_cut4_nChargSeen -> Fill(nChargSeen);
    h_cut4_ymerge -> Fill(ymerge);
    h_cut4_jet_nCharg_min -> Fill(jet_nCharg_min);
    h_cut4_jet_nCharg_max -> Fill(jet_nCharg_max);
    h_cut4_jetE_min -> Fill( jetE_min );
    h_cut4_jetE_max -> Fill( jetE_max ); 
    // Skip events if jet energy < 3 GeV
    if ( TL_jet[0].E() < 3. || TL_jet[1].E() < 3. )        continue;
    h_testCount -> Fill(7);
    h_jet_nCharg_min -> Fill(jet_nCharg_min);
    h_jet_nCharg_max -> Fill(jet_nCharg_max);    
    // Skip if less than 1 charged track
    if ( jet_nCharg[0] < 1 || jet_nCharg[1] < 1 )         continue;
    h_cutFlow -> Fill(5);
    // *******************************************************************


    h_testCount -> Fill(8);



    // Calculating different variables
    TLorentzVector TL_jetsum = TL_jet[0] + TL_jet[1];

    double jetMass_min = MyFuncs::Min( TL_jet[0].M(), TL_jet[1].M() );
    double jetMass_max = MyFuncs::Max( TL_jet[0].M(), TL_jet[1].M() );
    double jetSumMass = ( TL_jetsum.M() )  ;
    double cos_jj = MyFuncs::CosT( TL_jet[0], TL_jet[1] );
    double cos_jlep[2] = { MyFuncs::CosT( TL_jet[0], TL_lep ), MyFuncs::CosT( TL_jet[1], TL_lep ) };
    double cos_jlep_min = MyFuncs::Min( cos_jlep[0], cos_jlep[1] );
    double cos_jlep_max = MyFuncs::Max( cos_jlep[0], cos_jlep[1] );
    double cos_jpMiss[2] = { MyFuncs::CosT( TL_jet[0], TL_pMiss ), MyFuncs::CosT( TL_jet[1], TL_pMiss ) };
    double cos_jpMiss_min = MyFuncs::Min( cos_jpMiss[0], cos_jpMiss[1] );
    double cos_jpMiss_max = MyFuncs::Max( cos_jpMiss[0], cos_jpMiss[1] );
    double jet_absCost[2] = { fabs(cos(TL_jet[0].Theta())) , fabs(cos(TL_jet[1].Theta())) };

    h_ymerge -> Fill(ymerge);
    h_jetMass_min -> Fill(jetMass_min);
    h_jetMass_max -> Fill(jetMass_max);
    h_jetSumMass -> Fill(jetSumMass);
    h_jetSumP -> Fill(TL_jetsum.Rho());
    h_jetE_min -> Fill(jetE_min);
    h_jetE_max -> Fill(jetE_max);
    h_cos_jj -> Fill(cos_jj);
    h_cos_jlep_max -> Fill( cos_jlep_max );
    h_cos_jlep_min -> Fill( cos_jlep_min );
    h_cos_jpMiss_min -> Fill(cos_jpMiss_min);
    h_cos_jpMiss_max -> Fill(cos_jpMiss_max);
    h_jet_absCost -> Fill (jet_absCost[0]);
    h_jet_absCost -> Fill (jet_absCost[1]);



    //////////////////////////////////////////////////////////////////////////
    // Force into exactly 3 jets
    //////////////////////////////////////////////////////////////////////////

    if ( fjInputs.size() < 3 ) {
      std::cout << " Less than 3 particles for jet_3, skipping event"
		<< std::endl << std::endl;
      continue;
    }

    h_testCount -> Fill(9);


    exclusiveJets_3.resize(0);
    exclusiveJets_3 = clustSeq.exclusive_jets(3);
    ymerge_3 = clustSeq.exclusive_ymerge(3);
    int nJet_3 = exclusiveJets_3.size();

    TLorentzVector TL_jet_3[3];
    TLorentzVector TL_jetsum_3;
    double jetSumMass_3 = -10;
    double jetE_min_3 = -10;
    double jetE_max_3 = -10;
    double cos_jj_3[3];
    double cos_jlep_3[3];
    double cos_jpMiss_3[3];
    double thetaSum_jj_3 = -10 ;
    double cos_jj_min_3 = -10;
    double cos_jj_max_3 = -10;
    double cos_jlep_min_3 = -10;
    double cos_jlep_max_3 = -10;
    double cos_jpMiss_min_3 = -10;
    double cos_jpMiss_max_3 = -10;
    int jet_nCharg_3[3];
    jet_nCharg_3[0]= 0;
    jet_nCharg_3[1]= 0;
    jet_nCharg_3[2]= 0;

    if ( nJet_3 == 3 ) {

      for ( int i=0; i<nJet_3; i++ ) {
        TL_jet_3[i].SetPxPyPzE(exclusiveJets_3[i].px(),
			       exclusiveJets_3[i].py(),
			       exclusiveJets_3[i].pz(),
			       exclusiveJets_3[i].E() );
        // get the constituents of the jet
        std::vector<fastjet::PseudoJet> constituents_3 = exclusiveJets_3[i].constituents();
        for (int j=0; j<constituents_3.size(); j++){
          int jet_pid = constituents_3[j].user_index();
          //printf("i=%d, j=%d, jet_pid =  %4d \n", i, j, jet_pid) ;
          if (MyParticles::IsChargedParticle(jet_pid))
            jet_nCharg_3[i]++;
        }
      }

      nJet_3 = -1;
      // Calc only if jet energy > 3 GeV
      if ( TL_jet_3[0].E() > 3 && TL_jet_3[1].E() > 3 && TL_jet_3[2].E() > 3 ) {

        double jet_nCharg_min_3 = MyFuncs::Min_3( jet_nCharg_3[0], jet_nCharg_3[1], jet_nCharg_3[2] );
        double jet_nCharg_max_3 = MyFuncs::Max_3( jet_nCharg_3[0], jet_nCharg_3[1], jet_nCharg_3[2] );
        h_jet_nCharg_min_3 -> Fill(jet_nCharg_min_3);
        h_jet_nCharg_max_3 -> Fill(jet_nCharg_max_3);
        // Calc only if more than or equal 1 charged track
        if ( jet_nCharg_3[0] > 0 && jet_nCharg_3[1] > 0 && jet_nCharg_3[2] > 0 ) {
          nJet_3 = 3;

  	      // Calculating different variables
          TL_jetsum_3 = TL_jet_3[0] + TL_jet_3[1] + TL_jet_3[2];
          jetSumMass_3 = ( TL_jetsum_3.M() )  ;
          jetE_min_3 = MyFuncs::Min_3( TL_jet_3[0].E(), TL_jet_3[1].E(), TL_jet_3[2].E() );
          jetE_max_3 = MyFuncs::Max_3( TL_jet_3[0].E(), TL_jet_3[1].E(), TL_jet_3[2].E() );

          for ( int i=0; i<nJet_3; ++i ) {
            cos_jlep_3[i] = MyFuncs::CosT( TL_jet_3[i], TL_lep );
            cos_jpMiss_3[i] = MyFuncs::CosT( TL_jet_3[i], TL_pMiss );
          }
          cos_jj_3[0] = MyFuncs::CosT( TL_jet_3[0], TL_jet_3[1] );
          cos_jj_3[1] = MyFuncs::CosT( TL_jet_3[0], TL_jet_3[2] );
          cos_jj_3[2] = MyFuncs::CosT( TL_jet_3[1], TL_jet_3[2] );

          thetaSum_jj_3 = ( acos(cos_jj_3[0])+acos(cos_jj_3[1])+acos(cos_jj_3[2]) ) * 180./TMath::Pi() ;
          cos_jj_min_3 = MyFuncs::Min_3( cos_jj_3[0], cos_jj_3[1], cos_jj_3[2] );
          cos_jj_max_3 = MyFuncs::Max_3( cos_jj_3[0], cos_jj_3[1], cos_jj_3[2] );
          cos_jlep_min_3 = MyFuncs::Min_3( cos_jlep_3[0], cos_jlep_3[1], cos_jlep_3[2] );
          cos_jlep_max_3 = MyFuncs::Max_3( cos_jlep_3[0], cos_jlep_3[1], cos_jlep_3[2] );
          cos_jpMiss_min_3 = MyFuncs::Min_3( cos_jpMiss_3[0], cos_jpMiss_3[1], cos_jpMiss_3[2] );
          cos_jpMiss_max_3 = MyFuncs::Max_3( cos_jpMiss_3[0], cos_jpMiss_3[1], cos_jpMiss_3[2] );

          h_ymerge_3 -> Fill(ymerge_3);
          h_jetMass_3 -> Fill(TL_jet_3[0].M());
          h_jetMass_3 -> Fill(TL_jet_3[1].M());
          h_jetMass_3 -> Fill(TL_jet_3[2].M());
          h_jetSumMass_3 -> Fill(jetSumMass_3);
          h_jetE_min_3 -> Fill(jetE_min_3);
          h_jetE_max_3 -> Fill(jetE_max_3);
          h_cos_jj_min_3 -> Fill(cos_jj_min_3);
          h_cos_jj_max_3 -> Fill(cos_jj_max_3);
          h_cos_jlep_max_3 -> Fill(cos_jlep_max_3);
          h_cos_jlep_min_3 -> Fill(cos_jlep_min_3);
          h_cos_jpMiss_min_3 -> Fill(cos_jpMiss_min_3);
          h_cos_jpMiss_max_3 -> Fill(cos_jpMiss_max_3);
          h_thetaSum_jj_3 -> Fill(thetaSum_jj_3);

        }
      }
    }


   //    //////////////////////////////////////////////////////////////////////////
   //    // Cut flow only
   //    //////////////////////////////////////////////////////////////////////////


    // CUT 5 // **********************************************************
    // // cut1: -0.80<cos_jj<0.98         -> jets back to back (qq) + tautau

    h_cut5_mtot -> Fill( mtot );
    h_cut5_pMiss -> Fill( pMiss );
    h_cut5_nChargSeen   -> Fill( nChargSeen );
    h_cut5_pMiss_absCost  -> Fill( pMiss_absCost );
    h_cut5_cos_pMissLep   -> Fill( cos_pMissLep );
    h_cut5_jetSumMass  -> Fill( jetSumMass );
    h_cut5_jetSumP  -> Fill( TL_jetsum.Rho() );
    h_cut5_jet_nCharg_min -> Fill( jet_nCharg_min );
    h_cut5_jetMass_min -> Fill( jetMass_min );
    h_cut5_cos_jj  -> Fill( cos_jj );
    h_cut5_cos_jlep_max  -> Fill( cos_jlep_max );
    if (nJet_3 == 3){
      h_cut5_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
      h_cut5_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
    }
    else{
      h_cut5_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
    }    
    if ( cos_jj < -0.80 )                 continue;
    if ( cos_jj > 0.98 )                  continue;
    h_cutFlow -> Fill(6);

    // *******************************************************************


    // CUT 6 // **********************************************************
    // [cos_jlep_max_3| < 0.80    -> lepton inside jet + tau bkg
    // subsample cos_jlep_max

    h_cut6_mtot -> Fill( mtot );
    h_cut6_pMiss -> Fill( pMiss );
    h_cut6_nChargSeen   -> Fill( nChargSeen );
    h_cut6_pMiss_absCost  -> Fill( pMiss_absCost );
    h_cut6_cos_pMissLep   -> Fill( cos_pMissLep );
    h_cut6_jetSumMass  -> Fill( jetSumMass );
    h_cut6_jetSumP  -> Fill( TL_jetsum.Rho() );
    h_cut6_jet_nCharg_min -> Fill( jet_nCharg_min );
    h_cut6_jetMass_min -> Fill( jetMass_min );
    h_cut6_cos_jj  -> Fill( cos_jj );
    h_cut6_cos_jlep_max  -> Fill( cos_jlep_max );
    if (nJet_3 == 3){
      h_cutFlow -> Fill(7);
      h_cut6_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
      h_cut6_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
      if (cos_jlep_max_3 > 0.80)                   continue;
      h_cutFlow -> Fill(8);
    }
    else{
      h_cutFlow -> Fill(10);
      h_cut6_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
      if (cos_jlep_max > 0.80)                  continue;
      h_cutFlow -> Fill(11);
    }    


   // *******************************************************************


    // CUT 7 // **********************************************************
    // thetaSum_jj_3 > 300.

    h_cut7_mtot -> Fill( mtot );
    h_cut7_pMiss -> Fill( pMiss );
    h_cut7_nChargSeen   -> Fill( nChargSeen );
    h_cut7_pMiss_absCost  -> Fill( pMiss_absCost );
    h_cut7_cos_pMissLep   -> Fill( cos_pMissLep );
    h_cut7_jetSumMass  -> Fill( jetSumMass );
    h_cut7_jetSumP  -> Fill( TL_jetsum.Rho() );
    h_cut7_jet_nCharg_min -> Fill( jet_nCharg_min );
    h_cut7_jetMass_min -> Fill( jetMass_min );
    h_cut7_cos_jj  -> Fill( cos_jj );
    h_cut7_cos_jlep_max  -> Fill( cos_jlep_max );
    if (nJet_3 == 3){
      h_cut7_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
      h_cut7_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
      if(  thetaSum_jj_3 > 300.  )                   continue;
      h_cutFlow -> Fill(9);
    }
    else{
      h_cut7_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
    }    
    h_cutFlow -> Fill(12);
    // *******************************************************************


    // CUT 8 // **********************************************************
    // ( jetMass_min < 1.8 )

    h_cut8_mtot -> Fill( mtot );
    h_cut8_pMiss -> Fill( pMiss );
    h_cut8_nChargSeen   -> Fill( nChargSeen );
    h_cut8_pMiss_absCost  -> Fill( pMiss_absCost );
    h_cut8_cos_pMissLep   -> Fill( cos_pMissLep );
    h_cut8_jetSumMass  -> Fill( jetSumMass );
    h_cut8_jetSumP  -> Fill( TL_jetsum.Rho() );
    h_cut8_jet_nCharg_min -> Fill( jet_nCharg_min );
    h_cut8_jetMass_min -> Fill( jetMass_min );
    h_cut8_cos_jj  -> Fill( cos_jj );
    h_cut8_cos_jlep_max  -> Fill( cos_jlep_max );
    if (nJet_3 == 3){
      h_cut8_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
      h_cut8_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
    }
    else{
      h_cut8_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
    }    
    if ( jetMass_min < 1.8 )                         continue;
    h_cutFlow -> Fill(13);

    // *******************************************************************


    // //////////////////////////////////////////////////////////////////////////
    // CUT 9 & 10   **********************************************************
    // sliding window in pMiss and mass

    h_cut9_mtot -> Fill( mtot );
    h_cut9_pMiss -> Fill( pMiss );
    h_cut9_nChargSeen   -> Fill( nChargSeen );
    h_cut9_pMiss_absCost  -> Fill( pMiss_absCost );
    h_cut9_cos_pMissLep   -> Fill( cos_pMissLep );
    h_cut9_jetSumMass  -> Fill( jetSumMass );
    h_cut9_jetSumP  -> Fill( TL_jetsum.Rho() );
    h_cut9_jet_nCharg_min -> Fill( jet_nCharg_min );
    h_cut9_jetMass_min -> Fill( jetMass_min );
    h_cut9_cos_jj  -> Fill( cos_jj );
    h_cut9_cos_jlep_max  -> Fill( cos_jlep_max );
    if (nJet_3 == 3){
      h_cut9_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
      h_cut9_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
    }
    else{
      h_cut9_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
    }


    // *******************************************************************
    // N30: pMiss=40.7 +-3.5
    if( pMiss>37.2 && pMiss<44.2 ) {
      h_cutFlow -> Fill(14);
    // *******************************************************************
      h_cut10_N30_mtot -> Fill( mtot );
      h_cut10_N30_pMiss -> Fill( pMiss );
      h_cut10_N30_nChargSeen   -> Fill( nChargSeen );
      h_cut10_N30_pMiss_absCost  -> Fill( pMiss_absCost );
      h_cut10_N30_cos_pMissLep   -> Fill( cos_pMissLep );
      h_cut10_N30_jetSumMass  -> Fill( jetSumMass );
      h_cut10_N30_jetSumP  -> Fill( TL_jetsum.Rho() );
      h_cut10_N30_jet_nCharg_min -> Fill( jet_nCharg_min );
      h_cut10_N30_jetMass_min -> Fill( jetMass_min );
      h_cut10_N30_cos_jj  -> Fill( cos_jj );
      h_cut10_N30_cos_jlep_max  -> Fill( cos_jlep_max );
      if (nJet_3 == 3){
        h_cut10_N30_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
        h_cut10_N30_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
      }
      else{
        h_cut10_N30_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
      }
      // *******************************************************************
      // mtot cut for N30
      if (mtot>26. && mtot<34.) {
        h_cutFlow -> Fill(15);
      // *******************************************************************
        h_cut11_N30_mtot -> Fill( mtot );
        h_cut11_N30_pMiss -> Fill( pMiss );
        h_cut11_N30_nChargSeen   -> Fill( nChargSeen );
        h_cut11_N30_pMiss_absCost  -> Fill( pMiss_absCost );
        h_cut11_N30_cos_pMissLep   -> Fill( cos_pMissLep );
        h_cut11_N30_jetSumMass  -> Fill( jetSumMass );
        h_cut11_N30_jetSumP  -> Fill( TL_jetsum.Rho() );
        h_cut11_N30_jet_nCharg_min -> Fill( jet_nCharg_min );
        h_cut11_N30_jetMass_min -> Fill( jetMass_min );
        h_cut11_N30_cos_jj  -> Fill( cos_jj );
        h_cut11_N30_cos_jlep_max  -> Fill( cos_jlep_max );
        if (nJet_3 == 3){
          h_cut11_N30_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
          h_cut11_N30_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
        }
        else{
          h_cut11_N30_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
        }

        //Write remaining background events to a file.
        if ( strcmp(argv[1],"Z")==0 ) {
          ascii_io << evt;
        }
        if ( strcmp(argv[1],"Ztautau")==0 ) {
          ofstream outputFile;
          outputFile.open( "Bkg_HM_Ztautau.txt", ios::out | ios::app );
          outputFile << "N30 : " << endl;
          outputFile << "Bkg : " << argv[1] << endl;
          outputFile << "Event number : " << evt->event_number() << endl;
          outputFile << "=============================================" << endl;
          outputFile.close();
        }
        if ( strcmp(argv[1],"N30")==0 ) {
          ofstream outputFile;
          outputFile.open( "Events_N30_HM.txt", ios::out | ios::app );
          outputFile << evt->event_number() << endl;
          outputFile.close();
        }

      }
    }
    // *******************************************************************

    // *******************************************************************
    // N40: pMiss=36.8 +-3.5
    if( pMiss>33.3 && pMiss<40.3 ) {
      h_cutFlow -> Fill(16);
    // *******************************************************************
      h_cut10_N40_mtot -> Fill( mtot );
      h_cut10_N40_pMiss -> Fill( pMiss );
      h_cut10_N40_nChargSeen   -> Fill( nChargSeen );
      h_cut10_N40_pMiss_absCost  -> Fill( pMiss_absCost );
      h_cut10_N40_cos_pMissLep   -> Fill( cos_pMissLep );
      h_cut10_N40_jetSumMass  -> Fill( jetSumMass );
      h_cut10_N40_jetSumP  -> Fill( TL_jetsum.Rho() );
      h_cut10_N40_jet_nCharg_min -> Fill( jet_nCharg_min );
      h_cut10_N40_jetMass_min -> Fill( jetMass_min );
      h_cut10_N40_cos_jj  -> Fill( cos_jj );
      h_cut10_N40_cos_jlep_max  -> Fill( cos_jlep_max );
      if (nJet_3 == 3){
        h_cut10_N40_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
        h_cut10_N40_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
      }
      else{
        h_cut10_N40_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
      }
      // *******************************************************************
      // mtot cut for N40
      if (mtot>36. && mtot<44.) {
        h_cutFlow -> Fill(17);
      // *******************************************************************
        h_cut11_N40_mtot -> Fill( mtot );
        h_cut11_N40_pMiss -> Fill( pMiss );
        h_cut11_N40_nChargSeen   -> Fill( nChargSeen );
        h_cut11_N40_pMiss_absCost  -> Fill( pMiss_absCost );
        h_cut11_N40_cos_pMissLep   -> Fill( cos_pMissLep );
        h_cut11_N40_jetSumMass  -> Fill( jetSumMass );
        h_cut11_N40_jetSumP  -> Fill( TL_jetsum.Rho() );
        h_cut11_N40_jet_nCharg_min -> Fill( jet_nCharg_min );
        h_cut11_N40_jetMass_min -> Fill( jetMass_min );
        h_cut11_N40_cos_jj  -> Fill( cos_jj );
        h_cut11_N40_cos_jlep_max  -> Fill( cos_jlep_max );
        if (nJet_3 == 3){
          h_cut11_N40_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
          h_cut11_N40_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
        }
        else{
          h_cut11_N40_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
        }

        //Write remaining background events to a file.
        if ( strcmp(argv[1],"Z")==0 ) {
          ascii_io << evt;
        }
        if ( strcmp(argv[1],"Ztautau")==0 ) {
          ofstream outputFile;
          outputFile.open( "Bkg_HM_Ztautau.txt", ios::out | ios::app );
          outputFile << "N40 : " << endl;
          outputFile << "Bkg : " << argv[1] << endl;
          outputFile << "Event number : " << evt->event_number() << endl;
          outputFile << "=============================================" << endl;
          outputFile.close();
        }

      }
    }
    // *******************************************************************

    // *******************************************************************
    // N50: pMiss=31.9 +-3.5
    if( pMiss>28.4 && pMiss<35.4 ) {
      h_cutFlow -> Fill(18);
    // *******************************************************************
      h_cut10_N50_mtot -> Fill( mtot );
      h_cut10_N50_pMiss -> Fill( pMiss );
      h_cut10_N50_nChargSeen   -> Fill( nChargSeen );
      h_cut10_N50_pMiss_absCost  -> Fill( pMiss_absCost );
      h_cut10_N50_cos_pMissLep   -> Fill( cos_pMissLep );
      h_cut10_N50_jetSumMass  -> Fill( jetSumMass );
      h_cut10_N50_jetSumP  -> Fill( TL_jetsum.Rho() );
      h_cut10_N50_jet_nCharg_min -> Fill( jet_nCharg_min );
      h_cut10_N50_jetMass_min -> Fill( jetMass_min );
      h_cut10_N50_cos_jj  -> Fill( cos_jj );
      h_cut10_N50_cos_jlep_max  -> Fill( cos_jlep_max );
      if (nJet_3 == 3){
        h_cut10_N50_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
        h_cut10_N50_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
      }
      else{
        h_cut10_N50_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
      }
      // *******************************************************************
      // mtot cut for N50 
      if (mtot>46. && mtot<54.) {
        h_cutFlow -> Fill(19);
      // *******************************************************************
        h_cut11_N50_mtot -> Fill( mtot );
        h_cut11_N50_pMiss -> Fill( pMiss );
        h_cut11_N50_nChargSeen   -> Fill( nChargSeen );
        h_cut11_N50_pMiss_absCost  -> Fill( pMiss_absCost );
        h_cut11_N50_cos_pMissLep   -> Fill( cos_pMissLep );
        h_cut11_N50_jetSumMass  -> Fill( jetSumMass );
        h_cut11_N50_jetSumP  -> Fill( TL_jetsum.Rho() );
        h_cut11_N50_jet_nCharg_min -> Fill( jet_nCharg_min );
        h_cut11_N50_jetMass_min -> Fill( jetMass_min );
        h_cut11_N50_cos_jj  -> Fill( cos_jj );
        h_cut11_N50_cos_jlep_max  -> Fill( cos_jlep_max );
        if (nJet_3 == 3){
          h_cut11_N50_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
          h_cut11_N50_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
        }
        else{
          h_cut11_N50_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
        }

        //Write remaining background events to a file.
        if ( strcmp(argv[1],"Z")==0 ) {
          ascii_io << evt;
        }
        if ( strcmp(argv[1],"Ztautau")==0 ) {
          ofstream outputFile;
          outputFile.open( "Bkg_HM_Ztautau.txt", ios::out | ios::app );
          outputFile << "N50 : " << endl;
          outputFile << "Bkg : " << argv[1] << endl;
          outputFile << "Event number : " << evt->event_number() << endl;
          outputFile << "=============================================" << endl;
          outputFile.close();
        }

      }
    }
    // *******************************************************************

    // *******************************************************************
    // N60: pMiss=25.9 +-3.5
    if( pMiss>22.4 && pMiss<29.4 ) {
      h_cutFlow -> Fill(20);
    // *******************************************************************
      h_cut10_N60_mtot -> Fill( mtot );
      h_cut10_N60_pMiss -> Fill( pMiss );
      h_cut10_N60_nChargSeen   -> Fill( nChargSeen );
      h_cut10_N60_pMiss_absCost  -> Fill( pMiss_absCost );
      h_cut10_N60_cos_pMissLep   -> Fill( cos_pMissLep );
      h_cut10_N60_jetSumMass  -> Fill( jetSumMass );
      h_cut10_N60_jetSumP  -> Fill( TL_jetsum.Rho() );
      h_cut10_N60_jet_nCharg_min -> Fill( jet_nCharg_min );
      h_cut10_N60_jetMass_min -> Fill( jetMass_min );
      h_cut10_N60_cos_jj  -> Fill( cos_jj );
      h_cut10_N60_cos_jlep_max  -> Fill( cos_jlep_max );
      if (nJet_3 == 3){
        h_cut10_N60_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
        h_cut10_N60_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
      }
      else{
        h_cut10_N60_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
      }
      // *******************************************************************
      // mtot cut for N60 
      if (mtot>56. && mtot<64.) {
        h_cutFlow -> Fill(21);
      // *******************************************************************
        h_cut11_N60_mtot -> Fill( mtot );
        h_cut11_N60_pMiss -> Fill( pMiss );
        h_cut11_N60_nChargSeen   -> Fill( nChargSeen );
        h_cut11_N60_pMiss_absCost  -> Fill( pMiss_absCost );
        h_cut11_N60_cos_pMissLep   -> Fill( cos_pMissLep );
        h_cut11_N60_jetSumMass  -> Fill( jetSumMass );
        h_cut11_N60_jetSumP  -> Fill( TL_jetsum.Rho() );
        h_cut11_N60_jet_nCharg_min -> Fill( jet_nCharg_min );
        h_cut11_N60_jetMass_min -> Fill( jetMass_min );
        h_cut11_N60_cos_jj  -> Fill( cos_jj );
        h_cut11_N60_cos_jlep_max  -> Fill( cos_jlep_max );
        if (nJet_3 == 3){
          h_cut11_N60_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
          h_cut11_N60_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
        }
        else{
          h_cut11_N60_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
        }

        //Write remaining background events to a file.
        if ( strcmp(argv[1],"Z")==0 ) {
          ascii_io << evt;
        }
        if ( strcmp(argv[1],"Ztautau")==0 ) {
          ofstream outputFile;
          outputFile.open( "Bkg_HM_Ztautau.txt", ios::out | ios::app );
          outputFile << "N60 : " << endl;
          outputFile << "Bkg : " << argv[1] << endl;
          outputFile << "Event number : " << evt->event_number() << endl;
          outputFile << "=============================================" << endl;
          outputFile.close();
        }

      }
    }
    // *******************************************************************

    // *******************************************************************
    // N70: pMiss=18.7 +-3.5
    if( pMiss>15.2 && pMiss<22.2 ) {
      h_cutFlow -> Fill(22);
    // *******************************************************************
      h_cut10_N70_mtot -> Fill( mtot );
      h_cut10_N70_pMiss -> Fill( pMiss );
      h_cut10_N70_nChargSeen   -> Fill( nChargSeen );
      h_cut10_N70_pMiss_absCost  -> Fill( pMiss_absCost );
      h_cut10_N70_cos_pMissLep   -> Fill( cos_pMissLep );
      h_cut10_N70_jetSumMass  -> Fill( jetSumMass );
      h_cut10_N70_jetSumP  -> Fill( TL_jetsum.Rho() );
      h_cut10_N70_jet_nCharg_min -> Fill( jet_nCharg_min );
      h_cut10_N70_jetMass_min -> Fill( jetMass_min );
      h_cut10_N70_cos_jj  -> Fill( cos_jj );
      h_cut10_N70_cos_jlep_max  -> Fill( cos_jlep_max );
      if (nJet_3 == 3){
        h_cut10_N70_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
        h_cut10_N70_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
      }
      else{
        h_cut10_N70_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
      }
      // *******************************************************************
      // mtot cut for N70 
      if (mtot>66. && mtot<74.) {
        h_cutFlow -> Fill(23);
      // *******************************************************************
        h_cut11_N70_mtot -> Fill( mtot );
        h_cut11_N70_pMiss -> Fill( pMiss );
        h_cut11_N70_nChargSeen   -> Fill( nChargSeen );
        h_cut11_N70_pMiss_absCost  -> Fill( pMiss_absCost );
        h_cut11_N70_cos_pMissLep   -> Fill( cos_pMissLep );
        h_cut11_N70_jetSumMass  -> Fill( jetSumMass );
        h_cut11_N70_jetSumP  -> Fill( TL_jetsum.Rho() );
        h_cut11_N70_jet_nCharg_min -> Fill( jet_nCharg_min );
        h_cut11_N70_jetMass_min -> Fill( jetMass_min );
        h_cut11_N70_cos_jj  -> Fill( cos_jj );
        h_cut11_N70_cos_jlep_max  -> Fill( cos_jlep_max );
        if (nJet_3 == 3){
          h_cut11_N70_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
          h_cut11_N70_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
        }
        else{
          h_cut11_N70_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
        }

        //Write remaining background events to a file.
        if ( strcmp(argv[1],"Z")==0 ) {
          ascii_io << evt;
        }
        if ( strcmp(argv[1],"Ztautau")==0 ) {
          ofstream outputFile;
          outputFile.open( "Bkg_HM_Ztautau.txt", ios::out | ios::app );
          outputFile << "N70 : " << endl;
          outputFile << "Bkg : " << argv[1] << endl;
          outputFile << "Event number : " << evt->event_number() << endl;
          outputFile << "=============================================" << endl;
          outputFile.close();
        }

      }
    }
    // *******************************************************************

   // *******************************************************************
    // N80: pMiss=10.5 +-3.5
    if( pMiss>7. && pMiss<14. ) {
      h_cutFlow -> Fill(24);
    // *******************************************************************
      h_cut10_N80_mtot -> Fill( mtot );
      h_cut10_N80_pMiss -> Fill( pMiss );
      h_cut10_N80_nChargSeen   -> Fill( nChargSeen );
      h_cut10_N80_pMiss_absCost  -> Fill( pMiss_absCost );
      h_cut10_N80_cos_pMissLep   -> Fill( cos_pMissLep );
      h_cut10_N80_jetSumMass  -> Fill( jetSumMass );
      h_cut10_N80_jetSumP  -> Fill( TL_jetsum.Rho() );
      h_cut10_N80_jet_nCharg_min -> Fill( jet_nCharg_min );
      h_cut10_N80_jetMass_min -> Fill( jetMass_min );
      h_cut10_N80_cos_jj  -> Fill( cos_jj );
      h_cut10_N80_cos_jlep_max  -> Fill( cos_jlep_max );
      if (nJet_3 == 3){
        h_cut10_N80_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
        h_cut10_N80_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
      }
      else{
        h_cut10_N80_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
      }
      // *******************************************************************
      // mtot cut for N80 
      if (mtot>76. && mtot<84.) {
        h_cutFlow -> Fill(25);
      // *******************************************************************
        h_cut11_N80_mtot -> Fill( mtot );
        h_cut11_N80_pMiss -> Fill( pMiss );
        h_cut11_N80_nChargSeen   -> Fill( nChargSeen );
        h_cut11_N80_pMiss_absCost  -> Fill( pMiss_absCost );
        h_cut11_N80_cos_pMissLep   -> Fill( cos_pMissLep );
        h_cut11_N80_jetSumMass  -> Fill( jetSumMass );
        h_cut11_N80_jetSumP  -> Fill( TL_jetsum.Rho() );
        h_cut11_N80_jet_nCharg_min -> Fill( jet_nCharg_min );
        h_cut11_N80_jetMass_min -> Fill( jetMass_min );
        h_cut11_N80_cos_jj  -> Fill( cos_jj );
        h_cut11_N80_cos_jlep_max  -> Fill( cos_jlep_max );
        if (nJet_3 == 3){
          h_cut11_N80_cos_jlep_max_3 -> Fill( cos_jlep_max_3 );
          h_cut11_N80_thetaSum_jj_3 -> Fill( thetaSum_jj_3 );
        }
        else{
          h_cut11_N80_cos_jlep_max_NOT3 -> Fill( cos_jlep_max );
        }

        //Write remaining background events to a file.
        if ( strcmp(argv[1],"Z")==0 ) {
          ascii_io << evt;
        }
        if ( strcmp(argv[1],"Ztautau")==0 ) {
          ofstream outputFile;
          outputFile.open( "Bkg_HM_Ztautau.txt", ios::out | ios::app );
          outputFile << "N80 : " << endl;
          outputFile << "Bkg : " << argv[1] << endl;
          outputFile << "Event number : " << evt->event_number() << endl;
          outputFile << "=============================================" << endl;
          outputFile.close();
        }

      }
    }
    // *******************************************************************





  }
  //........................................Termination

  std::cout << "Total number of events: "<< icount << std::endl;


  histFile->Write();
  histFile->Close();

  return 0;
}
