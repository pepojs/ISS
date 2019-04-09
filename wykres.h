#ifndef WYKRES_H
#define WYKRES_H

#include <QWidget>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QtCharts/QDateTimeAxis>
#include <QEvent>
#include <QMouseEvent>

#include "iss_dane.h"

QT_CHARTS_USE_NAMESPACE

class Wykres : public QWidget
{
Q_OBJECT
public:
    Wykres(QWidget* Rodzic = nullptr);
    void ZmienWykres(QRgb NowyKolor, int NowyRozmiar);
    void ZmienKolorTla(QRgb NowyKolor);
    void UstawTytulWykresu(QString NowyTytul);
    void ZmienKolorTytulu(QRgb NowyKolor);
    void ZmienRozmiarTytulu(int NowyRozmiar);
    void UstawZakresOsiX(uint Min, uint Max);
    void UstawZakresOsiY(double Min, double Max);
    void ZmienIloscPodzialekOsiX(int NowaIlosc);
    void ZmienIloscPodzialekOsiY(int NowaIlosc);
    void ZmienRozmiarPodpisuOsiX(int NowyRozmiar);
    void ZmienRozmiarPodpisuOsiY(int NowyRozmiar);
    void ZmienGruboscOsiX(int NowyRozmiar);
    void ZmienGruboscOsiY(int NowyRozmiar);
    void ZmienKolorOsiX(QRgb NowyKolor);
    void ZmienKolorOsiY(QRgb NowyKolor);
    void ZmienPodpisOsiX(QString NowyPodpis);
    void ZmienPodpisOsiY(QString NowyPodpis);
    void DodajDaneDoWykresu(uint X, double Y);
    void WyczyscWykres();
    void WidokSiatki(bool Wlacz);

signals:
    void Kliknieto();

protected:
    bool event(QEvent* Zdarzenie);

private:
    QChartView* WidokWykresu;
    QChart* PoleWykresu;
    QLineSeries* LiniaWykresu;
    QDateTimeAxis* OsX;
    QValueAxis* OsY;

    QGridLayout* WarstwaGlowna;
};

#endif // WYKRES_H
