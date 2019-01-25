#ifndef CHECK_GAIN_HH
#define CHECK_GAIN_HH

#include <vector>

#include <TH1D.h>
#include <TGraph.h>

class Check_Gain
{
public:
    Check_Gain(const int nFiles);
    ~Check_Gain();
public:
    void AddData(TH1D* h_gain_allchannels);
    void Evaluate();
private:
    std::vector< TH1D* > vec_h_gain_allchannels;
    TGraph* g_meanGainValues;
    TH1D* h_meanGainValues;
    std::string analysis_result_DIR = "/home/iwanicki/sw/sr90readout/results/gainCheck";
};

#endif // CHECK_GAIN_HH
