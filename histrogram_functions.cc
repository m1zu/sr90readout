#include "histogram_functions.hh"

#include <TH1D.h>
#include <assert.h>

double HISTOGRAM_FUNCTION::meanValue(TH1D* h)
{
    double mean=0;
    int n = h->GetNbinsX();
    double* channel = &(h->GetArray()[1]);

    // full range (except border channels)
    for (int i=0; i<n; ++i)
        if ((((i+1)%64)==0 || (i%64)==0))
            ; else mean+=channel[i];
    mean /= double(n-16);

  // short range
  /*
  int count = 0;

  for (int i=0; i<n; ++i)
  {
      if ((((i+1)%64)==0 || (i%64)==0))
          ; else
      {
          if ((i>128) && (i<384)) {
              mean+=channel[i];
              count++;
          }
      }
  }
  mean /= double(count-1);
  */

    return mean;
}

void HISTOGRAM_FUNCTION::cropped_fillAnotherHist(TH1D* h_fill, const TH1D* h_ref)
{
    int n = h_ref->GetNbinsX();

    // full range (except border channels)
    for (int i=0; i<n; ++i)
        h_fill->Fill(h_ref->GetBinContent(i));
}

void HISTOGRAM_FUNCTION::cropHistrogram(TH1D* h_fill, const TH1D* h_ref)
{
    unsigned int counter = 1;
    for (int i=129; i<=384; ++i)
        if ((((i-1)%64)==0 || (i%64)==0))
            ;
        else {
            h_fill->SetBinContent(counter, h_ref->GetBinContent(i));
            counter++;
        }
}

void HISTOGRAM_FUNCTION::cropped_normalizeOnAnotherHist(TH1D* h_norm, const TH1D* h_ref)
{
    int n = h_ref->GetNbinsX();
    int m = h_norm->GetNbinsX();
    assert(n == m);

    for (int i=1; i<=n; ++i)
    {
        double value = h_norm->GetBinContent(i);
        double ref_value = h_ref->GetBinContent(i);
        double new_value = value/ref_value;
        h_norm->SetBinContent(i, new_value);
    }
}

double HISTOGRAM_FUNCTION::cropped_getMeanValue(const TH1D* h)
{
     int n = h->GetNbinsX();
     double sum = 0.;
     double divider = 0.;
     for (int i=1; i<=n; ++i)
     {
         double value = h->GetBinContent(i);
         double error = h->GetBinError(i);
         sum += value/(error*error);
         divider += 1./(error*error);
     }
     return sum/divider;
}

double HISTOGRAM_FUNCTION::cropped_getMeanValueError(const TH1D* h)
{
    int n = h->GetNbinsX();
    double divider = 0.;
    for (int i=1; i<=n; ++i)
    {
        double error = h->GetBinError(i)/sqrt(500000./512.); // average events in channel
        divider += 1./(error*error);
    }
    return sqrt(1./divider);
}
