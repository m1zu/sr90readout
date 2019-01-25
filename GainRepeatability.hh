#pragma once

#include <vector>

#include <TH1D.h>
#include <TGraph.h>

class GainRepeatability
{
public:
    GainRepeatability(const int nFiles);
    ~GainRepeatability();
public:
    void AddData(TH1D* h_gain_allchannels);
    void Evaluate();
private:
    std::vector< TH1D* > vec_h_gain_allchannels;
    TGraph* g_meanGainValues;
    TH1D* h_meanGainValues;
};
