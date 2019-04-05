#include "magazyn_danych.h"

Magazyn_danych::Magazyn_danych()
{
    MagazynDanych.resize(5400);
}

Magazyn_danych::Magazyn_danych(size_t IloscDanych)
{
    MagazynDanych.resize(IloscDanych);
}


void Magazyn_danych::WypelniDanymiZSieci(Http *Html)
{
    ISS_Dane NoweDane = Html->PobierzDaneOISS();
    uint32_t CzasDanych;

    if(NoweDane.CzasPrzelotu > (24*60*60))
        CzasDanych = NoweDane.CzasPrzelotu - (24*60*60);
    else
        CzasDanych = QDateTime::currentSecsSinceEpoch();

    ISS_Dane TabPom[10];
    size_t i;

    cout<<"Pobieranie danych do magazynu z sieci"<<endl;
    for(i = 0; i < (200/10); i++)
    {
        Html->PobierzDaneOISS(&TabPom[0], 10, CzasDanych, 16);
        CzasDanych += 10*16;
        for(size_t j = 0; j < 10; j++)
        {
            MagazynDanych[(i*10)+j] = TabPom[j];
            cout<<"Mag: "<<MagazynDanych[(i*10)+j].CzasPrzelotu<<endl;
        }

        cout<<"Pobrano "<<10*i<<" danych"<<endl;
    }

    Html->PobierzDaneOISS(&TabPom[0], MagazynDanych.size()%10, CzasDanych, 16);
    for(size_t j = 0; j < MagazynDanych.size()%10; j++)
            MagazynDanych[(i*10)+j] = TabPom[j];

    cout<<"Pobrano dane!"<<endl;
}


void Magazyn_danych::ZapiszDane()
{
    QString NazwaPliku = QCoreApplication::applicationDirPath() + "/" + "DaneStacji.txt";
    std::fstream plik(NazwaPliku.toStdString().c_str(), ios::in | ios::out);
    size_t i = 0;
    //Sprawdzanie zawartosci pliku i dopisywanie nowych danych w odpowiednim miejscu

    if(plik.good() == true)
    {
        if(plik.eof())
        {
            cout<<"Gdzie jest plik"<<endl;
            for(i = 0; i < MagazynDanych.size(); i++)
            {
                if(MagazynDanych[i].CzasPrzelotu == 0)break;
            }

            cout<<"i w funkcji zapisu"<<endl;
            plik<<MagazynDanych[0].CzasPrzelotu<<" "<<MagazynDanych[i-1].CzasPrzelotu<<" "<<60<<endl;

            for (i = 0; i < MagazynDanych.size(); i++)
            {
                plik<<MagazynDanych[i].CzasPrzelotu<<" "<<MagazynDanych[i].Predkosc<<" "<<MagazynDanych[i].Wysokosc<<" "<<MagazynDanych[i].DlugoscGeo<<" "<<MagazynDanych[i].SzerokoscGeo<<endl;
            }
        }

        plik.close();
    }

}
