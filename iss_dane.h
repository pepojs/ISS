#ifndef ISS_DANE_H
#define ISS_DANE_H

#include <QWidget>

struct ISS_Dane{
    double SzerokoscGeo;
    double DlugoscGeo;
    double Predkosc;
    double Wysokosc;
    uint32_t CzasPrzelotu;

    ISS_Dane():SzerokoscGeo(0), DlugoscGeo(0),Predkosc(0), Wysokosc(0), CzasPrzelotu(0)
    {;}
};

#endif // ISS_DANE_H
