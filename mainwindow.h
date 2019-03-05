#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <QObject>
#include <QLabel>
#include <QTimer>
#include <QDateTime>

#include "http.h"
#include "iss_dane.h"
#include "zegar.h"
#include "lokalizator.h"
#include "wykres.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void WypelniDanymi(QString* dane){*dane = DaneStrony;}

signals:
    void noweDaneISS(ISS_Dane);

private slots:
    void PobierzNoweDaneISS();


private:
    Ui::MainWindow *ui;
    QString DaneStrony;
    Http* Html;
    ISS_Dane DaneStacji;
    QTimer* CzasDoPobrania;
    QLabel* TytulISS;
    Lokalizator* WyswietlaczPolozeniaISS;
    QPushButton* PrzyciskWspolrzedne;
    Zegar* CzasPrzelotuISS;
    QPushButton* PrzyciskCzas;
    Wykres* WykresPredkosci;
    QPushButton* PrzyciskPredkosc;
    Wykres* WykresWysokosci;
    QPushButton* PrzyciskWysokosc;

    uint ObecnyZakresOsiCzasu;
    double ObecnyZakresOsiPredkosci;
    double ObecnyZakresOsiWysokosci;
};

#endif // MAINWINDOW_H
