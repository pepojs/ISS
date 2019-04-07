#include "magazyn_danych.h"

Magazyn_danych::Magazyn_danych()
{
    MagazynDanych.resize(5400);
    Glowa = 0;
}

Magazyn_danych::Magazyn_danych(size_t IloscDanych)
{
    MagazynDanych.resize(IloscDanych);
    Glowa = 0;
}


void Magazyn_danych::WypelniDanymiZSieci(Http *Html, uint32_t Czestotliwosc, uint32_t IloscDanychWypelnionych, uint32_t CzasDoTylu_S)
{
    ISS_Dane NoweDane = Html->PobierzDaneOISS();
    uint32_t CzasDanych;

    if(IloscDanychWypelnionych == 0)
    {
        if(NoweDane.CzasPrzelotu == 0)NoweDane = Html->PobierzDaneOISS();

        if(NoweDane.CzasPrzelotu > CzasDoTylu_S)
            CzasDanych = NoweDane.CzasPrzelotu - CzasDoTylu_S;
        else
            CzasDanych = QDateTime::currentSecsSinceEpoch() - CzasDoTylu_S;

    }else
    {
        CzasDanych = MagazynDanych[IloscDanychWypelnionych].CzasPrzelotu+Czestotliwosc;
        IloscDanychWypelnionych += 1;
    }

    ISS_Dane TabPom[10];
    size_t licznik = 0;
    size_t roznica = 0;

    cout<<"Czas danych "<<CzasDanych<<endl;
    cout<<"Ilosc "<<IloscDanychWypelnionych<<endl;
    cout<<"Pobieranie danych do magazynu z sieci"<<endl;

    do
    {
        NoweDane = Html->PobierzDaneOISS();
        licznik += 1;

    }while(NoweDane.CzasPrzelotu == 0 && licznik < 100);
cout<<"Licznik "<<licznik<<endl;
    licznik = 0;

    for(licznik = IloscDanychWypelnionych; licznik < (MagazynDanych.size()-10);)
    {
        Html->PobierzDaneOISS(&TabPom[0], 10, CzasDanych, Czestotliwosc);
        CzasDanych += 10*Czestotliwosc;
        for(size_t j = 0; j < 10; j++)
        {
            MagazynDanych[licznik] = TabPom[j];
            cout<<"Mag: "<<MagazynDanych[licznik].CzasPrzelotu<<endl;
            licznik += 1;
        }

        cout<<"Pobrano "<<licznik<<" danych"<<endl;
    }

    roznica = MagazynDanych.size()-licznik;
    Html->PobierzDaneOISS(&TabPom[0], roznica, CzasDanych, Czestotliwosc);
    for(size_t j = 0; j < roznica; j++)
    {
            MagazynDanych[licznik] = TabPom[j];
            //cout<<"Mag: "<<MagazynDanych[licznik].CzasPrzelotu<<endl;
            licznik += 1;
    }

    cout<<"Pobrano "<<licznik<<" danych"<<endl;
    //cout<<"Pobrano dane!"<<endl;
}


