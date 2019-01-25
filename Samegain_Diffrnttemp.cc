#include "Samegain_Diffrnttemp.hh"

#include "histogram_functions.hh"

#include <iostream>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TAxis.h>
#include <TGaxis.h>

SameGain_DiffrntTemp::SameGain_DiffrntTemp(int nFiles)
{

    if(chdir(analysis_result_DIR.c_str()) == -1) {
      std::cerr << std::endl << "Error in chdir: " << strerror(errno) << std::endl;
      std::cerr << "Setup variable (and path) analysis_result_DIR!" << std::endl;
      return;
    }

    g_temp_meanGainValues = new TGraph(nFiles);
    g_temp_setVoltage = new TGraph(nFiles);
    g_temp_meanLY = new TGraph(nFiles);
}

SameGain_DiffrntTemp::~SameGain_DiffrntTemp()
{

}

void SameGain_DiffrntTemp::AddData(double temperature, double voltage, TH1D* gain_values, TH1D* lightYield)
{
    TH1D* cropped_gain_values = new TH1D(Form("cropped_gain_values_t%f", temperature), "", 248, 0, 247);
    HISTOGRAM_FUNCTION::cropHistrogram(cropped_gain_values, gain_values);

    TH1D* cropped_ly_values = new TH1D(Form("cropped_ly_values_t%f", temperature), "", 248, 0, 247);
    HISTOGRAM_FUNCTION::cropHistrogram(cropped_ly_values, lightYield);

    temperature_gain_histogram_pair.push_back(std::make_pair(temperature, cropped_gain_values));
    temperature_setVoltage_pair.push_back(std::make_pair(temperature, voltage));
    temperature_meanLY_histogram_pair.push_back((std::make_pair(temperature, cropped_ly_values)));
}

