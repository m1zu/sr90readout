#ifndef SAMEGAIN_DIFFRNTTEMP_HH
#define SAMEGAIN_DIFFRNTTEMP_HH

#include <vector>
#include <string>

#include <TH1D.h>
#include <TGraph.h>

class SameGain_DiffrntTemp
{
public:
    SameGain_DiffrntTemp(int nFiles);
    ~SameGain_DiffrntTemp();
public:
    void AddData(double temperature, double voltage, TH1D* gain_values, TH1D* lightYield);
    void Evaluate();
private:
    void draw_GainAndVoltage_against_temp();
private:
    std::string analysis_result_DIR = "/home/iwanicki/sw/sr90readout/results/sameGain_diffTemp";

    std::vector< std::pair<double, TH1D*> > temperature_gain_histogram_pair;
    TGraph* g_temp_meanGainValues;

    std::vector< std::pair<double, double> > temperature_setVoltage_pair;
    TGraph* g_temp_setVoltage;

    std::vector< std::pair<double, TH1D*> > h_temp_gain_distribution_pair;

    std::vector< std::pair<double, TH1D*> > temperature_meanLY_histogram_pair;

    TGraph* g_temp_meanLY;
};

#endif // SAMEGAIN_DIFFRNTTEMP_HH
