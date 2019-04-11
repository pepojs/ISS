#ifndef MAGAZYN_DANYCH_H
#define MAGAZYN_DANYCH_H

#include <vector>
#include <QDateTime>
#include <fstream>
#include <QCoreApplication>
#include <math.h>

#include "iss_dane.h"
#include "http.h"

using namespace std;

class Magazyn_danych
{
    vector<ISS_Dane> MagazynDanych;
    uint32_t Glowa;

public:
    Magazyn_danych();
    Magazyn_danych(size_t IloscDanych);
    ~Magazyn_danych();

    void WypelniDanymiZSieci(Http* Html, uint32_t Czestotliwosc, uint32_t IloscDanychWypelnionych, uint32_t CzasDoTylu_S);
    void ZapiszDane(uint32_t Czestotliwosc);
    long int WypelniDanymiZPliku(Http* Html, uint32_t CzasDoTylu_S);
    size_t ZwrocIloscDanych(){return MagazynDanych.size();}
    ISS_Dane ZwrocDane(size_t Ktore){return MagazynDanych[Ktore];}
    uint32_t ZwrocGlowe(){return Glowa;}
    void DodajNoweDane(ISS_Dane NoweDane);


};

#endif // MAGAZYN_DANYCH_H