void Magazyn_danych::ZapiszDane(uint32_t Czestotliwosc)
{
    QString NazwaPliku = QCoreApplication::applicationDirPath() + "/" + "DaneStacji.txt";
    std::fstream plik(NazwaPliku.toStdString().c_str(), ios::out);
    size_t i = 0;
    size_t j = 0;
    size_t GlowaKopia1 = 0;
    size_t GlowaKopia2 = 0;
    //Sprawdzanie zawartosci pliku i dopisywanie nowych danych w odpowiednim miejscu

    if(plik.good() == true)
    {
        GlowaKopia1 = Glowa;
        for(j = 0; j < MagazynDanych.size(); j++)
        {
            if(MagazynDanych[GlowaKopia1].CzasPrzelotu != 0)break;
            GlowaKopia1 += 1;
            GlowaKopia1 = GlowaKopia1%MagazynDanych.size();
        }

        GlowaKopia2 = Glowa;
        for(i = 0; i < MagazynDanych.size(); i++)
        {
            if(MagazynDanych[GlowaKopia2].CzasPrzelotu == 0)break;
            GlowaKopia2 += 1;
            GlowaKopia2 = GlowaKopia2%MagazynDanych.size();
        }

        if(GlowaKopia2 == 0)
            GlowaKopia2 = MagazynDanych.size();

        plik<<MagazynDanych[GlowaKopia1].CzasPrzelotu<<" "<<MagazynDanych[GlowaKopia2-1].CzasPrzelotu<<" "<<Czestotliwosc<<endl;

        GlowaKopia1 = Glowa;
        for (i = 0; i < MagazynDanych.size(); i++)
        {
            plik<<MagazynDanych[GlowaKopia1].CzasPrzelotu<<" "<<MagazynDanych[GlowaKopia1].Predkosc<<" "<<MagazynDanych[GlowaKopia1].Wysokosc<<" "<<MagazynDanych[GlowaKopia1].DlugoscGeo<<" "<<MagazynDanych[GlowaKopia1].SzerokoscGeo<<endl;
            GlowaKopia1 += 1;
            GlowaKopia1 = GlowaKopia1%MagazynDanych.size();
        }

        plik.flush();
        plik.close();
    }

}

long int Magazyn_danych::WypelniDanymiZPliku(Http* Html, uint32_t CzasDoTylu_S)
{
    QString NazwaPliku = QCoreApplication::applicationDirPath() + "/" + "DaneStacji.txt";
    std::fstream plik(NazwaPliku.toStdString().c_str(), ios::in);

    uint32_t CzasPoczatku = 0;
    uint32_t CzasKonca = 0;
    uint32_t Czestotliwosc = 0;
    uint32_t Wierzsz = 0;

    ISS_Dane NoweDane = Html->PobierzDaneOISS();
    uint32_t CzasDanych;

    size_t k = 0;

    if(NoweDane.CzasPrzelotu == 0)NoweDane = Html->PobierzDaneOISS();

    if(NoweDane.CzasPrzelotu > CzasDoTylu_S)
        CzasDanych = NoweDane.CzasPrzelotu - CzasDoTylu_S;
    else
        CzasDanych = QDateTime::currentSecsSinceEpoch() - CzasDoTylu_S;

    if(plik.good() == true)
    {
        plik>>CzasPoczatku>>CzasKonca>>Czestotliwosc;

        if(CzasDanych > CzasKonca) return 1;
        else
        {
           Wierzsz = (uint32_t)(ceil((float)(CzasDanych - CzasPoczatku)/(float)Czestotliwosc)+1);

           k = 0;
           while(!plik.eof())
           {
               plik>>MagazynDanych[k].CzasPrzelotu>>MagazynDanych[k].Predkosc>>MagazynDanych[k].Wysokosc>>MagazynDanych[k].DlugoscGeo>>MagazynDanych[k].SzerokoscGeo;

               if(MagazynDanych[k].CzasPrzelotu == 0)break;
               k += 1;

               if(Wierzsz-1 > 0)
               {
                   k = 0;
                   Wierzsz -= 1;
               }
           }
        }

        plik.close();

        return (k-1);

    }else
        return -1;

}

void Magazyn_danych::DodajNoweDane(ISS_Dane NoweDane)
{
    MagazynDanych[Glowa].CzasPrzelotu = NoweDane.CzasPrzelotu;
    MagazynDanych[Glowa].Predkosc = NoweDane.Predkosc;
    MagazynDanych[Glowa].Wysokosc = NoweDane.Wysokosc;
    MagazynDanych[Glowa].DlugoscGeo = NoweDane.DlugoscGeo;
    MagazynDanych[Glowa].SzerokoscGeo = NoweDane.SzerokoscGeo;

    Glowa += 1;
    Glowa = Glowa%MagazynDanych.size();
}
