#include "mainwindow.h"

//QUrl str("http://wsn.spaceflight.esa.int/iss/index_portal.php");
//QUrl str("https://isstracker.pl/?lang=pl");
//QUrl str("http://www.satview.org/?sat_id=37820U");
//QUrl str("https://api.wheretheiss.at/v1/satellites/25544");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //setGeometry(100, 30, 1024,750);
    move(100,50);
    QSize RozmiarOkna = size();
    QSize RozmiarWidgetu;
    CzasDoPobrania = new QTimer;
    Strefa = 1;

    Html = new Http();

    CzasPrzelotuISS = new Zegar(this);
    //CzasPrzelotuISS->setGeometry(20,60,350,100);
    CzasPrzelotuISS->ZmienStrefeCzasowa(+1);

    CzasPrzelotuISS->setMaximumWidth(350);
    CzasPrzelotuISS->setMinimumWidth(300);

    WyswietlaczPolozeniaISS = new Lokalizator(this);
    //WyswietlaczPolozeniaISS->setGeometry(20,160,350,100);

    WyswietlaczPolozeniaISS->setMaximumWidth(350);
    WyswietlaczPolozeniaISS->setMinimumWidth(300);

    TytulISS = new QLabel(tr("<center><font size=6 color=white>Miedzynarodowa stacja kosmiczna</font></center>"), this);
    TytulISS->setGeometry(0,0,RozmiarOkna.width(),60);

    WykresPredkosci = new Wykres(this);
    //WykresPredkosci->setGeometry(20, 260, 360,250);

    WykresPredkosci->ZmienWykres(QRgb(0xffffff),1);
    WykresPredkosci->ZmienIloscPodzialekOsiY(3);
    WykresPredkosci->ZmienKolorTla(QRgb(0x000000));
    WykresPredkosci->ZmienKolorOsiX(QRgb(0xffffff));
    WykresPredkosci->ZmienKolorOsiY(QRgb(0xffffff));
    WykresPredkosci->ZmienPodpisOsiX(tr("Czas"));
    WykresPredkosci->ZmienPodpisOsiY(tr("Szybkość [km/h]"));

    WykresPredkosci->setMaximumWidth(360);
    WykresPredkosci->setMaximumHeight(240);
    WykresPredkosci->setMinimumWidth(300);
    WykresPredkosci->setMinimumHeight(230);

    WykresWysokosci = new Wykres(this);
    //WykresWysokosci->setGeometry(20, 490, 360,250);

    WykresWysokosci->ZmienWykres(QRgb(0xffffff),1);
    WykresWysokosci->ZmienIloscPodzialekOsiY(3);
    WykresWysokosci->ZmienKolorTla(QRgb(0x000000));
    WykresWysokosci->ZmienKolorOsiX(QRgb(0xffffff));
    WykresWysokosci->ZmienKolorOsiY(QRgb(0xffffff));
    WykresWysokosci->ZmienPodpisOsiX(tr("Czas"));
    WykresWysokosci->ZmienPodpisOsiY(tr("Wysokość [km]"));

    WykresWysokosci->setMaximumWidth(360);
    WykresWysokosci->setMaximumHeight(240);
    WykresWysokosci->setMinimumWidth(300);
    WykresWysokosci->setMinimumHeight(220);

    StrefaCzasowa = new QSlider(Qt::Horizontal, this);
    //StrefaCzasowa->setGeometry(550, 70, 300, 30);
    StrefaCzasowa->setMinimum(-12);
    StrefaCzasowa->setMaximum(14);
    StrefaCzasowa->setValue(1);
    StrefaCzasowa->hide();

    StrefaCzasowa->setMaximumHeight(30);
    //StrefaCzasowa->setMaximumWidth(300);

    WybranaStrefa = new QLabel(tr("<center><font color=white>Strefa: UTC+1</font></center>"), this);
    //WybranaStrefa->setGeometry(550,100,300,30);
    WybranaStrefa->hide();

    WybranaStrefa->setMaximumHeight(30);
    //WybranaStrefa->setMaximumWidth(300);

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
    //Model3D->setGeometry(400, 60, RozmiarOkna.width()-400,RozmiarOkna.height()-60);
    Model3D->SledzenieStacji(true);

    RozmiarWidgetu.setWidth(500);
    RozmiarWidgetu.setHeight(500);
    Model3D->setMinimumSize(RozmiarWidgetu);
    Model3D->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding));

    QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), Model3D, SLOT(NoweDaneISS(ISS_Dane)));

    QObject::connect(WyswietlaczPolozeniaISS, SIGNAL(Kliknieto()), this, SLOT(PrzycisnietyWspolrzedne()));
    QObject::connect(CzasPrzelotuISS, SIGNAL(Kliknieto()), this, SLOT(PrzycisnietyCzas()));
    QObject::connect(WykresWysokosci, SIGNAL(Kliknieto()), this, SLOT(PrzycisnietyWysokosc()));
    QObject::connect(StrefaCzasowa, SIGNAL(valueChanged(int)), this, SLOT(ZmianaStrefy(int)));

    QSpacerItem* Przestrzen = new QSpacerItem(150,0, QSizePolicy::Maximum, QSizePolicy::Maximum);
    QSpacerItem* Przestrzen1 = new QSpacerItem(0,30,QSizePolicy::Maximum, QSizePolicy::Minimum);

    WarstwaWyswietlaczy = new QVBoxLayout;
    WarstwaOpenGL = new QVBoxLayout;
    WarstwaPosrednia = new QHBoxLayout;
    WarstwaTytulu = new QHBoxLayout;
    WarstwaGlowna = new QVBoxLayout;

    WarstwaWyswietlaczy->addWidget(CzasPrzelotuISS);
    WarstwaWyswietlaczy->addWidget(WyswietlaczPolozeniaISS);
    WarstwaWyswietlaczy->addWidget(WykresWysokosci);
    WarstwaWyswietlaczy->addWidget(WykresPredkosci);
    WarstwaWyswietlaczy->setAlignment(Qt::AlignLeft);


    WarstwaOpenGL->addItem(Przestrzen1);
    WarstwaOpenGL->addWidget(StrefaCzasowa);
    WarstwaOpenGL->addWidget(WybranaStrefa);
    WarstwaOpenGL->addWidget(Model3D);

    Tymczasowy = new QSpinBox(this);
    Tymczasowy->setMinimumHeight(10);
    Tymczasowy->setMaximumHeight(10);
    Tymczasowy->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    WarstwaOpenGL->addWidget(Tymczasowy);
    Tymczasowy->hide();

    WarstwaTytulu->addWidget(TytulISS);

    WarstwaPosrednia->addLayout(WarstwaWyswietlaczy);
    WarstwaPosrednia->addLayout(WarstwaOpenGL);
    WarstwaPosrednia->addItem(Przestrzen);

    WarstwaGlowna->addLayout(WarstwaTytulu);
    WarstwaGlowna->addLayout(WarstwaPosrednia);



    QWidget* Centralny = new QWidget(this);
    setCentralWidget(Centralny);
    Centralny->setLayout(WarstwaGlowna);

    ISS_Dane Dane[10];
    Html->PobierzDaneOISS(&Dane[0], 10, 1436029892, 15);

    for(int i = 0; i < 10; i++)cout<<Dane[i].CzasPrzelotu<<endl;

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
        if(ObecnyZakresOsiPredkosci+5 <= DaneStacji.Predkosc || ObecnyZakresOsiPredkosci-5 >= DaneStacji.Predkosc)
        {
            ObecnyZakresOsiPredkosci = DaneStacji.Predkosc;
            WykresPredkosci->UstawZakresOsiY(DaneStacji.Predkosc-5, DaneStacji.Predkosc+5);
        }

        if(ObecnyZakresOsiWysokosci+2 <= DaneStacji.Wysokosc || ObecnyZakresOsiWysokosci-2 >= DaneStacji.Wysokosc)
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
    Model3D->RysowanieSiatki(false);
    StrefaCzasowa->show();
    WybranaStrefa->show();
    Model3D->PodswietlenieStrefy(1);
    Model3D->UstawKamereNaStrefie(Strefa);
    //Model3D->setGeometry(400, 130, size().width()-400, size().height()-130);
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

    StrefaCzasowa->show();
    WybranaStrefa->show();
    StrefaCzasowa->hide();
    WybranaStrefa->hide();

    Model3D->PodswietlenieStrefy(0);
    //Model3D->setGeometry(400, 60, size().width()-400,size().height()-60);
    Model3D->update();
    update();

}

