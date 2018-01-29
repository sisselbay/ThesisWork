#Comparison between Z->hadhad and Z->Nnu for mN[50,80]GeV

from __future__ import division
from ROOT import *
from math import *
from array import array
import numpy as np



# Define the colors
Black = kBlack
White = kWhite
Red = kRed+1
Violet = kViolet
Magenta = kMagenta+2
Blue = kAzure+1
Purple = kBlue-7
Green1 = kGreen+4
Green2 = kGreen+3
Green3 = kGreen+2
Green4 = kGreen-3
Green5 = kSpring-9
Green6 = kSpring+5
Green7 = kGreen-8
Green8 = kSpring+1
Green9 = kGreen-10
Green10 = kTeal-9
Gray = kGray
Brown = kOrange+3
Orange = kOrange+7 # +1
Yellow = kOrange  #kYellow
Pink = kPink+9



m_N = [ 30, 50, 80]
color = [ Orange, Red, Blue]

N_files = [] # list
#N_filesD = {}  # dict.


path_files = "/Users/bay/Pythia_Workdir/hepmc_read/Histos/Histos_11-14/"
path_plots = "./CombinedPlots/CombinedPlots_11-14/HM_arrow/"

#NS_path_files = "/Users/bay/Pythia_Workdir/hepmc_read/Histos/Histos_07-25/NewSignal/"


# Import files ------------------------------------------------------------------
# Background:
Z_histos = TFile.Open(path_files+"histosZ_det_misID.root")
tau_histos = TFile.Open(path_files+"histosZtautau_misID.root")
mu_histos = TFile.Open(path_files+"histosZmumu_misID.root")
ff_histos = TFile.Open(path_files+"histos_whiz_4fermion.root")
uU_histos = TFile.Open(path_files+"histos_whiz_2phot_uU.root")
dD_histos = TFile.Open(path_files+"histos_whiz_2phot_dD.root")
cC_histos = TFile.Open(path_files+"histos_whiz_2phot_cC.root")
sS_histos = TFile.Open(path_files+"histos_whiz_2phot_sS.root")
bB_histos = TFile.Open(path_files+"histos_whiz_2phot_bB.root")


bkg_color = [ Green10, Green8, Green6, Green3, Green1 ]

# Signal:
for mass in m_N:
    N_files.append(TFile.Open(path_files+"histosHNL{:}_mu_det_NewSignal_misID_tau0.root".format(mass))) #list



SavePlots = True


###################################################################################################

###################################################################################################

histograms  = [ "h_cut2_nLepton_cut", "h_cut1_nLepton_cut", "h_cut1_nLepton_cut_true", "h_cut1_nLepton_cut_fake", 
"h_cut2_pMiss_absCost", "h_cut3_cos_pMissLep", "h_cut4_jetE_min", "h_cut5_cos_jj", 
"h_cut6_cos_jlep_max_3", "h_cut6_cos_jlep_max", "h_cut6_cos_jlep_max_NOT3", 
"h_cut7_thetaSum_jj_3", "h_cut8_jetMass_min", 
"h_cut9_pMiss", "h_cut9_mtot",
"h_cut10_N30_mtot", "h_cut11_N30_mtot",
"h_cut10_N40_mtot", "h_cut11_N40_mtot",
"h_cut10_N50_mtot", "h_cut11_N50_mtot",
"h_cut10_N60_mtot", "h_cut11_N60_mtot",
"h_cut10_N70_mtot", "h_cut11_N70_mtot",
"h_cut10_N80_mtot", "h_cut11_N80_mtot"] 


