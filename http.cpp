#include "http.h"

Http::Http(QObject* Rodzic)
    : QObject (Rodzic)
{
    connect(&http, SIGNAL(finished(QNetworkReply*)), this, SLOT (PobranoStrone(QNetworkReply*))); //Kiedy skonczy pobierac odpowiedz wywola pobrano strone
}

void Http::PobranoStrone(QNetworkReply* Odpowiedz)
{
    htmlStrony = Odpowiedz->readAll(); //Odczytuje wszystkie dane
    Odpowiedz->deleteLater(); //Usuwa obiekt
    emit pobrano();
}

void Http::PobierzStrone(const QUrl &Url)
{
    QNetworkRequest Odpowiedz(Url); //Wysyla zapytanie
    http.get(Odpowiedz); //Pobiera odpowiedz

}

QString Http::ZwrocZawartoscStrony()
{
    return htmlStrony;
}

ISS_Dane Http::PobierzDaneOISS()
{
    QTimer MaxCzasPobierania;
    MaxCzasPobierania.setSingleShot(true);
    QEventLoop PetlaOczekiwania;

    connect(this, SIGNAL(pobrano()), &PetlaOczekiwania, SLOT(quit()));
    connect(&MaxCzasPobierania, SIGNAL(timeout()), &PetlaOczekiwania, SLOT(quit()));

    ISS_Dane Dane;
    QUrl StronaZDanymi("https://api.wheretheiss.at/v1/satellites/25544");

    this->PobierzStrone(StronaZDanymi);

    MaxCzasPobierania.start(1000);
    PetlaOczekiwania.exec();

    if(htmlStrony.isEmpty())
    {
      /*QMessageBox* Komunikat = new QMessageBox();
        Komunikat->setText("Nie udało się pobrać danych z strony: \nhttps://api.wheretheiss.at/v1/satellites/25544");
        Komunikat->setStandardButtons(QMessageBox::Ok);
        Komunikat->show();*/
        std::cerr<<"Nie pobrano danych ze strony"<<std::endl;
        return Dane;
    }

    Dane = this->ParserDanychISS();

    return Dane;
}

void Http::PobierzDaneOISS(ISS_Dane *TablicaDanych, int IloscDanych, uint PoczatekCzasu, uint Czestotliwoasc)
{
    if(IloscDanych >= 10) IloscDanych = 10;

    QTimer MaxCzasPobierania;
    MaxCzasPobierania.setSingleShot(true);
    QEventLoop PetlaOczekiwania;
    QString NapisPomocniczy = "https://api.wheretheiss.at/v1/satellites/25544/positions?timestamps=";

    connect(this, SIGNAL(pobrano()), &PetlaOczekiwania, SLOT(quit()));
    connect(&MaxCzasPobierania, SIGNAL(timeout()), &PetlaOczekiwania, SLOT(quit()));

    for (uint i = 0; i <IloscDanych; i++)
    {
        NapisPomocniczy.append(QString::number(PoczatekCzasu + i*Czestotliwoasc));
        NapisPomocniczy.append(",");
    }

    NapisPomocniczy.remove(NapisPomocniczy.length()-1,1);

    QUrl StronaZDanymi(NapisPomocniczy);
    PobierzStrone(StronaZDanymi);

    MaxCzasPobierania.start(1000);
    PetlaOczekiwania.exec();

    if(htmlStrony.isEmpty())
    {
      /*QMessageBox* Komunikat = new QMessageBox();
        Komunikat->setText("Nie udało się pobrać danych z strony: \nhttps://api.wheretheiss.at/v1/satellites/25544");
        Komunikat->setStandardButtons(QMessageBox::Ok);
        Komunikat->show();*/
        std::cerr<<"Nie pobrano danych ze strony"<<std::endl;
        return;
    }

   PraserWieluDanychISS(TablicaDanych, IloscDanych);

}

