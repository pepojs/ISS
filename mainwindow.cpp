#include "mainwindow.h"
#include "ui_mainwindow.h"

//QUrl str("http://wsn.spaceflight.esa.int/iss/index_portal.php");
//QUrl str("https://isstracker.pl/?lang=pl");
//QUrl str("http://www.satview.org/?sat_id=37820U");
//QUrl str("https://api.wheretheiss.at/v1/satellites/25544");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setGeometry(100, 100, 1024,800);
    QSize RozmiarOkna = size();
    CzasDoPobrania = new QTimer;


    Html = new Http();
    PrzyciskWspolrzedne = new QPushButton(this);
    PrzyciskWspolrzedne->setGeometry(80,200, 200,60);
    WyswietlaczPolozeniaISS = new Lokalizator(PrzyciskWspolrzedne);

    PrzyciskCzas = new QPushButton(this);
    PrzyciskCzas->setGeometry(120,60,200,80);
    CzasPrzelotuISS = new Zegar(PrzyciskCzas);

    CzasPrzelotuISS->ZmienStrefeCzasowa(+1);

    QObject::connect(CzasDoPobrania, SIGNAL(timeout()), this, SLOT(PobierzNoweDaneISS()));
    QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
    QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));

    emit noweDaneISS(DaneStacji);
    CzasDoPobrania->start(2000);

    std::cout<<"Szerokosc Geograficzna: "<<DaneStacji.SzerokoscGeo<<std::endl;
    std::cout<<"Dlugosc Geograficzna: "<<DaneStacji.DlugoscGeo<<std::endl;
    std::cout<<"Wysokosc: "<<DaneStacji.Wysokosc<<std::endl;
    std::cout<<"Predkosc: "<<DaneStacji.Predkosc<<std::endl;
    std::cout<<"Czas przelotu: "<<DaneStacji.CzasPrzelotu<<std::endl;
    //QObject::connect(Html, SIGNAL(pobrano()), this, SLOT (Dane()));

    TytulISS = new QLabel(tr("<center><font size=6 color=white>Miedzynarodowa stacja kosmiczna</font></center>"), this);
    TytulISS->setGeometry(0,0,RozmiarOkna.width(),60);

    PrzyciskPredkosc = new QPushButton(this);
    PrzyciskPredkosc->setGeometry(20, 300, 360,250);
    WykresPredkosci = new Wykres(PrzyciskPredkosc);
    WykresPredkosci->resize(360,250);

    WykresPredkosci->ZmienWykres(QRgb(0xffffff),1);
    WykresPredkosci->ZmienIloscPodzialekOsiY(3);
    WykresPredkosci->ZmienKolorTla(QRgb(0x000000));
    WykresPredkosci->ZmienKolorOsiX(QRgb(0xffffff));
    WykresPredkosci->ZmienKolorOsiY(QRgb(0xffffff));
    WykresPredkosci->ZmienPodpisOsiX(tr("Czas"));
    WykresPredkosci->ZmienPodpisOsiY(tr("Predkosc [km/h]"));

    PrzyciskWysokosc = new QPushButton(this);
    PrzyciskWysokosc->setGeometry(20, 550, 360,250);
    WykresWysokosci = new Wykres(PrzyciskWysokosc);
    WykresWysokosci->resize(360,250);

    WykresWysokosci->ZmienWykres(QRgb(0xffffff),1);
    WykresWysokosci->ZmienIloscPodzialekOsiY(3);
    WykresWysokosci->ZmienKolorTla(QRgb(0x000000));
    WykresWysokosci->ZmienKolorOsiX(QRgb(0xffffff));
    WykresWysokosci->ZmienKolorOsiY(QRgb(0xffffff));
    WykresWysokosci->ZmienPodpisOsiX(tr("Czas"));
    WykresWysokosci->ZmienPodpisOsiY(tr("Wysokosc [km]"));


    ObecnyZakresOsiCzasu = DaneStacji.CzasPrzelotu;
    ObecnyZakresOsiPredkosci = DaneStacji.Predkosc;
    ObecnyZakresOsiWysokosci = DaneStacji.Wysokosc;

    this->setStyleSheet("background-color:black;");

    Model3D = new OpenGLWidget(this);
    Model3D->setGeometry(400, 60, RozmiarOkna.width()-400,RozmiarOkna.height()-60);
    Model3D->setMouseTracking(true);
}

MainWindow::~MainWindow()
{

}

void MainWindow::PobierzNoweDaneISS()
{
    DaneStacji = Html->PobierzDaneOISS();
    //DodajElementDoDanych(DaneStacji);

    if(ObecnyZakresOsiPredkosci+5 <= DaneStacji.Predkosc)
    {
        ObecnyZakresOsiPredkosci = DaneStacji.Predkosc;
        WykresPredkosci->UstawZakresOsiY(DaneStacji.Predkosc-5, DaneStacji.Predkosc+5);
    }

    if(ObecnyZakresOsiWysokosci+5 <= DaneStacji.Wysokosc)
    {
        ObecnyZakresOsiWysokosci = DaneStacji.Wysokosc;
        WykresWysokosci->UstawZakresOsiY(DaneStacji.Wysokosc-5, DaneStacji.Wysokosc+5);
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
