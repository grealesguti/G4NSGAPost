#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TLegend.h"
#include <vector>
#include "readRootDataFromFolder.cc"

void plotHistRootData(const std::string& folder, const std::string& treeName, const std::string& branchName, const std::string& plotName, double xMin = -1, double xMax = -1, int nBins = 100, const std::string& saveFolder = ".")
{
    // Read data from root file
    std::vector<double> data = readRootDataFromFolder(folder, treeName, branchName);

    // Create histogram
    TH1D* hist = new TH1D("hist", "", nBins, xMin, xMax);
    for (double d : data)
        hist->Fill(d);

    // Set histogram style
    hist->SetLineWidth(2);
    hist->SetLineColor(kBlue);
    hist->GetXaxis()->SetTitle("X");
    hist->GetYaxis()->SetTitle("Y");
    hist->SetStats(0);

    // Create canvas
    TCanvas* c = new TCanvas("c", "", 800, 600);
    c->SetTopMargin(0.05);
    c->SetBottomMargin(0.13);
    c->SetLeftMargin(0.16);
    c->SetRightMargin(0.05);

    // Draw histogram
    hist->Draw("HIST");

    // Save plot to pdf file
    std::cout << "Save folder: "<< saveFolder << std::endl;

    std::cout << "Save path: "<< plotName + ".pdf" << std::endl;

    c->SaveAs((saveFolder + plotName + ".pdf").c_str());
    c->SaveAs((saveFolder + plotName + ".tex").c_str());

    delete c;
    delete hist;
}
