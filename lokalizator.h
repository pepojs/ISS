#ifndef LOKALIZATOR_H
#define LOKALIZATOR_H

#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>

#include "iss_dane.h"

class Lokalizator : public QWidget
{
    Q_OBJECT
public:
    Lokalizator(QWidget* Rodzic = nullptr);
    void ZmienSzerokoscGeo(double NowaSzerokosc);
    void ZmienDlugoscGeo(double NowaDlugosc);

signals:
    void Kliknieto();

protected:
    bool event(QEvent* Zdarzenie);

private slots:
    void AktualizujDaneISS(ISS_Dane NoweDane);

private:
    QLabel* Tytul;
    QLabel* SzerokoscZnak;
    QLabel* DlugoscZnak;
    QLCDNumber* SzerokoscGeo;
    QLCDNumber* DlugoscGeo;

    QHBoxLayout* WarstwaWyswietlaczy;
    QVBoxLayout* WarstwaGlowna;

};

#endif // LOKALIZATOR_H