void SameGain_DiffrntTemp::Evaluate()
{
    ///////////////////////
    for (unsigned int i=0; i<temperature_gain_histogram_pair.size(); ++i)
    {
        double temperature = temperature_gain_histogram_pair.at(i).first;
        double meanGain = HISTOGRAM_FUNCTION::meanValue(temperature_gain_histogram_pair.at(i).second);
        g_temp_meanGainValues->SetPoint(i, temperature, meanGain);
    }
    TCanvas* c1 = new TCanvas("c1", "c1");
    gStyle->SetOptStat("nemrou");
    c1->cd();
    g_temp_meanGainValues->SetTitle();
    g_temp_meanGainValues->GetXaxis()->SetTitle("temperature [#circC]");
    g_temp_meanGainValues->GetXaxis()->SetTitleOffset(1.0);
    g_temp_meanGainValues->GetYaxis()->SetTitle("mean Gain [ADC count]");
    g_temp_meanGainValues->GetYaxis()->SetTitleOffset(1.2);
    g_temp_meanGainValues->Draw();
    std::string outfilePath = analysis_result_DIR + std::string("/meanGain_against_temperature.pdf");
    c1->SaveAs(outfilePath.c_str(), "Overwrite");
    c1->Close();
    delete c1;

    //////////////////////////
    for (unsigned int i=0; i<temperature_setVoltage_pair.size(); ++i)
    {
        g_temp_setVoltage->SetPoint(i, temperature_setVoltage_pair.at(i).first, temperature_setVoltage_pair.at(i).second);
    }
    TCanvas* c2 = new TCanvas("c2", "c2");
    gStyle->SetOptStat("nemrou");
    c2->cd();
    g_temp_setVoltage->SetTitle();
    g_temp_setVoltage->GetXaxis()->SetTitle("temperature [#circC]");
    g_temp_setVoltage->GetXaxis()->SetTitleOffset(1.0);
    g_temp_setVoltage->GetYaxis()->SetTitle("set Voltage [V]");
    g_temp_setVoltage->GetYaxis()->SetTitleOffset(1.2);
    g_temp_setVoltage->Draw();
    outfilePath = analysis_result_DIR + std::string("/voltage_against_temperature.pdf");
    c2->SaveAs(outfilePath.c_str(), "Overwrite");
    c2->Close();
    delete c2;

    /////////////////////////
    for (unsigned int i=0; i<temperature_gain_histogram_pair.size(); ++i)
    {
        double temperature = temperature_gain_histogram_pair.at(i).first;
        TH1D* h_ref = temperature_gain_histogram_pair.at(i).second;
        h_temp_gain_distribution_pair.push_back(std::make_pair(temperature,
                                                                new TH1D(Form("h_temp_gain_distribution_it%d",i), "", 50, 40, 55)));
        HISTOGRAM_FUNCTION::cropped_fillAnotherHist(h_temp_gain_distribution_pair.at(i).second, h_ref);
    }
    TCanvas* c3 = new TCanvas("c3", "c3");
    TLegend* t3 = new TLegend( .15, .6, .3, .9);
    gStyle->SetOptStat("");
    c3->cd();
    for (unsigned int i=0; i<h_temp_gain_distribution_pair.size(); ++i)
    {
        double colorCode = (420. - h_temp_gain_distribution_pair.at(i).first)/10.;
        int color = std::round(colorCode);
        h_temp_gain_distribution_pair.at(i).second->GetXaxis()->SetTitle("Gain [ADC count]");
        h_temp_gain_distribution_pair.at(i).second->GetXaxis()->SetTitleOffset(1.);
        h_temp_gain_distribution_pair.at(i).second->GetXaxis()->SetLabelSize(40);
        h_temp_gain_distribution_pair.at(i).second->GetXaxis()->SetTitleSize(40);
        h_temp_gain_distribution_pair.at(i).second->GetYaxis()->SetLabelSize(40);
        h_temp_gain_distribution_pair.at(i).second->GetYaxis()->SetTitle("#");
        h_temp_gain_distribution_pair.at(i).second->GetYaxis()->SetTitleSize(50);
        h_temp_gain_distribution_pair.at(i).second->GetYaxis()->SetTitleOffset(.5);
        h_temp_gain_distribution_pair.at(i).second->SetFillColor(color);
        h_temp_gain_distribution_pair.at(i).second->Draw("same");
        t3->AddEntry(h_temp_gain_distribution_pair.at(i).second, Form("%2.0f #circC", h_temp_gain_distribution_pair.at(i).first), "f");
    }
    t3->Draw();
    outfilePath = analysis_result_DIR + std::string("/gain_temperature_distributions.pdf");
    c3->SaveAs(outfilePath.c_str(), "Overwrite");
    c3->Close();
    delete c3;

    ///////////////////////
    draw_GainAndVoltage_against_temp();

    ///////////////////////
    TCanvas* c4 = new TCanvas("c4", "c4");
    TLegend* t4 = new TLegend( .77, .6, .9, .9);
    c4->cd();
    for (unsigned int i=0; i<temperature_meanLY_histogram_pair.size(); ++i)
    {
        double colorCode = (30. - temperature_meanLY_histogram_pair.at(i).first)/10.;
        int color = std::round(colorCode);
        temperature_meanLY_histogram_pair.at(i).second->SetLineColor(color);
        gStyle->SetOptStat("");
        temperature_meanLY_histogram_pair.at(i).second->GetXaxis()->SetTitle("SiPM channel number");
        temperature_meanLY_histogram_pair.at(i).second->GetXaxis()->SetTitleOffset(1.);
        temperature_meanLY_histogram_pair.at(i).second->GetXaxis()->SetLabelSize(40);
        temperature_meanLY_histogram_pair.at(i).second->GetXaxis()->SetTitleSize(40);
        temperature_meanLY_histogram_pair.at(i).second->GetYaxis()->SetLabelSize(40);
        temperature_meanLY_histogram_pair.at(i).second->GetYaxis()->SetTitle("mean channel light yield [pixel]");
        temperature_meanLY_histogram_pair.at(i).second->GetYaxis()->SetTitleSize(50);
        temperature_meanLY_histogram_pair.at(i).second->GetYaxis()->SetTitleOffset(.9);
        temperature_meanLY_histogram_pair.at(i).second->Draw("same");
        t4->AddEntry(temperature_meanLY_histogram_pair.at(i).second, Form("%2.0f #circC", temperature_meanLY_histogram_pair.at(i).first), "l");
    }
    t4->Draw();
    outfilePath = analysis_result_DIR + std::string("/LY_temperature_distributions.pdf");
    c4->SaveAs(outfilePath.c_str(), "Overwrite");
    c4->Close();
    delete c4;

    //////////////////////////
    for (unsigned int i=0; i<temperature_meanLY_histogram_pair.size(); ++i)
    {
        int n = temperature_meanLY_histogram_pair.size();
        HISTOGRAM_FUNCTION::cropped_normalizeOnAnotherHist( temperature_meanLY_histogram_pair.at(i).second,
                                                            temperature_meanLY_histogram_pair.at(n-1).second);
    }
    TCanvas* c5 = new TCanvas("c5", "c5");
    TLegend* t5 = new TLegend( .77, .6, .9, .9);
    c5->cd();
    for (unsigned int i=0; i<temperature_meanLY_histogram_pair.size(); ++i)
    {
        double colorCode = (30. - temperature_meanLY_histogram_pair.at(i).first)/10.;
        int color = std::round(colorCode);
        temperature_meanLY_histogram_pair.at(i).second->SetLineColor(color);
        temperature_meanLY_histogram_pair.at(i).second->GetYaxis()->SetRangeUser(0.96, 1.18);
        gStyle->SetOptStat("");
        temperature_meanLY_histogram_pair.at(i).second->GetXaxis()->SetTitle("SiPM channel number");
        temperature_meanLY_histogram_pair.at(i).second->GetXaxis()->SetTitleOffset(1.);
        temperature_meanLY_histogram_pair.at(i).second->GetXaxis()->SetLabelSize(40);
        temperature_meanLY_histogram_pair.at(i).second->GetXaxis()->SetTitleSize(40);
        temperature_meanLY_histogram_pair.at(i).second->GetYaxis()->SetLabelSize(40);
        temperature_meanLY_histogram_pair.at(i).second->GetYaxis()->SetTitle("LY(ch, T) / LY(ch, 20#circC)");
        temperature_meanLY_histogram_pair.at(i).second->GetYaxis()->SetTitleSize(50);
        temperature_meanLY_histogram_pair.at(i).second->GetYaxis()->SetTitleOffset(.9);
        temperature_meanLY_histogram_pair.at(i).second->Draw("same");
        t5->AddEntry(temperature_meanLY_histogram_pair.at(i).second, Form("%2.0f #circC", temperature_meanLY_histogram_pair.at(i).first), "l");
    }
    t5->Draw();
    outfilePath = analysis_result_DIR + std::string("/realtiveLY_temperature_distributions.pdf");
    c5->SaveAs(outfilePath.c_str(), "Overwrite");
    c5->Close();
    delete c5;

    /////////////////////////
    for (unsigned int i=0; i<temperature_meanLY_histogram_pair.size(); ++i)
    {
        double temperature = temperature_meanLY_histogram_pair.at(i).first;
        double meanLY = HISTOGRAM_FUNCTION::cropped_getMeanValue(temperature_meanLY_histogram_pair.at(i).second);
        g_temp_meanLY->SetPoint(i, temperature, meanLY);

    }
    TCanvas* c6 = new TCanvas("c6", "c6");
    c6->cd();
    g_temp_meanLY->SetTitle("");
    g_temp_meanLY->Draw("ap");
    outfilePath = analysis_result_DIR + std::string("/meanLY_temperature.pdf");
    c6->SaveAs(outfilePath.c_str(), "Overwrite");
    c6->Close();
    delete c6;
}

