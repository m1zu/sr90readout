#include "Helpers.hh"
#include "GainRepeatability.hh"
#include "Check_Gain.hh"
#include "Samegain_Diffrnttemp.hh"

#include <TApplication.h>
#include <TVirtualFitter.h>

#include <vector>
#include <utility>
#include <cstring>

int main(int argc, char** argv) {
    //TApplication application("sr90readout", &argc, argv);
    //TH1::AddDirectory(false);
    Helpers::setRootStyle();

    QString filepath = ("/home/iwanicki/data/freeze/SR90_again_same_gain/SORTED/");
    QStringList files = QDir(filepath).entryList(QStringList() << "*messung*.root");
    const int nFiles = files.count();
    qDebug() << endl << "using" << nFiles << " files ... ";

    // Analysis Classes Initialization

    //GainRepeatability gainRepeatability(nFiles);
    //Check_Gain checkGain(nFiles);
    SameGain_DiffrntTemp sameGain(nFiles);

    // Data Processing

    foreach (QString file, files)
    {
        qDebug() << "\n  ..processing "<< file;
        QString fullPath = filepath + file;

        std::string temperature_sign = file.mid(8, 1).toStdString(); // extract voltage form filename aswell!
        double temperature = file.mid(9, 2).toDouble();
        if (temperature_sign.compare("m") == 0)
            temperature*=(-1);
        double voltage = file.mid(0, 2).toDouble() + file.mid(3, 3).toDouble()*0.001;
        TH1D* h_gain_allChannels = Helpers::extractFromFile<TH1D>(fullPath, "parameter value 0 HistogramDisplay").front();
        TH1D* h_meanLY_allChannels = Helpers::extractFromFile<TH1D>(fullPath, "cluster signal value").front();
        //gainRepeatability.AddData(h_gain_allChannels);
        //checkGain.AddData(h_gain_allChannels);
        sameGain.AddData(temperature, voltage, h_gain_allChannels, h_meanLY_allChannels);
    }

    // Evaluation

    //gainRepeatability.Evaluate();
    //checkGain.Evaluate();
    sameGain.Evaluate();

    //application.Run();
    return 0;
}
