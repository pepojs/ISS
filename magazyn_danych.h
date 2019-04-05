#ifndef MAGAZYN_DANYCH_H
#define MAGAZYN_DANYCH_H

#include <vector>
#include <QDateTime>
#include <fstream>
#include <QCoreApplication>

#include "iss_dane.h"
#include "http.h"

using namespace std;

class Magazyn_danych
{
    vector<ISS_Dane> MagazynDanych;

public:
    Magazyn_danych();
    Magazyn_danych(size_t IloscDanych);
    void WypelniDanymiZSieci(Http* Html);
    void ZapiszDane();
    void WypelniDanymiZPliku();
    size_t ZwrocIloscDanych(){MagazynDanych.size();}
    ISS_Dane ZwrocDane(size_t Ktore){return MagazynDanych[Ktore];}


};

#endif // MAGAZYN_DANYCH_H
