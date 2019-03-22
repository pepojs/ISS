#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <QObject>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QWindow>
#include <QSlider>

#include "http.h"
#include "iss_dane.h"
#include "zegar.h"
#include "lokalizator.h"
#include "wykres.h"
#include "openglwidget.h"


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
    void PrzycisnietyCzas();
    void PrzycisnietyWspolrzedne();
    void PrzycisnietyWysokosc();


private:
    Ui::MainWindow *ui;
    QString DaneStrony;
    Http* Html;
    ISS_Dane DaneStacji;
    QTimer* CzasDoPobrania;
    QLabel* TytulISS;
    Lokalizator* WyswietlaczPolozeniaISS;
    Zegar* CzasPrzelotuISS;
    Wykres* WykresPredkosci;
    Wykres* WykresWysokosci;

    OpenGLWidget* Model3D;

    QSlider* StrefaCzasowa;
    QLabel* WybranaStrefa;

    uint ObecnyZakresOsiCzasu;
    double ObecnyZakresOsiPredkosci;
    double ObecnyZakresOsiWysokosci;
};

#endif // MAINWINDOW_H