ISS_Dane Http::ParserDanychISS()
{
    ISS_Dane NoweDane;
    int PoczatekWyrazenia;
    int KoniecWyrazenia;
    int DlugoscWyrazenia;
    QString NapisPomocniczy;

    htmlStrony.remove(0,1);

    for(int i = 0; i < htmlStrony.size(); i++)
    {
        if(htmlStrony[i] == '"')
            htmlStrony.remove(i,1);
    }

    PoczatekWyrazenia = htmlStrony.indexOf("latitude");
    DlugoscWyrazenia = htmlStrony.indexOf(':', PoczatekWyrazenia) + 1 - PoczatekWyrazenia;
    KoniecWyrazenia = htmlStrony.indexOf(',', PoczatekWyrazenia);

    NapisPomocniczy = htmlStrony.mid(PoczatekWyrazenia + DlugoscWyrazenia, KoniecWyrazenia - (PoczatekWyrazenia + DlugoscWyrazenia));

    NoweDane.SzerokoscGeo = NapisPomocniczy.toDouble();

    PoczatekWyrazenia = htmlStrony.indexOf("longitude");
    DlugoscWyrazenia = htmlStrony.indexOf(':', PoczatekWyrazenia) + 1 - PoczatekWyrazenia;
    KoniecWyrazenia = htmlStrony.indexOf(',', PoczatekWyrazenia);

    NapisPomocniczy = htmlStrony.mid(PoczatekWyrazenia + DlugoscWyrazenia, KoniecWyrazenia - (PoczatekWyrazenia + DlugoscWyrazenia));

    NoweDane.DlugoscGeo = NapisPomocniczy.toDouble();

    PoczatekWyrazenia = htmlStrony.indexOf("altitude");
    DlugoscWyrazenia = htmlStrony.indexOf(':', PoczatekWyrazenia) + 1 - PoczatekWyrazenia;
    KoniecWyrazenia = htmlStrony.indexOf(',', PoczatekWyrazenia);

    NapisPomocniczy = htmlStrony.mid(PoczatekWyrazenia + DlugoscWyrazenia, KoniecWyrazenia - (PoczatekWyrazenia + DlugoscWyrazenia));

    NoweDane.Wysokosc = NapisPomocniczy.toDouble();

    PoczatekWyrazenia = htmlStrony.indexOf("velocity");
    DlugoscWyrazenia = htmlStrony.indexOf(':', PoczatekWyrazenia) + 1 - PoczatekWyrazenia;
    KoniecWyrazenia = htmlStrony.indexOf(',', PoczatekWyrazenia);

    NapisPomocniczy = htmlStrony.mid(PoczatekWyrazenia + DlugoscWyrazenia, KoniecWyrazenia - (PoczatekWyrazenia + DlugoscWyrazenia));

    NoweDane.Predkosc = NapisPomocniczy.toDouble();

    PoczatekWyrazenia = htmlStrony.indexOf("timestamp");
    DlugoscWyrazenia = htmlStrony.indexOf(':', PoczatekWyrazenia) + 1 - PoczatekWyrazenia;
    KoniecWyrazenia = htmlStrony.indexOf(',', PoczatekWyrazenia);

    NapisPomocniczy = htmlStrony.mid(PoczatekWyrazenia + DlugoscWyrazenia, KoniecWyrazenia - (PoczatekWyrazenia + DlugoscWyrazenia));

    NoweDane.CzasPrzelotu = NapisPomocniczy.toUInt();

    return  NoweDane;
}

void Http::PraserWieluDanychISS(ISS_Dane* TablicaDanych, int IloscDanych)
{
    ISS_Dane NoweDane;
    int PoczatekWyrazenia = 0;
    int KoniecWyrazenia = 0;
    int DlugoscWyrazenia = 0;
    QString NapisPomocniczy;

    htmlStrony.remove(0,1);

    for(int i = 0; i < htmlStrony.size(); i++)
    {
        if(htmlStrony[i] == '"')
            htmlStrony.remove(i,1);
    }

    for(int i = 0; i < IloscDanych; i++)
    {
        PoczatekWyrazenia = htmlStrony.indexOf("latitude", KoniecWyrazenia);
        DlugoscWyrazenia = htmlStrony.indexOf(':', PoczatekWyrazenia) + 1 - PoczatekWyrazenia;
        KoniecWyrazenia = htmlStrony.indexOf(',', PoczatekWyrazenia);

        NapisPomocniczy = htmlStrony.mid(PoczatekWyrazenia + DlugoscWyrazenia, KoniecWyrazenia - (PoczatekWyrazenia + DlugoscWyrazenia));

        TablicaDanych[i].SzerokoscGeo = NapisPomocniczy.toDouble();

        PoczatekWyrazenia = htmlStrony.indexOf("longitude", KoniecWyrazenia);
        DlugoscWyrazenia = htmlStrony.indexOf(':', PoczatekWyrazenia) + 1 - PoczatekWyrazenia;
        KoniecWyrazenia = htmlStrony.indexOf(',', PoczatekWyrazenia);

        NapisPomocniczy = htmlStrony.mid(PoczatekWyrazenia + DlugoscWyrazenia, KoniecWyrazenia - (PoczatekWyrazenia + DlugoscWyrazenia));

        TablicaDanych[i].DlugoscGeo = NapisPomocniczy.toDouble();

        PoczatekWyrazenia = htmlStrony.indexOf("altitude", KoniecWyrazenia);
        DlugoscWyrazenia = htmlStrony.indexOf(':', PoczatekWyrazenia) + 1 - PoczatekWyrazenia;
        KoniecWyrazenia = htmlStrony.indexOf(',', PoczatekWyrazenia);

        NapisPomocniczy = htmlStrony.mid(PoczatekWyrazenia + DlugoscWyrazenia, KoniecWyrazenia - (PoczatekWyrazenia + DlugoscWyrazenia));

        TablicaDanych[i].Wysokosc = NapisPomocniczy.toDouble();

        PoczatekWyrazenia = htmlStrony.indexOf("velocity", KoniecWyrazenia);
        DlugoscWyrazenia = htmlStrony.indexOf(':', PoczatekWyrazenia) + 1 - PoczatekWyrazenia;
        KoniecWyrazenia = htmlStrony.indexOf(',', PoczatekWyrazenia);

        NapisPomocniczy = htmlStrony.mid(PoczatekWyrazenia + DlugoscWyrazenia, KoniecWyrazenia - (PoczatekWyrazenia + DlugoscWyrazenia));

        TablicaDanych[i].Predkosc = NapisPomocniczy.toDouble();

        PoczatekWyrazenia = htmlStrony.indexOf("timestamp", KoniecWyrazenia);
        DlugoscWyrazenia = htmlStrony.indexOf(':', PoczatekWyrazenia) + 1 - PoczatekWyrazenia;
        KoniecWyrazenia = htmlStrony.indexOf(',', PoczatekWyrazenia);

        NapisPomocniczy = htmlStrony.mid(PoczatekWyrazenia + DlugoscWyrazenia, KoniecWyrazenia - (PoczatekWyrazenia + DlugoscWyrazenia));

        TablicaDanych[i].CzasPrzelotu = NapisPomocniczy.toUInt();
    }

}
