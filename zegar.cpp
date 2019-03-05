#include "zegar.h"

Zegar::Zegar(QWidget* Rodzic)
    :QWidget (Rodzic)
{
    Czas = new QLabel(tr("<center><font color=white>Czas przelotu:</font></center>"));
    StrefaCzasowa = new QLabel(tr("<font color=whiteStrefa: UTC-0</font>"));
    Wyswietlacz = new QLCDNumber;

    GlownaWarstwa = new QVBoxLayout;

    GlownaWarstwa->addWidget(Czas);
    GlownaWarstwa->addWidget(Wyswietlacz);
    GlownaWarstwa->addWidget(StrefaCzasowa);

    setLayout(GlownaWarstwa);

    Wyswietlacz->setSegmentStyle(QLCDNumber::Outline);
    Wyswietlacz->setDigitCount(8);

    Strefa = 0;
}

void Zegar::UstawCzasUnixFormatUTC1(uint NowyCzas)
{
    QDateTime CzasNaZegarze;
    if(Strefa < 0)
        NowyCzas = NowyCzas - uint(abs(Strefa-1)*60*60); //Dodaje lub odejmuje wartosc zwiazana ze strefa
    else
        NowyCzas = NowyCzas + uint((Strefa-1)*60*60);

    CzasNaZegarze.setTime_t(NowyCzas);

    Wyswietlacz->display(CzasNaZegarze.toString(Qt::SystemLocaleLongDate));

}

void Zegar::ZmienStrefeCzasowa(int NowaStrefa)
{
    if(NowaStrefa >= 14)
        Strefa = 14;
    else if(NowaStrefa <= -12)
        Strefa = -12;
    else
        Strefa = NowaStrefa;

    StrefaCzasowa->setText(tr("<font color=white>Strefa: UTC") + QString::number(Strefa)+"</font>");
}

void Zegar::AktualizujDaneISS(ISS_Dane NoweDane)
{
    this->UstawCzasUnixFormatUTC1(NoweDane.CzasPrzelotu);
}



