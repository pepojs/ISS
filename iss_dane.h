#ifndef ISS_DANE_H
#define ISS_DANE_H

#include <QWidget>

//Przerobic to na klase wraz z całym interfejsem opisac 4 klasy, na pewno http oraz dane_stacji, 2 dodatkowe wybrac (ciekawsze)
//Dane stacji dodac jakie jednoski na przyklad pobierz_predkosc_kmH, dokumentacja html-u doxygen, metody ktore nie zmieniaja
//obiekt dodac const.
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
