#include "lokalizator.h"

Lokalizator::Lokalizator(QWidget* Rodzic)
    :QWidget (Rodzic)
{
    Tytul = new QLabel(tr("<center><font color=white>Wspolrzedne Geograficzne</font></center>"));
    SzerokoscZnak = new QLabel("N");
    DlugoscZnak = new QLabel("W");
    SzerokoscGeo = new QLCDNumber;
    DlugoscGeo = new QLCDNumber;

    WarstwaWyswietlaczy = new QHBoxLayout;
    WarstwaGlowna = new QVBoxLayout;

    WarstwaWyswietlaczy->addWidget(SzerokoscGeo);
    WarstwaWyswietlaczy->addWidget(SzerokoscZnak);
    WarstwaWyswietlaczy->addStretch();
    WarstwaWyswietlaczy->addWidget(DlugoscGeo);
    WarstwaWyswietlaczy->addWidget(DlugoscZnak);

    WarstwaGlowna->addWidget(Tytul);
    WarstwaGlowna->addLayout(WarstwaWyswietlaczy);
    setLayout(WarstwaGlowna);

    SzerokoscGeo->setSegmentStyle(QLCDNumber::Outline);
    DlugoscGeo->setSegmentStyle(QLCDNumber::Outline);

    SzerokoscGeo->setDigitCount(6);
    DlugoscGeo->setDigitCount(6);

}

void Lokalizator::ZmienSzerokoscGeo(double NowaSzerokosc)
{
    if(NowaSzerokosc >= 0)
    {
        SzerokoscGeo->display(NowaSzerokosc);
        SzerokoscZnak->setText(tr("<font color=white>N</font>"));

    }else
    {
        SzerokoscGeo->display(-NowaSzerokosc);
        SzerokoscZnak->setText(tr("<font color=white>S</font>"));
    }
}

void Lokalizator::ZmienDlugoscGeo(double NowaDlugosc)
{
    if(NowaDlugosc >= 0)
    {
        DlugoscGeo->display(NowaDlugosc);
        DlugoscZnak->setText(tr("<font color=white>E</font>"));

    }else
    {
        DlugoscGeo->display(-NowaDlugosc);
        DlugoscZnak->setText(tr("<font color=white>W</font>"));
    }
}

void Lokalizator::AktualizujDaneISS(ISS_Dane NoweDane)
{
    this->ZmienSzerokoscGeo(NoweDane.SzerokoscGeo);
    this->ZmienDlugoscGeo(NoweDane.DlugoscGeo);
}




