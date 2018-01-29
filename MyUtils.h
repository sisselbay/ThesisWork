// MyUtils.h

#include "TLorentzVector.h"
#include <iostream>

namespace MyUtils
{
	class MyParticles
  {
  public:

   static bool IsNeutrino(int pid);

   static bool IsGamma(int pid);

   static bool IsNeutron(int pid);

   static bool IsK0(int pid);

   static bool IsPi0(int pid);

   static bool IsChargedPion(int pid);

   static bool IsElectron(int pid);

   static bool IsMuon(int pid);

   static bool IsLepton(int pid);

   static bool IsChargedParticle(int pid);

   static bool IsHadron(int pid);

 };

 class MyFuncs
 {
 public:

  static double Min(double x, double y);

  static double Max(double x, double y);

  static double Min_3(double x, double y, double z);

  static double Max_3(double x, double y, double z);

  static double CosT( TLorentzVector a, TLorentzVector b );

  static double Square(double a);

 };

 class MyDetector
 {
 public:

  static double Tracker(double pT);

  static double Ecal(double pT);

  static double Hcal(double pT);

  static bool NotDetected();

  static bool NotAccepted(double eta);


 };


}

