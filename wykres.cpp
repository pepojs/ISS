#include "wykres.h"

/*!
 * \file
 * \brief Definicja meody klasy Wykres.
 *
 * Zawiera definicje metod klasy Wykres.
 */


Wykres::Wykres(QWidget* Rodzic)
    :QWidget (Rodzic)
{
    LiniaWykresu = new QLineSeries;

    PoleWykresu = new QChart;
    PoleWykresu->addSeries(LiniaWykresu);
    PoleWykresu->legend()->hide();

    //Ustawienie koloru wykresu
    QPen KolorWykres(QRgb(0xff0000));
    KolorWykres.setWidth(5);

    LiniaWykresu->setPen(KolorWykres);

    QFont CzcionkaWykres;
    CzcionkaWykres.setPixelSize(18);
    PoleWykresu->setTitleFont(CzcionkaWykres);

    QBrush KolorTla(QRgb(0x0000ff));
    PoleWykresu->setBackgroundBrush(KolorTla);
    PoleWykresu->setPlotAreaBackgroundVisible(false);

    OsX = new QDateTimeAxis;
    OsY = new QValueAxis;

    QFont CzcionkaOsi;
    CzcionkaOsi.setPixelSize(12);
    OsX->setLabelsFont(CzcionkaOsi);
    OsY->setLabelsFont(CzcionkaOsi);

    QPen KolorOsie(QRgb(0xff0000));
    KolorOsie.setWidth(2);
    OsX->setLinePen(KolorOsie);
    OsY->setLinePen(KolorOsie);


    OsX->setGridLineVisible(false);
    OsY->setGridLineVisible(false);

    OsY->setRange(0,30);
    OsY->setTickCount(5);

    OsX->setTickCount(3);
    OsX->setFormat("hh:mm:ss");
    OsX->setTitleText("Dane");

    PoleWykresu->addAxis(OsX, Qt::AlignBottom);
    PoleWykresu->addAxis(OsY, Qt::AlignLeft);

    LiniaWykresu->attachAxis(OsX);
    LiniaWykresu->attachAxis(OsY);

    WidokWykresu = new QChartView(PoleWykresu);
    WidokWykresu->setRenderHint(QPainter::Antialiasing);

    WarstwaGlowna = new QGridLayout;
    WarstwaGlowna->addWidget(WidokWykresu, 0 ,0);
    setLayout(WarstwaGlowna);


}

bool Wykres::event(QEvent* Zdarzenie)
{
    QMouseEvent *Mysz;

    switch(Zdarzenie->type())
    {
        case QEvent::MouseButtonPress:
            Mysz = static_cast<QMouseEvent* >(Zdarzenie);

            if(Mysz->button() == Qt::LeftButton)
            {
                emit Kliknieto();
            }
        break;

        default:
        break;
    }

    return QWidget::event(Zdarzenie);
}


void Wykres::ZmienWykres(QRgb NowyKolor, int NowyRozmiar)
{
    QPen KolorWykresu(NowyKolor);
    KolorWykresu.setWidth(NowyRozmiar);
    LiniaWykresu->setPen(KolorWykresu);
}

void Wykres::ZmienKolorTla(QRgb NowyKolor)
{
    QBrush KolorTla(NowyKolor);
    PoleWykresu->setBackgroundBrush(KolorTla);
}

void Wykres::UstawTytulWykresu(QString NowyTytul)
{
    PoleWykresu->setTitle(NowyTytul);
}

void Wykres::ZmienKolorTytulu(QRgb NowyKolor)
{
    QBrush KolorTytulu(NowyKolor);
    PoleWykresu->setTitleBrush(KolorTytulu);
}

void Wykres::ZmienRozmiarTytulu(int NowyRozmiar)
{
    QFont CzcionkaTytulu;
    CzcionkaTytulu.setPixelSize(NowyRozmiar);
    PoleWykresu->setTitleFont(CzcionkaTytulu);
}

void Wykres::UstawZakresOsiX(uint Min, uint Max)
{
    QDateTime CzasMin, CzasMax;
    CzasMax.setTime_t(Max);
    CzasMin.setTime_t(Min);
    OsX->setRange(CzasMin, CzasMax);
}

void Wykres::UstawZakresOsiY(double Min, double Max)
{
    OsY->setRange(Min,Max);
}

void Wykres::ZmienIloscPodzialekOsiX(int NowaIlosc)
{
    OsX->setTickCount(NowaIlosc);
}

void Wykres::ZmienIloscPodzialekOsiY(int NowaIlosc)
{
    OsY->setTickCount(NowaIlosc);
}

void Wykres::ZmienRozmiarPodpisuOsiX(int NowyRozmiar)
{
    QFont CzcionkaOsi;
    CzcionkaOsi.setPixelSize(NowyRozmiar);
    OsX->setLabelsFont(CzcionkaOsi);
}

void Wykres::ZmienRozmiarPodpisuOsiY(int NowyRozmiar)
{
    QFont CzcionkaOsi;
    CzcionkaOsi.setPixelSize(NowyRozmiar);
    OsY->setLabelsFont(CzcionkaOsi);
}

void Wykres::ZmienGruboscOsiX(int NowyRozmiar)
{
    QPen GruboscOsi;
    GruboscOsi.setWidth(NowyRozmiar);
    OsX->setLinePen(GruboscOsi);
}

void Wykres::ZmienGruboscOsiY(int NowyRozmiar)
{
    QPen GruboscOsi;
    GruboscOsi.setWidth(NowyRozmiar);
    OsY->setLinePen(GruboscOsi);
}

void Wykres::ZmienKolorOsiX(QRgb NowyKolor)
{
    OsX->setLinePenColor(NowyKolor);
    OsX->setLabelsColor(NowyKolor);
}

void Wykres::ZmienKolorOsiY(QRgb NowyKolor)
{
    OsY->setLinePenColor(NowyKolor);
    OsY->setLabelsColor(NowyKolor);
}

void Wykres::ZmienPodpisOsiX(QString NowyPodpis)
{
    OsX->setTitleText(NowyPodpis);
}

void Wykres::ZmienPodpisOsiY(QString NowyPodpis)
{
    OsY->setTitleText(NowyPodpis);
}

void Wykres::DodajDaneDoWykresu(uint X, double Y)
{
    QDateTime Czas;
    Czas.setTime_t(X);
    LiniaWykresu->append(Czas.toMSecsSinceEpoch(), Y);
}

void Wykres::WyczyscWykres()
{
    LiniaWykresu->clear();
}

void Wykres::WidokSiatki(bool Wlacz)
{
    OsX->setGridLineVisible(Wlacz);
    OsY->setGridLineVisible(Wlacz);
}

