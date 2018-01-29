//  MyUtils.cc
// compile with: cl /c /EHsc MathFuncsLib.cpp
// post-build command: lib MathFuncsLib.obj

#include <stdlib.h>
#include "MyUtils.h"
#include <stdexcept>
#include "TLorentzVector.h"
#include "TMath.h"
#include "TRandom.h"

using namespace std;

namespace MyUtils 
{
  ////////////////////////////////////////////////////////////////////////////////
  //My particles 
  ////////////////////////////////////////////////////////////////////////////////

  bool MyParticles::IsNeutrino(int pid) {
    if ( abs(pid) == 12 || abs(pid) == 14 || abs(pid) == 16 ) return true;
    else return false;
  }

  bool MyParticles::IsGamma(int pid) {
    if ( abs(pid) == 22 ) return true;
    else return false;
  }

  bool MyParticles::IsNeutron(int pid) {
    if ( abs(pid) == 2112 ) return true;
    else return false;
  }

  bool MyParticles::IsK0(int pid) {
    if ( abs(pid) == 130 || abs(pid) == 310 || abs(pid) == 311 ) return true;
    else return false;
  }

  bool MyParticles::IsPi0(int pid) {
    if ( abs(pid) == 111 || abs(pid) == 113 || abs(pid) == 221 ) return true;
    else return false;
  }

  bool MyParticles::IsChargedPion(int pid) {
    if ( abs(pid) == 211 ) return true;
    else return false;
  }

  bool MyParticles::IsElectron(int pid) {
    if ( abs(pid) == 11 ) return true;
    else return false;
  }

  bool MyParticles::IsMuon(int pid) {
    if ( abs(pid) == 13 ) return true;
    else return false;
  }

  bool MyParticles::IsLepton(int pid){
    if (IsElectron(pid) || IsMuon(pid)) return true;
    else return false; 
  }

  bool MyParticles::IsChargedParticle(int pid) {
    if ( IsNeutrino(pid) || IsGamma(pid) || IsNeutron(pid) || IsK0(pid) || IsPi0(pid) ) return false;
    else return true;
  } 

  bool MyParticles::IsHadron( int pid ) {
    if ( IsNeutrino(pid) || IsGamma(pid) || IsLepton(pid) ) return false;
    else return true;
  }

  ////////////////////////////////////////////////////////////////////////////////
    // My functions
  ////////////////////////////////////////////////////////////////////////////////

  double MyFuncs::Min(double x, double y) {
    if ( x < y ) return x;
    else return y;
  }

  double MyFuncs::Max(double x, double y) {
    if ( x > y ) return x;
    else return y;
  }

  double MyFuncs::Min_3(double x, double y, double z) {
    if ( x < y && x < z ) return x;
    else if ( y < x && y < z ) return y;
    else return z;
  }

  double MyFuncs::Max_3(double x, double y, double z) {
    if ( x > y && x > z ) return x;
    else if ( y > x && y > z ) return y;
    else return z;
  }

  double MyFuncs::CosT( TLorentzVector a, TLorentzVector b ) {
    return ( a.Vect().Unit() * b.Vect().Unit() );
  }

  double MyFuncs::Square(double a) {
    double b = a*a;
    return  b;
  }


  ////////////////////////////////////////////////////////////////////////////////
  // My detector
  ////////////////////////////////////////////////////////////////////////////////

  // Calculate the smearing function dpT/pT for Tracker, Ecal & Hcal:
  double MyDetector::Tracker( double pT ) {
    double a = 1e-3; //const.
    double b = 2e-5; //noise
    double dpT = sqrt( MyFuncs::Square(a) + MyFuncs::Square(b*pT) );
    dpT =  1 + dpT * gRandom->Gaus(0,1) ;
    return dpT;
  }

  double MyDetector::Ecal( double pT ) {
    double a = 0.011; //const.
    double b = 0.165; //stoch
    double c = 0.010; //noise
    double dpT = sqrt( MyFuncs::Square(a) + MyFuncs::Square(b/sqrt(pT)) + MyFuncs::Square(c/pT) );
    dpT =  1 + dpT * gRandom->Gaus(0,1) ;
    return dpT;
  }

  double MyDetector::Hcal( double pT ) {
    double a = 0.016; //const.
    double b = 0.58; //stoch
    double c = 0.18; //noise
    double dpT = sqrt( MyFuncs::Square(a) + MyFuncs::Square(b/sqrt(pT)) + MyFuncs::Square(c/pT) );
    dpT =  1 + dpT * gRandom->Gaus(0,1) ;
    return dpT;
  }

  // Detector efficiency = 0.995 -> use only for the tracker!!
  bool MyDetector::NotDetected() {
    if ( gRandom->Rndm()>0.995 ) return true;
    else return false;
  }
  
  // Appeptance |eta|< 2.79  (sim |theta| > 7.02) 
  bool MyDetector::NotAccepted(double eta) {
    if ( eta > 2.79 ) return true;
    else return false;
  }


} // end of class