void SameGain_DiffrntTemp::draw_GainAndVoltage_against_temp()
{
   TCanvas *c7 = new TCanvas("c7","transparent pad",200,10,700,500);
   TPad *pad1 = new TPad("pad1","",0,0,1,1);
   TPad *pad2 = new TPad("pad2","",0,0,1,1);
   pad2->SetFillStyle(4000); //will be transparent
   pad1->Draw();
   pad1->cd();

   g_temp_setVoltage->GetYaxis()->SetLabelSize(18);
   g_temp_setVoltage->GetYaxis()->SetTitle("SiPM voltage [V]");
   g_temp_setVoltage->GetYaxis()->SetTitleSize(20);
   g_temp_setVoltage->GetYaxis()->SetTitleOffset(1.1);
   g_temp_setVoltage->GetXaxis()->SetLabelSize(20);
   g_temp_setVoltage->GetXaxis()->SetTitleSize(20);
   g_temp_setVoltage->GetXaxis()->SetTitleOffset(1.1);
   g_temp_setVoltage->Draw();
   pad1->Modified();
   c7->cd();

   //compute the pad range with suitable margins
   Double_t ymin = 49.5;
   Double_t ymax = 51.0;
   Double_t dy = (ymax-ymin)/0.8; //10 per cent margins top and bottom
   Double_t xmin = g_temp_setVoltage->GetXaxis()->GetXmin();
   Double_t xmax = g_temp_setVoltage->GetXaxis()->GetXmax();
   Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right
   pad2->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
   pad2->Draw();
   pad2->cd();
   g_temp_meanGainValues->SetMarkerColor(kBlue);
   g_temp_meanGainValues->SetLineColor(kBlue);
   g_temp_meanGainValues->SetMarkerStyle(kFullSquare);
   g_temp_meanGainValues->Draw("X+Y+ pl");

   // draw axis on the right side of the pad
   TGaxis *axis = new TGaxis(xmax,ymin,xmax,ymax,ymin,ymax,50510,"+L");
   axis->SetLabelColor(kBlue);
   axis->SetTitle("Gain [ADC count]");
   axis->SetTitleOffset(1.2);
   axis->SetTitleColor(kBlue);
   axis->Draw();

   std::string outfilePath = analysis_result_DIR + std::string("/gain_and_setVoltage_temp.pdf");
   c7->SaveAs(outfilePath.c_str(), "Overwrite");
   c7->Close();
   delete c7;
}
