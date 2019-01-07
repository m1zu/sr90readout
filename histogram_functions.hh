#pragma once

double meanValue(TH1D* h)
{
  double mean=0;
  int n = h->GetNbinsX();
  double* channel = &(h->GetArray()[1]);

  // full range (except border channels)
  for (int i=0; i<n; ++i)
    if ((((i+1)%64)==0 || (i%64)==0))
      ; else mean+=channelLightYield[i];
  mean /= double(n-16);

  /*
  // short range
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
