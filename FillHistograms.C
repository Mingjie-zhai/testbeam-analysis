
#include "Riostream.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TString.h"
#include "TMath.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TGraphErrors.h"

void SetHistoStyle(TH1F* histoSomething, Int_t color, Int_t markerStyle, Float_t markerSize, TString xAxisTitle, TString yAxisTitle);

int FillHistograms()
{
    TString configName = "1";
    TString cTreeNameInInputFile = "tree";
    TString cDataPath = "../../testbeamanalysis/Data";
    TString cHistoResultsPath = "ResultsHistos";
    TString cPDFResultsPath = "ResultsPlots";
    
    //-------------------------------------------------------------------------------------------------------------------------------------//
    const Int_t cNumberOfChannels = 4;
    const Int_t cNumberOfIntBranches = 6;
    const Int_t cNumberOfFloatBranches = 27;

    const TString cTreeIntBranches[cNumberOfIntBranches] = { "FEI4x", "FEI4y", "runNumber", "eventNumber", "cycleNumber", "nChannel" };
    const TString cTreeFloatBranches[cNumberOfFloatBranches] = { "pulseHeightNoise", "pulseHeight", "timeCFD", "timeZCD", "timeCFD20", "timeCFD70", "timeZCD20", "timeCFD50", "timeZCD50", "timeCTD", "timeAtMax", "pedestal", "noise", "noiseZCD", "charge", "jitter", "jitterCFD20", "jitterCFD50", "jitterCTD", "riseTime1090", "derivative", "derivativeCFD20", "derivativeCFD50", "derivativeCTD", "derivativeZCD", "width", "TOT" };
   

    //Declare here the bining (min,max,and nBins) of your histograms. each triplet corresponds to the variable with the same index in cTreeFloatBranches (Is this english !!!!!!?).
    enum enumBining { kNBins, kMinBin, kMaxBin };
    const Float_t cHistogramsBins[cNumberOfFloatBranches][3] = {
      {1200,-100,300},//pulseHeightNoise
      {1000,0,100},////pulseHeight
      {11000,0,220000},//timeCFD
      {11000,0,220000},//timeZCD
      {11000,0,220000},//timeCFD20
      {11000,0,220000},//timeCFD70
      {11000,0,220000},//timeZCD20
      {11000,0,220000},//timeCFD50
      {11000,0,220000},//timeZCD50
      {11000,0,220000},//timeCTD
      {11000,0,220000},//timeAtMax
      {2000,-500,500},//pedestal
      {3000,0,300},//noise
      {3000,0,300},//noiseZCD
      {1100,-100000,1000000},//charge
      //{10000,-100,100},//charge
      {10000,-500000,3000000},//jitter
      {10000,-500000,3000000},//jitterCFD20
      {10000,-500000,3000000},//jitterCFD50
      {10000,-500000,3000000},//jitterCTD
      {10000,0,10000},//riseTime1090
      {1000,-2,2},//derivative
      {1000,-2,2},//derivativeCFD20
      {1000,-2,2},//derivativeCFD50
      {1000,-2,2},//derivativeCTD
      {1000,0,1000},//derivativeZCD
      {1000,0,1000},//width
      {10000,-300000,50000}//TOT
    };
    
    
    

      //This is the input file containing the tree for each configuration (merged runs)
    TFile* inputFile = new TFile(Form("%s/%s.root", cDataPath.Data(), configName.Data()));
    TTree* testBeamEventTree = ((TTree*)inputFile->Get(cTreeNameInInputFile));
    // testBeamEventTree->SetBranchAddress("event", &event);
    Long_t numberOfEntries = testBeamEventTree->GetEntries();
    //-------------------------------------------------------------------------------------------------------------------------------------//


    //-------------------------------------------------------------------------------------------------------------------------------------//
    //Define an array of integer in order to get the integer branches of the tree. This can be done in a more intelligent way if you just want to plot them but this way you are saving the data for later use..
    Int_t arrayIntBranches[cNumberOfIntBranches];
    for (Int_t iBranch = 0; iBranch < cNumberOfIntBranches; iBranch++) {
        testBeamEventTree->SetBranchAddress(cTreeIntBranches[iBranch], &arrayIntBranches[iBranch]);
    }

    //Similarly define an array of floats in order to get the float branches of the tree. The floats themselves are arrays -> 2d arrays..
    Float_t arrayFloatBranches[cNumberOfFloatBranches][cNumberOfChannels];
    for (Int_t iBranch = 0; iBranch < cNumberOfFloatBranches; iBranch++) {
        testBeamEventTree->SetBranchAddress(cTreeFloatBranches[iBranch], &arrayFloatBranches[iBranch]);
    }


    //-------------------------------------------------------------------------------------------------------------------------------------//
    //Here define the histograms. For the moment, only 1D histograms for each array entry is defined. Correlations histograms (nD) can be added after similarly
    //The bining of the histograms is defined in the external macro CommonParams.C
    TH1F*** histoVariablesPerChannel = new TH1F * *[cNumberOfFloatBranches];
    for (Int_t iBranch = 0; iBranch < cNumberOfFloatBranches; iBranch++) {
        histoVariablesPerChannel[iBranch] = new TH1F * [cNumberOfChannels];
        for (Int_t iChannel = 0; iChannel < cNumberOfChannels; iChannel++) {
            histoVariablesPerChannel[iBranch][iChannel] = new TH1F(Form("histo%s_Channel%d", cTreeFloatBranches[iBranch].Data(), iChannel), "", cHistogramsBins[iBranch][kNBins], cHistogramsBins[iBranch][kMinBin], cHistogramsBins[iBranch][kMaxBin]);
            histoVariablesPerChannel[iBranch][iChannel]->Sumw2();
            histoVariablesPerChannel[iBranch][iChannel]->SetTitle(Form("%s distribution in channel %d ", cTreeFloatBranches[iBranch].Data(), iChannel));
            SetHistoStyle(histoVariablesPerChannel[iBranch][iChannel], kBlack, kFullCircle, 0.5, cTreeFloatBranches[iBranch], "counts");
        }
    }

   
    //Loop over the tree entries, get the branches and fill the histograms
    for (Long_t iEntry = 0; iEntry < numberOfEntries; iEntry++) 
    {
        testBeamEventTree->GetEntry(iEntry);

       
        //Fill all simple histograms.
        //TODO: Add something somwhere in order to specify the histograms to be filled. Currently every one is filled
        for (Int_t iBranch = 0; iBranch < cNumberOfFloatBranches; iBranch++) 
        {
            for (Int_t iChannel = 0; iChannel < cNumberOfChannels; iChannel++) 
            {
                histoVariablesPerChannel[iBranch][iChannel]->Fill(arrayFloatBranches[iBranch][iChannel]);
            }
        }


    }//End loop over tree entries


    //-------------------------------------------------------------------------------------------------------------------------------------//
    //Create and output file and store the histograms:
    //Directory per channel or directory per variable ??
    
    TString outputFileName = Form("%s/AnalysisResults_", cHistoResultsPath.Data());
    outputFileName.Append(Form("%s.root", configName.Data()));
    TFile* outputHistosFile = new TFile(outputFileName, "RECREATE");
    //An array of directories per variable
    TDirectory* arrayOfDirectories[cNumberOfFloatBranches];
    for (Int_t iBranch = 0; iBranch < cNumberOfFloatBranches; iBranch++) {
        arrayOfDirectories[iBranch] = outputHistosFile->mkdir(Form("%s", cTreeFloatBranches[iBranch].Data()));
        arrayOfDirectories[iBranch]->cd();
        for (Int_t iChannel = 0; iChannel < cNumberOfChannels; iChannel++) {
            histoVariablesPerChannel[iBranch][iChannel]->Write();
        }
    }
    outputHistosFile->Close();
    cout << Form("\n Done reading the tree for the configuration %s", configName.Data()) << endl;
    //-------------------------------------------------------------------------------------------------------------------------------------//
    return 0;
};



void SetHistoStyle(TH1F* histoSomething, Int_t color, Int_t markerStyle, Float_t markerSize, TString xAxisTitle, TString yAxisTitle)
{
    histoSomething->SetLineColor(color);
    histoSomething->SetMarkerColor(color);
    histoSomething->SetMarkerStyle(markerStyle);
    histoSomething->SetMarkerSize(markerSize);
    histoSomething->GetXaxis()->SetLabelSize(0.025);
    histoSomething->GetXaxis()->SetTitleSize(0.035);
    histoSomething->GetXaxis()->SetTitleOffset(1);
    histoSomething->GetXaxis()->SetTitle(xAxisTitle);
    histoSomething->GetYaxis()->SetLabelSize(0.035);
    histoSomething->GetYaxis()->SetTitleSize(0.035);
    histoSomething->GetYaxis()->SetTitleOffset(1.35);
    histoSomething->GetYaxis()->SetTitle(yAxisTitle);
}
