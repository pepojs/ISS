#include "mainwindow.h"
#include "ui_mainwindow.h"

//QUrl str("http://wsn.spaceflight.esa.int/iss/index_portal.php");
//QUrl str("https://isstracker.pl/?lang=pl");
//QUrl str("http://www.satview.org/?sat_id=37820U");
//QUrl str("https://api.wheretheiss.at/v1/satellites/25544");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setGeometry(100, 30, 1024,750);
    QSize RozmiarOkna = size();
    CzasDoPobrania = new QTimer;


    Html = new Http();

    CzasPrzelotuISS = new Zegar(this);
    CzasPrzelotuISS->setGeometry(20,60,350,100);
    CzasPrzelotuISS->ZmienStrefeCzasowa(+1);

    WyswietlaczPolozeniaISS = new Lokalizator(this);
    WyswietlaczPolozeniaISS->setGeometry(20,160,350,100);

    TytulISS = new QLabel(tr("<center><font size=6 color=white>Miedzynarodowa stacja kosmiczna</font></center>"), this);
    TytulISS->setGeometry(0,0,RozmiarOkna.width(),60);

    WykresPredkosci = new Wykres(this);
    WykresPredkosci->setGeometry(20, 260, 360,250);

    WykresPredkosci->ZmienWykres(QRgb(0xffffff),1);
    WykresPredkosci->ZmienIloscPodzialekOsiY(3);
    WykresPredkosci->ZmienKolorTla(QRgb(0x000000));
    WykresPredkosci->ZmienKolorOsiX(QRgb(0xffffff));
    WykresPredkosci->ZmienKolorOsiY(QRgb(0xffffff));
    WykresPredkosci->ZmienPodpisOsiX(tr("Czas"));
    WykresPredkosci->ZmienPodpisOsiY(tr("Predkosc [km/h]"));

    WykresWysokosci = new Wykres(this);
    WykresWysokosci->setGeometry(20, 490, 360,250);

    WykresWysokosci->ZmienWykres(QRgb(0xffffff),1);
    WykresWysokosci->ZmienIloscPodzialekOsiY(3);
    WykresWysokosci->ZmienKolorTla(QRgb(0x000000));
    WykresWysokosci->ZmienKolorOsiX(QRgb(0xffffff));
    WykresWysokosci->ZmienKolorOsiY(QRgb(0xffffff));
    WykresWysokosci->ZmienPodpisOsiX(tr("Czas"));
    WykresWysokosci->ZmienPodpisOsiY(tr("Wysokosc [km]"));

    StrefaCzasowa = new QSlider(Qt::Horizontal, this);
    StrefaCzasowa->setGeometry(550, 70, 300, 30);
    StrefaCzasowa->setMinimum(-12);
    StrefaCzasowa->setMaximum(12);
    StrefaCzasowa->setValue(1);
    StrefaCzasowa->hide();

    WybranaStrefa = new QLabel(tr("<center><font color=white>Strefa: UTC-1</font></center>"), this);
    WybranaStrefa->setGeometry(550,100,300,30);
    WybranaStrefa->hide();

    QObject::connect(CzasDoPobrania, SIGNAL(timeout()), this, SLOT(PobierzNoweDaneISS()));
    QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
    QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));


    emit noweDaneISS(DaneStacji);
    CzasDoPobrania->start(2000);

    ObecnyZakresOsiCzasu = DaneStacji.CzasPrzelotu;
    ObecnyZakresOsiPredkosci = DaneStacji.Predkosc;
    ObecnyZakresOsiWysokosci = DaneStacji.Wysokosc;

    this->setStyleSheet("background-color:black;");
    Model3D = new OpenGLWidget(this);
    Model3D->setGeometry(400, 60, RozmiarOkna.width()-400,RozmiarOkna.height()-60);
    Model3D->SledzenieStacji(true);

    QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), Model3D, SLOT(NoweDaneISS(ISS_Dane)));

    QObject::connect(WyswietlaczPolozeniaISS, SIGNAL(Kliknieto()), this, SLOT(PrzycisnietyWspolrzedne()));
    QObject::connect(CzasPrzelotuISS, SIGNAL(Kliknieto()), this, SLOT(PrzycisnietyCzas()));
    QObject::connect(WykresWysokosci, SIGNAL(Kliknieto()), this, SLOT(PrzycisnietyWysokosc()));

}

MainWindow::~MainWindow()
{

}

void MainWindow::PobierzNoweDaneISS()
{
    DaneStacji = Html->PobierzDaneOISS();
    //DodajElementDoDanych(DaneStacji);

    if(DaneStacji.Wysokosc > 0)
    {
        if(ObecnyZakresOsiPredkosci+5 <= DaneStacji.Predkosc)
        {
            ObecnyZakresOsiPredkosci = DaneStacji.Predkosc;
            WykresPredkosci->UstawZakresOsiY(DaneStacji.Predkosc-5, DaneStacji.Predkosc+5);
        }

        if(ObecnyZakresOsiWysokosci+2 <= DaneStacji.Wysokosc)
        {
            ObecnyZakresOsiWysokosci = DaneStacji.Wysokosc;
            WykresWysokosci->UstawZakresOsiY(DaneStacji.Wysokosc-2, DaneStacji.Wysokosc+2);
        }

        if(ObecnyZakresOsiCzasu+30 <= DaneStacji.CzasPrzelotu)
        {
            ObecnyZakresOsiCzasu = DaneStacji.CzasPrzelotu;
            WykresPredkosci->UstawZakresOsiX(DaneStacji.CzasPrzelotu-30, DaneStacji.CzasPrzelotu+30);
            WykresWysokosci->UstawZakresOsiX(DaneStacji.CzasPrzelotu-30, DaneStacji.CzasPrzelotu+30);
        }

        WykresPredkosci->DodajDaneDoWykresu(DaneStacji.CzasPrzelotu, DaneStacji.Predkosc);
        WykresWysokosci->DodajDaneDoWykresu(DaneStacji.CzasPrzelotu, DaneStacji.Wysokosc);

        emit noweDaneISS(DaneStacji);
    }




}

void MainWindow::PrzycisnietyCzas()
{
   /* Model3D->hide();
    QObject::disconnect(this, SIGNAL(noweDaneISS(ISS_Dane)), Model3D, SLOT(NoweDaneISS(ISS_Dane)));
    */
    Model3D->RysowaniePunktuSledzacego(false);
    Model3D->RysowanieTrajektorii(false);
    Model3D->SledzenieStacji(false);
    Model3D->RysowanieSiatki(true);
    StrefaCzasowa->show();
    WybranaStrefa->show();
    Model3D->setGeometry(400, 130, size().width()-400, size().height()-130);
    Model3D->update();
}

void MainWindow::PrzycisnietyWspolrzedne()
{
    Model3D->SledzenieStacji(true);
    Model3D->RysowaniePunktuSledzacego(true);
    Model3D->RysowanieTrajektorii(false);
    Model3D->RysowanieSiatki(false);
    StrefaCzasowa->hide();
    WybranaStrefa->hide();
    Model3D->setGeometry(400, 60, size().width()-400,size().height()-60);
    Model3D->update();

}

void MainWindow::PrzycisnietyWysokosc()
{
    Model3D->RysowanieTrajektorii(true);
    Model3D->RysowaniePunktuSledzacego(true);
    Model3D->RysowanieSiatki(false);
    StrefaCzasowa->hide();
    WybranaStrefa->hide();
    Model3D->setGeometry(400, 60, size().width()-400,size().height()-60);
    Model3D->update();
}