void MainWindow::PrzycisnietyWysokosc()
{
    Model3D->RysowanieTrajektorii(true);
    Model3D->RysowaniePunktuSledzacego(true);
    Model3D->RysowanieSiatki(false);

    StrefaCzasowa->hide();
    WybranaStrefa->hide();

    StrefaCzasowa->show();
    WybranaStrefa->show();
    StrefaCzasowa->hide();
    WybranaStrefa->hide();

    Model3D->PodswietlenieStrefy(0);
    //Model3D->setGeometry(400, 60, size().width()-400,size().height()-60);
    Model3D->update();
}

void MainWindow::ZmianaStrefy(int NowaStrefa)
{
    Strefa = NowaStrefa;

    if(NowaStrefa >= 0)
        WybranaStrefa->setText(tr("<center><font color=white>Strefa: UTC+%1</font></center>").arg(NowaStrefa));
    else
        WybranaStrefa->setText(tr("<center><font color=white>Strefa: UTC-%1</font></center>").arg(-NowaStrefa));

    CzasPrzelotuISS->ZmienStrefeCzasowa(NowaStrefa);
    CzasPrzelotuISS->update();

    StrefaCzasowa->show();
    WybranaStrefa->show();

    Tymczasowy->show();
    Tymczasowy->hide();

    Model3D->UstawKamereNaStrefie(NowaStrefa);
    Model3D->update();

}
