#include "Helpers.hh"
#include "histogram_functions.hh"

#include <TApplication.h>
#include <TVirtualFitter.h>

#include <vector>
#include <utility>

int main(int argc, char** argv) {
    TApplication application("sr90readout", &argc, argv);
    //TH1::AddDirectory(false);
    Helpers::setRootStyle();

    QString filepath = ("/home/iwanicki/data/freeze/repeatability_20C/Referenzmessung/");
    QStringList files = QDir(filepath).entryList(QStringList() << "*Summary*.root");
    const int nFiles = files.count();
    qDebug() << endl << "using" << nFiles << " files ... ";

    Double_t* mean = new Double_t[nFiles];
    std::vector< std::pair<double, TH1D*> > voltage_gain_histogram_pair;

    foreach (QString file, files)
    {
      qDebug() << "\n  ..processing "<< file;

      QString fullPath = filepath + file;
      double temperature = file.mid(1, 2).toDouble();
      qDebug() << file.mid(1,2);
      voltage_gain_histogram_pair.push_back(std::make_pair(temperature, Helpers::extractFromFile<TH1D>(fullPath, "parameter value 0 HistogramDisplay").front()));
    }

    TGraph* g_meanGainValues = new TGraph(voltage_gain_histogram_pair.size());
    for (int i=0; i<voltage_gain_histogram_pair.size(); ++i)
    {
        double voltage = voltage_gain_histogram_pair.at(i).first;
        double meanGain = meanValue(voltage_gain_histogram_pair.at(i).second);
        g_meanGainValues->SetPoint(i, voltage, meanGain);
    }

  /* draw and fit */

    TCanvas* c1 = new TCanvas("c1", "c1");
    gStyle->SetOptStat("nemrou");
    c1->cd(0);
    g_meanGainValues->Draw();

    application.Run();
    return 0;
}
