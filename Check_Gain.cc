#include "Check_Gain.hh"

#include <TCanvas.h>
#include <TStyle.h>

#include <iostream>
#include <cstring>
#include <string>
#include <dirent.h>
#include <sys/stat.h>

#include <histogram_functions.hh>

Check_Gain::Check_Gain(const int nFiles)
{
    if(chdir(analysis_result_DIR.c_str()) == -1) {
      std::cerr << std::endl << "Error in chdir: " << strerror(errno) << std::endl;
      std::cerr << "Setup variable (and path) analysis_result_DIR!" << std::endl;
      return;
    }

    g_meanGainValues = new TGraph(nFiles);
    h_meanGainValues = new TH1D("h_meanGainValues","h_meanGainValues", 50, 46., 50.);
}

Check_Gain::~Check_Gain()
{
    delete h_meanGainValues;
    delete g_meanGainValues;
}

void Check_Gain::AddData(TH1D* h_gain_allchannels)
{
    vec_h_gain_allchannels.push_back(h_gain_allchannels);
    return;
}

void Check_Gain::Evaluate()
{
    for (unsigned int i=0; i<vec_h_gain_allchannels.size(); ++i)
    {
        double meanGain = HISTOGRAM_FUNCTION::meanValue(vec_h_gain_allchannels.at(i));
        g_meanGainValues->SetPoint(i, i, meanGain);
        h_meanGainValues->Fill(meanGain);

        TH1D h_gain("h_gain", Form("h_gain_it_%d",i), 50, 40., 60.);
        int n = h_gain.GetNbinsX();
        double* channel = &(vec_h_gain_allchannels.at(i)->GetArray()[1]);
        for (int j=0; j<n; ++j)
          h_gain.Fill(channel[j]);

        TCanvas* c = new TCanvas();
        c->cd();
        h_gain.Draw();
        std::string outfilePath = analysis_result_DIR + std::string(Form("/gain_allchannels_it_%d.pdf",i));
        c->SaveAs(outfilePath.c_str(), "Overwrite");
        c->Close();
        delete c;
    }

    TCanvas* c1 = new TCanvas("c_g_meanGainValues", "c_g_meanGainValues");
    c1->cd();
    g_meanGainValues->SetTitle("g_meanGainValues");
    g_meanGainValues->Draw();
    std::string outfilePath = analysis_result_DIR + std::string("/g_meanGainValues.pdf");
    c1->SaveAs(outfilePath.c_str(), "Overwrite");
    c1->Close();
    delete c1;

    TCanvas* c2 = new TCanvas("c_h_meanGainValues", "c_h_meanGainValues");
    c2->cd();
    gStyle->SetOptStat("mreuon");
    h_meanGainValues->Draw();
    outfilePath = analysis_result_DIR + std::string("/h_meanGainValues.pdf");
    c2->SaveAs(outfilePath.c_str(), "Overwrite");
    c2->Close();
    delete c2;
}

