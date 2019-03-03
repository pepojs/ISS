#ifndef ISS_DANE_H
#define ISS_DANE_H

struct ISS_Dane{
    double SzerokoscGeo;
    double DlugoscGeo;
    double Predkosc;
    double Wysokosc;

    ISS_Dane():SzerokoscGeo(0), DlugoscGeo(0),Predkosc(0), Wysokosc(0)
    {;}
};

#endif // ISS_DANE_H
