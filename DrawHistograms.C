//needed root classes
#include "Riostream.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TMath.h"
#include "TStyle.h"
#include "TDirectory.h"

// //needed macros
void SetCanvasStyle(TCanvas* can);

void DrawHistograms()
{
    TString variableName = "charge";
    TString configName = "1";
    TString cDataPath = "ResultsHistos";
  const Int_t cNumberOfChannels = 4;
  Int_t channelID = 2;
  TString cPDFResultsPath = "ResultsPlots";
  gStyle->SetOptStat(0);

  //-------------------------------------------------------------------------------------------------------------------------------------//
  //This is the input file containing the histograms for each configuration. The input corresponds to the output of FillHistograms.C
  
  //-------------------------------------------------------------------------------------------------------------------------------------//
  
  TFile* inputFile = new TFile(Form("%s/AnalysisResults_%s.root", cDataPath.Data(), configName.Data()));
  //-------------------------------------------------------------------------------------------------------------------------------------//
  //Ploting...
  TCanvas *canMain = new TCanvas(Form("can_%s",variableName.Data()),"", 1800,1200);
  SetCanvasStyle(canMain);

  
    TH1F *histoSomethingVsChannel = ((TH1F*) inputFile->Get(Form("%s/histo%s_Channel%d",variableName.Data(),variableName.Data(),channelID)));
    histoSomethingVsChannel->Draw("COLZ");
  
  //Save as pdf
  //Make the name using the arguments of the function
  TString pdfFileName;
  pdfFileName.Form("%s/%s_%s.pdf",cPDFResultsPath.Data(),variableName.Data(),configName.Data());
  canMain->SaveAs(pdfFileName.Data());
  //-------------------------------------------------------------------------------------------------------------------------------------//


}

void SetCanvasStyle(TCanvas* can) {
    // gStyle->SetOptStat(0);
    int font = 42;
    // gROOT->SetStyle("Plain");
    gStyle->SetTitleFillColor(0);
    gStyle->SetTitleBorderSize(0);
    gStyle->SetLegendBorderSize(1);
    gStyle->SetTextFont(font);
    gStyle->SetTickLength(0.02, "y");
    gStyle->SetTickLength(0.02, "x");
    can->SetTickx();
    can->SetTicky();
    gStyle->SetLabelSize(0.04, "xyz");
    gStyle->SetLabelFont(font, "xyz");
    gStyle->SetLabelOffset(0.01, "xyz");
    gStyle->SetTitleFont(font, "xyz");
    gStyle->SetTitleOffset(1.1, "xy");
    gStyle->SetTitleSize(0.04, "xyz");
    gStyle->SetMarkerSize(1.1);
    gStyle->SetLineWidth(1);
    gStyle->SetLegendFont(42);
    gStyle->SetLegendBorderSize(0);
    gStyle->SetLegendFillColor(10);
    gStyle->SetEndErrorSize(0);
    can->SetFillColor(0);
    can->SetBorderMode(0);
    can->SetBorderSize(0);
    can->SetLeftMargin(0.09);
    can->SetRightMargin(0.11);
    can->SetBottomMargin(0.1518219);
    // can->SetTopMargin(0.);
    can->SetFrameBorderMode(0);
}