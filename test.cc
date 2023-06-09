#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include <vector>
#include "checkRootData.cc"

std::tuple<std::vector<double>, std::vector<double>, std::vector<double>> readRootCT(const char* fileName)
{
    std::cout << "fileName: " << fileName << std::endl;

    std::vector<double> CTL1, CTL1Div, gunZ;

    if (checkRootData(fileName,"EndOfEvent","fphL")) {
        // setting branches and trees
        TFile* file = TFile::Open(fileName);
        TTree *treeEOE = (TTree*)file->Get("EndOfEvent");
        TBranch* branch_phL = treeEOE->GetBranch("fphL");
        TBranch* branch_primaryZ = treeEOE->GetBranch("fPrimaryZ");

        TTree *treeD = (TTree*)file->Get("Detected");
        TBranch* branch_HX = treeD->GetBranch("fX");
        TBranch* branch_HZ = treeD->GetBranch("fZ");
        TBranch* branch_Evt = treeD->GetBranch("fEvent");

        // Set address of each reading variable
        double fX,fZ,evt,fPrimaryZ,fphL;
        branch_HX->SetAddress(&fX);
        branch_HZ->SetAddress(&fZ);
        branch_Evt->SetAddress(&evt);
        branch_primaryZ->SetAddress(&fPrimaryZ);
        branch_phL->SetAddress(&fphL);

        // calculating Cross-Talk ph by entries
        CTL1.resize(treeD->GetEntries());
        for (int i = 0; i < treeD->GetEntries(); i++) {
            treeD->GetEntry(i);
            if (fX > -6.4 && fX < -3.2 && fZ > 0) {
                CTL1[evt] += 1;
            }                                            
        }           
            
        // calculating Cross-Talk percentage by entries
        CTL1Div.resize(treeEOE->GetEntries());
        gunZ.resize(treeEOE->GetEntries());
        for (int i = 0; i < treeEOE->GetEntries(); i++) {
            treeEOE->GetEntry(i);
            CTL1Div[i] = CTL1[i] / fphL;       
            gunZ[i] = fPrimaryZ * 1000;                                  
        }            

        file->Close();
        return std::make_tuple(CTL1, CTL1Div, gunZ);
    } else {
        std::cout << "Something wrong with the file!!!" << std::endl;
        return std::make_tuple(CTL1, CTL1Div, gunZ);
    }
}
