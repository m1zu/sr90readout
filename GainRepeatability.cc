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
    h_singleCh_gainDist = new TH1D("h_singleCh_gainDist","h_singleCh_gainDist", 40, 45., 48.5);
}

GainRepeatability::~GainRepeatability()
{
    delete g_meanGainValues;
    delete h_meanGainValues;
    delete h_singleCh_gainDist;
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

        std::vector<double> gain_in;
        for (int i=1; i<=vec_h_gain_allchannels.at(i)->GetNbinsX(); ++i)
            gain_in.push_back(vec_h_gain_allchannels.at(i)->GetBinContent(i));

        gainValue.push_back(gain_in);

        TCanvas* c = new TCanvas();
        c->cd();
        vec_h_gain_allchannels[i]->Draw();
        c->SaveAs(Form("~/sw/sr90readout/results/gainRepeatability/gain_allchannels_file_%d.pdf",i), "Overwrite");
        c->Close();
        delete c;

        h_singleCh_gainDist->Fill(vec_h_gain_allchannels.at(i)->GetBinContent(cherryPickChNr));
    }
    calculate_channelGainRMS();

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

    TCanvas* c3 = new TCanvas("c_h_singleCh_gainDist", "c_h_singleCh_gainDist");
    c3->cd();
    gStyle->SetOptStat("mreuon");
    h_singleCh_gainDist->Draw();
    c3->SaveAs("~/sw/sr90readout/results/gainRepeatability/h_singleCh_gainDist.root", "Overwrite");
    c3->SaveAs("~/sw/sr90readout/results/gainRepeatability/h_singleCh_gainDist.pdf", "Overwrite");
    c3->Close();
    delete c3;
}

void GainRepeatability::calculate_channelGainRMS()
{
    std::vector<double> channelRMS;
    int nMeasurements = gainValue.size();
    int nChannels = gainValue.at(0).size();
    for(int channel= 0; channel < nChannels; ++channel)
    {
        double sum = 0;
        for (int measNr = 0; measNr < nMeasurements; ++measNr)
            sum += gainValue.at(measNr).at(channel);
        double mean = sum /nChannels;

        sum = 0;
        for (int measNr = 0; measNr < nMeasurements; ++measNr)
            sum += std::pow(gainValue.at(measNr).at(channel) - mean, 2);
        double RMS = std::sqrt(1./double(nMeasurements-1)*sum);
        channelRMS.push_back(RMS);
    }

    double sum = 0;
    for (unsigned int i = 0; i< channelRMS.size(); ++i)
        sum += channelRMS.at(i);
    double averageRMS = sum/channelRMS.size();
    std::cout << "\n\n" << averageRMS << std::endl;
}
