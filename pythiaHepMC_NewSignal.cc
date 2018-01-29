// pythiaHepMC.cc 
// Creating pythia events and save in HepMC format in /scr/bay
// Run example with ./pythiaHepMC N 50 1000000


// Now with misIdentification of pions - if without change
// both in naming the files and in the actual misID section.  

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"


// Generating pythia events and write to HepMC datafile
using namespace Pythia8;

int main(int argc, char **argv) {

  TString histfname;
  std::string dataPath;
  TRandom3 myRandomNumber(1);


  if ( argc<2 ) {
    printf("Must must enter option for process!\n");
    return 0;
  }


  int nEvent = 10000;
  cout << argc << endl;
  if ( argc==4 ) {
    sscanf (argv[3],"%d",&nEvent);
  }

  bool firstEvent = true;

  // Generator.
  Pythia pythia;

  ////////// Running a different random seed
  pythia.readString("Random:setSeed = on"); // Default = off
  pythia.readString("Random:seed = 0"); // Default = -1
  ///////////////////////////////////////////////////////

  // Allow no substructure in e+- beams: normal for corrected LEP data.
  pythia.readString("PDF:lepton = on"); 
  pythia.readString("SpaceShower:QEDshowerByL = on"); 
  pythia.readString("HadronLevel:all = on"); //
  pythia.readString("Beams:idA =  11");
  pythia.readString("Beams:idB = -11");
  double eCM =  91.2; //161; ///
  pythia.settings.parm("Beams:eCM", eCM );

  // Z -> nuN ->nu l qq', all hadronic states, flat phase space
  //} else 
  if ( argv[1][0] == 'N' ) {
    pythia.readString("WeakSingleBoson:ffbar2gmZ = on");
    char something[] = "5000000:new = N2 N2 2 0 0 xx. 0.0 0.0 0.0 0.0 0 1 0 1 0"; // before: tau0=100.0 no.4 after xx
    something[26] = argv[2][0];
    something[27] = argv[2][1];
    pythia.readString(something);
    pythia.readString("5000000:isResonance = false");
    pythia.readString("5000000:addChannel = 1 0.4652853137 91 -1 2 13");      // Du mu
    pythia.readString("5000000:addChannel = 1 0.2477664259e-1 91 -3 2 13");   // Su mu
    pythia.readString("5000000:addChannel = 1 0.8201559847e-5 91 -5 2 13");   // Bu mu  
    pythia.readString("5000000:addChannel = 1 0.2372986153e-1 91 -1 4 13");   // Dc mu
    pythia.readString("5000000:addChannel = 1 0.4853957884 91 -3 4 13");      // Sc mu
    pythia.readString("5000000:addChannel = 1 0.8041922597e-3 91 -5 4 13");   // Bc mu
    pythia.readString("5000000:mayDecay = on");
    pythia.readString("23:onMode = off");
    pythia.readString("23:addchannel = 1 1. 103 12 5000000"); // Z->nu_eN
    
    char HNL[] = "/scr/bay/testSample_newSignal_hepmcout_HNLxx_mu_tau0.dat";
    HNL[42] = argv[2][0];
    HNL[43] = argv[2][1];
    dataPath = HNL;
    char hist[] = "newSignal_histosHNLxx_mu_pythia_testSample_tau0.root";
    hist[19] = argv[2][0];
    hist[20] = argv[2][1];
    //  
    histfname = hist;
  } else {
    printf("Wrong option. Stopping\n");
    return 0;
  }

  pythia.init();


  ////////////////////////////////////////////////////////////////////////////////////////
  // Define histograms:
  TFile *histFile = new TFile(histfname,"RECREATE");

  TH1F* h_qq_vecProd_flat = new TH1F("h_qq_vecProd_flat","qq_vecProd_flat ;vecProd(q,q) [GeV^2] ;Events/10GeV^2",400,0.,4000.);
  TH1F* h_qq_vecProd_ME = new TH1F("h_qq_vecProd_ME","qq_vecProd_ME ;vecProd(q,q) [GeV^2] ;Events/10GeV",400,0.,4000.);
  TH1F* h_qq_invMass_flat = new TH1F("h_qq_invMass_flat","qq_invMass_flat ;M_{inv,qq} [GeV] ;Events/1GeV",100,0.,100.);
  TH1F* h_qq_invMass_ME = new TH1F("h_qq_invMass_ME","qq_invMass_ME ;M_{inv,qq} [GeV] ;Events/1GeV",100,0.,100.);
  TH1F* h_wtME = new TH1F("h_wtME","wtME ;wtME ;Events/1",400000,0.,4.);

  TH1F* h_Pmu_flat = new TH1F("h_Pmu_flat","Pmu_flat ; p_{#mu} [GeV] ;Events/1GeV",50,0.,50.);
  TH1F* h_Pmu_ME = new TH1F("h_Pmu_ME","Pmu_ME ;p_{#mu} [GeV];Events/1GeV",50,0.,50.);
  TH1F* h_Pmu_Nframe_flat = new TH1F("h_Pmu_Nframe_flat","Pmu_Nframe_flat ;p_{#mu} (Nframe) [GeV] ;Events/1GeV",50,0.,50.);
  TH1F* h_Pmu_Nframe_ME = new TH1F("h_Pmu_Nframe_ME","Pmu_Nframe_ME ;p_{#mu} (Nframe) [GeV] ;Events/1GeV",50,0.,50.);

  TH1F* h_cutFlow = new TH1F("h_cutFlow","cutFlow ;cutFlow ;Events/1",5,0.,5.);
  TH1F* h_P_Pion = new TH1F("h_P_Pion","P_Pion ;P_Pion ;Events/1GeV",50,0.,50.);
  TH1F* h_nChargSeen = new TH1F("h_nChargSeen","#Charg Seen ;#Charg Seen ;Events/1",60,0.,60.);
  TH1F* h_pMiss = new TH1F("h_pMiss","pMiss ;pMiss ;Events/1GeV",60,0.,60.);
  TH1F* h_nLepton_true = new TH1F("h_nLepton_true","#Leptons true ;#Leptons ;Events/1",10,0.,10.);
  TH1F* h_nLepton_fake = new TH1F("h_nLepton_fake","#Leptons fake ;#Leptons ;Events/1",10,0.,10.);
  TH1F* h_nLepton_cut = new TH1F("h_nLepton_cut","#Leptons ;#Leptons ;Events/1",10,0.,10.);
  TH1F* h_nPion_cut = new TH1F("h_nPion_cut","# Pions ;# Pions ;Events/1",20,0.,20.);
  TH1F* h_P_Pion_max = new TH1F("h_P_Pion_max","P_Pion_max ;P_Pion_max ;Events/1GeV",50,0.,50.);
  TH1F* h_P_Pion_max_afterCut = new TH1F("h_P_Pion_max_afterCut","P_Pion_max_afterCut ;P_Pion_max_afterCut ;Events/1GeV",50,0.,50.);
  ////////////////////////////////////////////////////////////////////////////////////////


  // Interface for conversion from Pythia8::Event to HepMC event.
  HepMC::Pythia8ToHepMC ToHepMC;
  HepMC::IO_GenEvent ascii_io( dataPath, std::ios::out );

  ////////////////////////////////////////////////////////////////////////////////////////
  // Define maximum values for matrix element for every signal
  ////////////////////////////////////////////////////////////////////////////////////////

  //double wtMEmax_event = 0.;
  
  double wtMEmax_Nxx[] = {2.e-5, 3.e-4, 2.e-3, 6.e-3, 2.e-2, 5.e-2, 2.e-1, 4. };

  int N_i = atoi(argv[2])/10;

  int i = N_i -1;

  double wtMEmax_N = wtMEmax_Nxx[ i ];


  ////////////////////////////////////////////////////////////////////////////////////////
  // Begin event loop. Generate event. Skip if error. List first few.
  ////////////////////////////////////////////////////////////////////////////////////////

  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

    if (!pythia.next()) continue;


    int nChargSeen = 0;
    int nLepton_cut = 0;
    int nPion_cut = 0;
    double P_Pion_max = 0.;

    TLorentzVector p4Prt = TLorentzVector(0.,0.,0.,0.);
    TLorentzVector p4Sum = TLorentzVector(0.,0.,0.,0.);
    TLorentzVector p4Lep = TLorentzVector(0.,0.,0.,0.);
    TLorentzVector p4Pion = TLorentzVector(0.,0.,0.,0.);


    ////////////////////////////////////////////////////////////////////////////////////////
    // MATRIX ELEMENT CALCULATION 
    ////////////////////////////////////////////////////////////////////////////////////////

    double mW2 = 80.385*80.385;
    double GammaW2 = 2.1*2.1;
    double wtMEmax = -1.;

    int ip_N = -1;
    double weight = -1;
    double qQ_vecProd;
    double qQ_invMass;
    TLorentzVector p_N = {0.,0.,0.,0.};
    TLorentzVector p_mu = {0.,0.,0.,0.};
    TLorentzVector p_Q = {0.,0.,0.,0.};
    TLorentzVector p_q = {0.,0.,0.,0.};
    TLorentzVector p_Qq = {0.,0.,0.,0.};
    TLorentzVector p_mu_Nframe = {0.,0.,0.,0.};

    ////////////////////////////////////////////////////////////////////////////////////////
    // Loop over pythia particles
    ////////////////////////////////////////////////////////////////////////////////////////
    for (int ip = 0; ip < pythia.event.size(); ++ip) {
      // Sterile neutrino, N
      if ( pythia.event[ip].idAbs() == 5000000 ) {
        ip_N = ip;
        p_N.SetPxPyPzE(pythia.event[ip].px(),
                       pythia.event[ip].py(),
                       pythia.event[ip].pz(),
                       pythia.event[ip].e() );
      } 
      // Muon from N
      if ( pythia.event[ip].mother1() == ip_N  &&  pythia.event[ip].idAbs() == 13 ){
        p_mu.SetPxPyPzE(pythia.event[ip].px(),
                       pythia.event[ip].py(),
                       pythia.event[ip].pz(),
                       pythia.event[ip].e() );
      } 
      // Antiquark from N
      if ( pythia.event[ip].mother1() == ip_N  &&  pythia.event[ip].idAbs() != 13  && pythia.event[ip].id() < 0){
        p_Q.SetPxPyPzE(pythia.event[ip].px(),
                       pythia.event[ip].py(),
                       pythia.event[ip].pz(),
                       pythia.event[ip].e() );
      }
      // Quark from N
      if ( pythia.event[ip].mother1() == ip_N  &&  pythia.event[ip].idAbs() != 13  && pythia.event[ip].id() > 0){
        p_q.SetPxPyPzE(pythia.event[ip].px(),
                       pythia.event[ip].py(),
                       pythia.event[ip].pz(),
                       pythia.event[ip].e() );
      } 
   
      // Try to swap Q and q

      double x_1 =  ( p_N * p_Q ); //p_N*p_Q 
      //double x_1 =  ( p_N * p_q ); //p_N*p_Q 

      double x_2 =  p_q * p_mu ;  // p_q*p_mu
      //double x_2 =  p_Q * p_mu ;  // p_q*p_mu

      double x_3 =  ( p_q + p_Q ) * ( p_q + p_Q )  ; //(p_q*p_Q)^2 

      qQ_vecProd = p_q * p_Q; // vector product of qq system for validation
      p_Qq = (p_q + p_Q);
      qQ_invMass = p_Qq.M(); // inv. mass of qq system for validation


      // Matrix-element expression 
      double wtME = ( x_1 * x_2 ) /
       (  pow2( x_3 - mW2 ) + ( mW2 + GammaW2) );

      if ( wtME > wtMEmax ) {
        wtMEmax = wtME;
        //cout << "wtME : " <<  wtME << endl;
      } 

      if ( wtME > wtMEmax_N ){
        cout << "Value of matrix-element larger than max value!!" << endl;
        break;
      }

      weight =  wtME / wtMEmax_N;

    ////////////////////////////////////////////////////////////////////////////////////////
    } // end of particle loop
    ////////////////////////////////////////////////////////////////////////////////////////

    // Fill values for matrix element
    if (wtMEmax > -0.5) {
      //cout << "wtME : " <<  wtME << endl;
      h_wtME -> Fill(wtMEmax);
    }
   

    /////////////////////////////////////////
    // Calc. mu momentum in N-CM-frame
    double beta_x = p_N.Px()/p_N.E();
    double beta_y = p_N.Py()/p_N.E();
    double beta_z = p_N.Pz()/p_N.E();

    // boost vector
    TVector3 b( - beta_x, - beta_y, - beta_z ); 

    p_mu_Nframe = p_mu;
    p_mu_Nframe.Boost(b);


    /////////////////////////////////////////
    // Flat phase space
    h_qq_vecProd_flat -> Fill(qQ_vecProd);
    h_qq_invMass_flat -> Fill(qQ_invMass);
    h_Pmu_flat -> Fill(p_mu.Rho());
    h_Pmu_Nframe_flat -> Fill( p_mu_Nframe.Rho() );
    

    /////////////////////////////////////////
    // Weight events with ME
    /////////////////////////////////////////
    double R = myRandomNumber.Rndm();
    // skip event with low weight randomly
    if (weight < R) {
      continue;
    }

    /////////////////////////////////////////
    // ME
    h_qq_vecProd_ME -> Fill(qQ_vecProd);
    h_qq_invMass_ME -> Fill(qQ_invMass);
    h_Pmu_ME -> Fill(p_mu.Rho());
    h_Pmu_Nframe_ME -> Fill( p_mu_Nframe.Rho() );


   ////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////
    // Loop over pythia particles
    ////////////////////////////////////////////////////////////////////////////////////////

    for (int ip = 0; ip < pythia.event.size(); ++ip) {


      if (!pythia.event[ip].isFinal() )                             continue;

      // Skip particles close to beam pipe
      if ( fabs(cos(pythia.event[ip].theta())) > 0.99 )             continue;

      // Skip soft particles  
      if ( sqrt( pow(pythia.event[ip].px(),2) +
                 pow(pythia.event[ip].py(),2) +
                 pow(pythia.event[ip].pz(),2)  ) < 0.1 )            continue;

      // Skip neutrinos
      if ( pythia.event[ip].idAbs() == 12 ||
           pythia.event[ip].idAbs() == 14 ||
           pythia.event[ip].idAbs() == 16   )                       continue;


      // Count charged particles
      if ( pythia.event[ip].isCharged() ) nChargSeen++;

      // Sum all particle momentum  //S
      p4Prt.SetPxPyPzE(pythia.event[ip].px(),
                       pythia.event[ip].py(),
                       pythia.event[ip].pz(),
                       pythia.event[ip].e() );
      p4Sum += p4Prt;


      // Count Leptons with p>3GeV
      if (//pythia.event[ip].idAbs() == 11 ||
          pythia.event[ip].idAbs() == 13  ) {
          //pythia.event[ip].idAbs() == 15    ){
          p4Lep.SetPxPyPzE(pythia.event[ip].px(),
                         pythia.event[ip].py(),
                         pythia.event[ip].pz(),
                         pythia.event[ip].e() );
        if (p4Lep.Rho() > 3) {                   
          nLepton_cut++;
        }
      }

      // Count pions with p>3GeV
      if ( pythia.event[ip].idAbs() == 211 ){
      	p4Pion.SetPxPyPzE(pythia.event[ip].px(),
                         pythia.event[ip].py(),
                         pythia.event[ip].pz(),
                         pythia.event[ip].e() );
      	h_P_Pion -> Fill(p4Pion.Rho());
      	// find p_max
      	if  (p4Pion.Rho() > P_Pion_max) {
      		P_Pion_max = p4Pion.Rho();
      	}
        if (p4Pion.Rho() > 3) {                   
          nPion_cut++;
        }
      }



    } // end of particle loop



    h_cutFlow->Fill(0);

    // Require more than 3 charged (visible) particles
    h_nChargSeen -> Fill(nChargSeen);
    if ( nChargSeen < 3 )                                         continue;
    h_cutFlow->Fill(1);

    // Require pMiss > 5GeV
    h_pMiss -> Fill(p4Sum.Rho());
    if ( p4Sum.Rho() < 5 )                                        continue;
    h_cutFlow->Fill(2);


    // plot true lepton
    if ( nLepton_cut > 0 ) {
    	h_nLepton_true -> Fill(nLepton_cut);
    }

   //////////////////////////////////////////////////////////////////////////// 
	// Save events w/ pions p>3GeV w/ e=0.25% if no leptons in event
    if ( nLepton_cut == 0 ) {
    	if ( myRandomNumber.Rndm() < nPion_cut*0.0025 ) {
    		nLepton_cut = 1;
    		h_nLepton_fake -> Fill(nLepton_cut);
    	}
    }
    //////////////////////////////////////////////////////////////////////////

    h_nLepton_cut -> Fill(nLepton_cut);
    h_nPion_cut -> Fill(nPion_cut);
    h_P_Pion_max -> Fill(P_Pion_max);
    // Require at least 1 lepton with p>3GeV
    if ( nLepton_cut < 1 )                                        continue;
    h_cutFlow->Fill(3);
    h_P_Pion_max_afterCut -> Fill(P_Pion_max);



    // Construct new empty HepMC event and fill it.
    // Units will be as chosen for HepMC build; but can be changed
    // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
    ToHepMC.fill_next_event( pythia, hepmcevt );

    // Write the HepMC event to file. Done with it.
    ascii_io << hepmcevt;
    delete hepmcevt;




  } // End of event loop. Statistics. Output histograms.

  /////////////////////////////
  // Scale histograms: 
  double norm_1 = h_Pmu_flat->GetEntries();
  h_Pmu_flat->Scale(1/norm_1);
  double norm_2 = h_Pmu_ME->GetEntries();
  h_Pmu_ME->Scale(1/norm_2);
  double norm_3 = h_Pmu_Nframe_flat->GetEntries();
  h_Pmu_Nframe_flat->Scale(1/norm_3);
  double norm_4 = h_Pmu_Nframe_ME->GetEntries();
  h_Pmu_Nframe_ME->Scale(1/norm_4);
  double norm_5 = h_qq_vecProd_flat->GetEntries();
  h_qq_vecProd_flat->Scale(1/norm_5);
  double norm_6 = h_qq_vecProd_ME->GetEntries();
  h_qq_vecProd_ME->Scale(1/norm_6);
  double norm_7 = h_qq_invMass_flat->GetEntries();
  h_qq_invMass_flat->Scale(1/norm_7);
  double norm_8 = h_qq_invMass_ME->GetEntries();
  h_qq_invMass_ME->Scale(1/norm_8);


  /////////////////////////////


  pythia.stat();

  histFile->Write();

  histFile->Close();




  return 0;

}






