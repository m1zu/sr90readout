#include "GainRepeatability.hh"

#include "histogram_functions.hh"

#include <TCanvas.h>
#include <TStyle.h>

#include <iostream>
#include <cstring>
#include <string>
#include <dirent.h>
#include <sys/stat.h>

GainRepeatability::GainRepeatability(const int nFiles)
{
    std::string analysis_dir = "/home/iwanicki/sw/sr90readout/results/";
    std::string result_folder = "gainRepeatability";

    if(chdir(analysis_dir.c_str()) == -1) {
      std::cerr << std::endl << "Error in chdir: " << strerror(errno) << std::endl;
      std::cerr << "Setup variable analysis_dir!" << std::endl;
      return;
    }
    else if(!(mkdir(result_folder.c_str(), 0777) == -1)) {
      std::cout << "Created gainRepeatability in analysis_dir: " << std::endl;
    }

    g_meanGainValues = new TGraph(nFiles);
    h_meanGainValues = new TH1D("h_meanGainValues","h_meanGainValues", 40, 48., 48.5);
}

GainRepeatability::~GainRepeatability()
{
    delete g_meanGainValues;
    delete h_meanGainValues;
}

void GainRepeatability::AddData(TH1D* h_gain_allchannels)
{
    vec_h_gain_allchannels.push_back(h_gain_allchannels);
    return;
}

void GainRepeatability::Evaluate()
{
    for (unsigned int i=0; i<vec_h_gain_allchannels.size(); ++i)
    {
        double meanGain = HISTOGRAM_FUNCTION::meanValue(vec_h_gain_allchannels.at(i));
        g_meanGainValues->SetPoint(i, i, meanGain);
        h_meanGainValues->Fill(meanGain);

        TCanvas* c = new TCanvas();
        c->cd();
        vec_h_gain_allchannels[i]->Draw();
        c->SaveAs(Form("~/sw/sr90readout/results/gainRepeatability/gain_allchannels_file_%d.pdf",i), "Overwrite");
        c->Close();
        delete c;
    }

    TCanvas* c1 = new TCanvas("c_g_meanGainValues", "c_g_meanGainValues");
    c1->cd();
    g_meanGainValues->SetTitle("g_meanGainValues");
    g_meanGainValues->Draw();
    c1->SaveAs("~/sw/sr90readout/results/gainRepeatability/g_meanGainValues.root", "Overwrite");
    c1->SaveAs("~/sw/sr90readout/results/gainRepeatability/g_meanGainValues.pdf", "Overwrite");
    c1->Close();
    delete c1;

    TCanvas* c2 = new TCanvas("c_h_meanGainValues", "c_h_meanGainValues");
    c2->cd();
    gStyle->SetOptStat("mreuon");
    h_meanGainValues->Draw();
    c2->SaveAs("~/sw/sr90readout/results/gainRepeatability/h_meanGainValues.root", "Overwrite");
    c2->SaveAs("~/sw/sr90readout/results/gainRepeatability/h_meanGainValues.pdf", "Overwrite");
    c2->Close();
    delete c2;
}
