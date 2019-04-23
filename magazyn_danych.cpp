#include "magazyn_danych.h"

/*!
 * \file
 * \brief Definicja meody klasy Magazyn_danych.
 *
 * Zawiera definicje metod klasy Magazyn_danych.
 */

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

Magazyn_danych::~Magazyn_danych()
{
    ZapiszDane(60);
}

void Magazyn_danych::WypelniDanymiZSieci(Http *Html, uint32_t Czestotliwosc, uint32_t IloscDanychWypelnionych, uint32_t CzasDoTylu_S)
{
    ISS_Dane NoweDane = Html->PobierzDaneOISS();
    uint32_t CzasDanych;

    if(IloscDanychWypelnionych == 0)
    {
        if(NoweDane.ZwrocCzasPrzelotu_UTS() == 0)NoweDane = Html->PobierzDaneOISS();

        if(NoweDane.ZwrocCzasPrzelotu_UTS() > CzasDoTylu_S)
            CzasDanych = NoweDane.ZwrocCzasPrzelotu_UTS() - CzasDoTylu_S;
        else
            CzasDanych = QDateTime::currentSecsSinceEpoch() - CzasDoTylu_S;

    }else
    {
        CzasDanych = MagazynDanych[IloscDanychWypelnionych].ZwrocCzasPrzelotu_UTS()+Czestotliwosc;
        IloscDanychWypelnionych += 1;
    }

    ISS_Dane TabPom[10];
    size_t licznik = 0;
    size_t roznica = 0;
    size_t licznik_pom = 0;
    size_t licznik_prob = 0;
    size_t wykryto0 = 0;

    cout<<"Czas danych "<<CzasDanych<<endl;
    cout<<"Ilosc "<<IloscDanychWypelnionych<<endl;
    cout<<"Pobieranie danych do magazynu z sieci"<<endl;

    do
    {
        NoweDane = Html->PobierzDaneOISS();
        licznik += 1;

    }while(NoweDane.ZwrocCzasPrzelotu_UTS() == 0 && licznik < 60);

    if(licznik >= 60)
    {
        cerr<<"Nie udalo nawiazac sie poloaczenia ze strona"<<endl;
        return;
    }

    cout<<"Licznik "<<licznik<<endl;
    licznik = 0;

    for(licznik = IloscDanychWypelnionych; licznik < (MagazynDanych.size()-10);)
    {
        licznik_pom = licznik;
        licznik_prob = 0;

        do
        {
            licznik = licznik_pom;
            cout<<"Czas danych: "<<CzasDanych<<endl;

            Html->PobierzDaneOISS(&TabPom[0], 10, CzasDanych, Czestotliwosc);
            licznik_prob += 1;

            licznik_prob += 1;

        }while((TabPom[0].ZwrocCzasPrzelotu_UTS() == 0 || TabPom[0].ZwrocCzasPrzelotu_UTS() != CzasDanych) && licznik_prob < 60);

        if(licznik_prob >= 60)
        {
            cerr<<"Polaczenie z serwerem zostalo przerwane"<<endl;
            return;
        }

        for(size_t j = 0; j < 10; j++)
        {
            MagazynDanych[licznik] = TabPom[j];
            cout<<"Mag: "<<MagazynDanych[licznik].ZwrocCzasPrzelotu_UTS()<<endl;
            licznik += 1;
        }

        CzasDanych += 10*Czestotliwosc;
        cout<<"Pobrano "<<licznik<<" danych"<<endl;
    }


    licznik_pom = licznik;
    licznik_prob = 0;

    roznica = MagazynDanych.size()-licznik;
    cout<<"roznica: "<<roznica<<endl;
    do
    {
        wykryto0 = 0;
        licznik = licznik_pom;
        Html->PobierzDaneOISS(&TabPom[0], roznica, CzasDanych, Czestotliwosc);
        for(size_t j = 0; j < roznica; j++)
        {
            if(TabPom[j].ZwrocCzasPrzelotu_UTS() == 0)
            {
                wykryto0 = 1;
                break;
            }
            MagazynDanych[licznik] = TabPom[j];
            cout<<"Mag: "<<MagazynDanych[licznik].ZwrocCzasPrzelotu_UTS()<<endl;
            licznik += 1;
        }
        licznik_prob += 1;

    }while(wykryto0 != 0 && licznik_prob < 60 && roznica > 0);

    if(licznik_prob >= 60)
    {
        cerr<<"Polaczenie z serwerem zostalo przerwane"<<endl;
        return;
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
            if(MagazynDanych[GlowaKopia1].ZwrocCzasPrzelotu_UTS() != 0)break;
            GlowaKopia1 += 1;
            GlowaKopia1 = GlowaKopia1%MagazynDanych.size();
        }

        GlowaKopia2 = Glowa;
        for(i = 0; i < MagazynDanych.size(); i++)
        {
            if(MagazynDanych[GlowaKopia2].ZwrocCzasPrzelotu_UTS() == 0)break;
            GlowaKopia2 += 1;
            GlowaKopia2 = GlowaKopia2%MagazynDanych.size();
        }

        if(GlowaKopia2 == 0)
            GlowaKopia2 = MagazynDanych.size();

        plik<<MagazynDanych[GlowaKopia1].ZwrocCzasPrzelotu_UTS()<<" "<<MagazynDanych[GlowaKopia2-1].ZwrocCzasPrzelotu_UTS()<<" "<<Czestotliwosc<<endl;

        GlowaKopia1 = Glowa;
        for (i = 0; i < MagazynDanych.size(); i++)
        {
            plik<<MagazynDanych[GlowaKopia1];
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

    if(NoweDane.ZwrocCzasPrzelotu_UTS() == 0)NoweDane = Html->PobierzDaneOISS();

    if(NoweDane.ZwrocCzasPrzelotu_UTS() > CzasDoTylu_S)
        CzasDanych = NoweDane.ZwrocCzasPrzelotu_UTS() - CzasDoTylu_S;
    else
        CzasDanych = QDateTime::currentSecsSinceEpoch() - CzasDoTylu_S;


    cout<<"Czas danych: "<<CzasDanych<<endl;
    if(plik.good() == true)
    {
        plik>>CzasPoczatku>>CzasKonca>>Czestotliwosc;

        if(CzasDanych > CzasKonca) return 0;
        else
        {
           Wierzsz = (uint32_t)(ceil((float)(CzasDanych - CzasPoczatku)/(float)Czestotliwosc)+1);

           k = 0;
           while(!plik.eof())
           {
               plik>>MagazynDanych[k];

               if(MagazynDanych[k].ZwrocCzasPrzelotu_UTS() == 0)break;
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
    MagazynDanych[Glowa].ZmienCzasPrzelotu_UTS(NoweDane.ZwrocCzasPrzelotu_UTS());
    MagazynDanych[Glowa].ZmienPredkosc_kmH(NoweDane.ZwrocPredkosc_kmH());
    MagazynDanych[Glowa].ZmienWysokosc_km(NoweDane.ZwrocWysokosc_km());
    MagazynDanych[Glowa].ZmienDlugoscGeo_Stopnie(NoweDane.ZwrocDlugoscGeo_Stopnie());
    MagazynDanych[Glowa].ZmienSzerokoscGeo_Stopnie(NoweDane.ZwrocSzerokoscGeo_Stopnie());

    Glowa += 1;
    Glowa = Glowa%MagazynDanych.size();
}
