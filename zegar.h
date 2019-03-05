#ifndef ZEGAR_H
#define ZEGAR_H

#include <QLCDNumber>
#include <QLabel>
#include <QWidget>
#include <QDateTime>
#include <QVBoxLayout>
#include <iostream>

#include "iss_dane.h"

class Zegar : public QWidget
{
    Q_OBJECT
public:
    Zegar(QWidget* Rodzic = nullptr);
    void UstawCzasUnixFormatUTC1(uint NowyCzas);
    void ZmienStrefeCzasowa(int Strefa);

private slots:
    void AktualizujDaneISS(ISS_Dane NoweDane);

private:
    int Strefa;

    QLabel* Czas;
    QLabel* StrefaCzasowa;
    QLCDNumber* Wyswietlacz;

    QVBoxLayout* GlownaWarstwa;
};

#endif // ZEGAR_H
