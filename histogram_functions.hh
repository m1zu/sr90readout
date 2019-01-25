#ifndef HISTOGRAM_FUNCTIONS
#define HISTOGRAM_FUNCTIONS

#include <TH1D.h>

namespace HISTOGRAM_FUNCTION
{
double meanValue(TH1D* h);
void cropHistrogram(TH1D* h_fill, const TH1D* h_ref);
void cropped_fillAnotherHist(TH1D* h_fill, const TH1D* h_ref);
void cropped_normalizeOnAnotherHist(TH1D* h_norm, const TH1D* h_ref);
double cropped_getMeanValue(const TH1D* h);
}

#endif // HISTOGRAM_FUNCTIONS
