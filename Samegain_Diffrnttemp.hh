#ifndef SAMEGAIN_DIFFRNTTEMP_HH
#define SAMEGAIN_DIFFRNTTEMP_HH

#include <vector>
#include <string>

#include <TH1D.h>
#include <TGraph.h>
#include <TGraphErrors.h>


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
    void draw_meanGain_against_temperature();
    void draw_voltage_against_temperature();
    void draw_gain_temperature_distributions();
    void draw_ly_temperature_distributions();
    void calculate_ly_against_temp_Results();
private:
    void correct_ly_by_gain();
private:
    std::string analysis_result_DIR = "/home/iwanicki/sw/sr90readout/results/sameGain_diffTemp";

    std::vector< std::pair<double, TH1D*> > temperature_gain_histogram_pair;
    TGraph* g_temp_meanGainValues;

    std::vector< std::pair<double, double> > temperature_setVoltage_pair;
    TGraph* g_temp_setVoltage;

    std::vector< std::pair<double, TH1D*> > h_temp_gain_distribution_pair;

    std::vector< std::pair<double, TH1D*> > temperature_meanLY_histogram_pair;

    TGraphErrors* g_temp_meanLY;
};

#endif // SAMEGAIN_DIFFRNTTEMP_HH