#------------------------------------------------------------------------
# Begin loop over all histograms:
#------------------------------------------------------------------------
for i in range( len(histograms) ):

  # Get files and stack 2phot:
  h_uU_i = uU_histos.Get( histograms[i] )
  h_dD_i = dD_histos.Get( histograms[i] )
  h_cC_i = cC_histos.Get( histograms[i] )
  h_sS_i = sS_histos.Get( histograms[i] )
  h_bB_i = bB_histos.Get( histograms[i] )

  hsum_2phot_i = h_uU_i
  hsum_2phot_i.Add(h_dD_i)
  hsum_2phot_i.Add(h_cC_i)
  hsum_2phot_i.Add(h_sS_i)
  hsum_2phot_i.Add(h_bB_i)


  # Get files and stack all bkg histos:
  hs_i = THStack('histStack', 'histStack')
  h_Z_i = Z_histos.Get( histograms[i] )
  h_tau_i = tau_histos.Get( histograms[i] )
  h_mu_i = mu_histos.Get( histograms[i] )
  h_ff_i = ff_histos.Get( histograms[i] )
  h_ff_i.Scale(1/1000)

  bkg_histos = [ h_ff_i, h_mu_i, hsum_2phot_i, h_tau_i, h_Z_i]


  # Stack Bkg histos:
  for k in range( len(bkg_histos) ):
    h_bkg_i = bkg_histos[k]
    if (bkg_histos[k].GetEntries() == 0):
      continue
    h_bkg_i.SetLineWidth(2)
    h_bkg_i.SetLineColor(bkg_color[k])
    h_bkg_i.SetFillColor(bkg_color[k])
    hs_i.Add(h_bkg_i)
  





###################################################################################################
  # Draw 
###################################################################################################

  canvas = TCanvas('canvas', 'i')
  canvas.SetLogy()
  #canvas.SetGridx()

  # Set tickmarks
  gStyle.SetPadTickX(1);
  gStyle.SetPadTickY(1);
  gStyle.SetOptStat("");




  
  Ymax =  hs_i.GetMaximum()
  h_ff_i.Draw()
  hs_i.Draw("same hist")

  legend = TLegend(0.45, 0.75, 0.6, 0.87)     #TLegend(x1,y1,x2,y2)
  legend2 = TLegend(0.6, 0.75, 0.75, 0.87)     #TLegend(x1,y1,x2,y2)
  legend3 = TLegend(0.75, 0.73, 0.87, 0.87)


  legend.SetFillColorAlpha( White, 0.0)#25 )
  legend.SetLineColor(White)
  legend2.SetFillColorAlpha( White, 0.0)#25 )
  legend2.SetLineColor(White)
  legend3.SetFillColorAlpha( White, 0.0)#25 )
  legend3.SetLineColor(White)


  legend.AddEntry(h_Z_i, 'Z #rightarrow qq ' , "f")
  legend.AddEntry(h_tau_i, 'Z #rightarrow #tau #tau' , "f")
  legend2.AddEntry(h_uU_i, 'eeqq' , "f")
  legend2.AddEntry(h_mu_i, 'Z #rightarrow #mu #mu' , "f")



  h_ff_i.GetXaxis().SetLabelSize(0.045);
  h_ff_i.GetXaxis().SetTitleSize(0.045);
  h_ff_i.GetXaxis().SetTitleOffset(0.95);
  h_ff_i.GetYaxis().SetLabelSize(0.045);
  h_ff_i.GetYaxis().SetTitleSize(0.045);
  h_ff_i.GetYaxis().SetTitleOffset(0.9);
  h_ff_i.SetTitle("");						


  #Looping over all N mass files
  for f in range( len(N_files) ):
    h_N_i = N_files[f].Get( histograms[i] )

    h_N_i.SetLineColor( color[f] )
    h_N_i.SetLineWidth(3)
    h_N_i.SetMarkerStyle(3)
    h_N_i.SetMarkerSize(0.1)
    h_N_i.SetMarkerColor(color[f])

    if (h_N_i.GetMaximum() > Ymax):
      Ymax = h_N_i.GetMaximum()

    h_ff_i.SetMaximum(Ymax*20)
    h_ff_i.SetMinimum(0.5)#1

    h_N_i.Draw("same")
    legend3.AddEntry(h_N_i, 'N ('+str(m_N[f])+')', "f")
    legend3.Draw("same")




  gPad.RedrawAxis()
  legend.Draw("same")
  legend2.Draw("same")
  gPad.Update()



#     #################    ############################################


  if (SavePlots) : canvas.SaveAs(path_plots+histograms[i]+".pdf)")


