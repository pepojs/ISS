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

    AktywnaTrajektoria = false;
    PrzelaczAktualizacjeDanych = false;

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

    MagazynDanychStacji = new Magazyn_danych(24*60);
    long int IleDanych = MagazynDanychStacji->WypelniDanymiZPliku(Html, 24*60*60);//+3*60
    cout<<"Ile danych: "<<IleDanych<<endl;
    if(IleDanych >= 0)
        MagazynDanychStacji->WypelniDanymiZSieci(Html, 60, IleDanych, 24*60*60);//+3*60
    else
        MagazynDanychStacji->WypelniDanymiZSieci(Html, 60, 0, 24*60*60);//+3*60

    MagazynDanychStacji->ZapiszDane(60);

    for(size_t k = 0; k < MagazynDanychStacji->ZwrocIloscDanych(); k++)
    {
        cout<<"K: "<<k<<", Czas: "<<MagazynDanychStacji->ZwrocDane(k).ZwrocCzasPrzelotu_UTS()<<", Wysokosc: "<<MagazynDanychStacji->ZwrocDane(k).ZwrocWysokosc_km()<<endl;
    }

    QObject::connect(CzasDoPobrania, SIGNAL(timeout()), this, SLOT(PobierzNoweDaneISS()));
    QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
    QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));


    emit noweDaneISS(DaneStacji);
    CzasDoPobrania->start(2000);

    ObecnyZakresOsiCzasu = DaneStacji.ZwrocCzasPrzelotu_UTS();
    ObecnyZakresOsiPredkosci = DaneStacji.ZwrocPredkosc_kmH();
    ObecnyZakresOsiWysokosci = DaneStacji.ZwrocWysokosc_km();

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

    Licznik = 0;

}

MainWindow::~MainWindow()
{
    CzasDoPobrania->stop();
    //MagazynDanychStacji->ZapiszDane(60);
}

void MainWindow::PobierzNoweDaneISS()
{
    size_t pom;
    size_t roznica = 0;
    ISS_Dane TabPom[10];

    DaneStacji = Html->PobierzDaneOISS();
    //DodajElementDoDanych(DaneStacji);

    if(DaneStacji.ZwrocWysokosc_km() > 0)
    {
        if(PrzelaczAktualizacjeDanych == false)
        {
            if(ObecnyZakresOsiPredkosci+5 <= DaneStacji.ZwrocPredkosc_kmH() || ObecnyZakresOsiPredkosci-5 >= DaneStacji.ZwrocPredkosc_kmH())
            {
                ObecnyZakresOsiPredkosci = DaneStacji.ZwrocPredkosc_kmH();
                WykresPredkosci->UstawZakresOsiY(DaneStacji.ZwrocPredkosc_kmH()-5, DaneStacji.ZwrocPredkosc_kmH()+5);
            }

            if(ObecnyZakresOsiWysokosci+2 <= DaneStacji.ZwrocWysokosc_km() || ObecnyZakresOsiWysokosci-2 >= DaneStacji.ZwrocWysokosc_km())
            {
                ObecnyZakresOsiWysokosci = DaneStacji.ZwrocWysokosc_km();
                WykresWysokosci->UstawZakresOsiY(DaneStacji.ZwrocWysokosc_km()-2, DaneStacji.ZwrocWysokosc_km()+2);
            }

            if(ObecnyZakresOsiCzasu+30 <= DaneStacji.ZwrocCzasPrzelotu_UTS() + ((Strefa - 1)*60*60) || ObecnyZakresOsiCzasu-30 >= DaneStacji.ZwrocCzasPrzelotu_UTS() + ((Strefa - 1)*60*60))
            {
                ObecnyZakresOsiCzasu = DaneStacji.ZwrocCzasPrzelotu_UTS() + ((Strefa - 1)*60*60);
                WykresPredkosci->UstawZakresOsiX(DaneStacji.ZwrocCzasPrzelotu_UTS() + ((Strefa - 1)*60*60) - 30, DaneStacji.ZwrocCzasPrzelotu_UTS() + ((Strefa - 1)*60*60) + 30);
                WykresWysokosci->UstawZakresOsiX(DaneStacji.ZwrocCzasPrzelotu_UTS() + ((Strefa - 1)*60*60) - 30, DaneStacji.ZwrocCzasPrzelotu_UTS() + ((Strefa - 1)*60*60) + 30);
            }
        }


        WykresPredkosci->DodajDaneDoWykresu(DaneStacji.ZwrocCzasPrzelotu_UTS() + ((Strefa - 1)*60*60), DaneStacji.ZwrocPredkosc_kmH());
        WykresWysokosci->DodajDaneDoWykresu(DaneStacji.ZwrocCzasPrzelotu_UTS() + ((Strefa - 1)*60*60), DaneStacji.ZwrocWysokosc_km());

        emit noweDaneISS(DaneStacji);

        if(Licznik == 0)
            Model3D->WypelniTrajektorie(*MagazynDanychStacji);

        Licznik += 1;

        if(MagazynDanychStacji->ZwrocGlowe() == 0)
        {
            pom = MagazynDanychStacji->ZwrocIloscDanych()-1;

        }else
        {
            pom = MagazynDanychStacji->ZwrocGlowe()-1;
        }

        //cout<<"Roz czas : "<<(DaneStacji.ZwrocCzasPrzelotu_UTS() - MagazynDanychStacji->ZwrocDane(pom).ZwrocCzasPrzelotu_UTS())<<endl;
        //cout<<"MD: "<<MagazynDanychStacji->ZwrocDane(pom).ZwrocCzasPrzelotu_UTS()<<", DS: "<<DaneStacji.ZwrocCzasPrzelotu_UTS()<<endl;

        if((DaneStacji.ZwrocCzasPrzelotu_UTS() - MagazynDanychStacji->ZwrocDane(pom).ZwrocCzasPrzelotu_UTS()) >= 120)
        {
            roznica = (DaneStacji.ZwrocCzasPrzelotu_UTS() - MagazynDanychStacji->ZwrocDane(pom).ZwrocCzasPrzelotu_UTS())/60;

            if(roznica >= 10) roznica = 10;

            Html->PobierzDaneOISS(&TabPom[0], roznica, MagazynDanychStacji->ZwrocDane(pom).ZwrocCzasPrzelotu_UTS(), 60);
            for(size_t j = 0; j < roznica; j++)
            {
                    MagazynDanychStacji->DodajNoweDane(TabPom[j]);
                    cout<<"tabPom: "<<TabPom[j]<<endl;
            }
        }

        //if(Licznik == 30)
        if((DaneStacji.ZwrocCzasPrzelotu_UTS() - MagazynDanychStacji->ZwrocDane(pom).ZwrocCzasPrzelotu_UTS()) >= 60)
        {
            MagazynDanychStacji->DodajNoweDane(DaneStacji);
            Licznik = 0;
        }
    }




}

void MainWindow::PrzycisnietyCzas()
{
    AktywnaTrajektoria = false;
    if(PrzelaczAktualizacjeDanych == true)
    {
        QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
        QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));

        QObject::disconnect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
        QObject::disconnect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));
        QObject::disconnect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), Model3D, SLOT(TrajektoriaNoweDane(ISS_Dane)));
    }

    PrzelaczAktualizacjeDanych = false;

   /* Model3D->hide();
    QObject::disconnect(this, SIGNAL(noweDaneISS(ISS_Dane)), Model3D, SLOT(NoweDaneISS(ISS_Dane)));
    */
    Model3D->RysowaniePunktuSledzacego(false);
    Model3D->RysowanieTrajektorii(false);
    Model3D->SledzenieStacji(false);
    Model3D->RysowanieSiatki(false);
    Model3D->RysowanieWskTrajektorii(false);

    StrefaCzasowa->show();
    WybranaStrefa->show();
    Model3D->PodswietlenieStrefy(1);
    Model3D->UstawKamereNaStrefie(Strefa);
    //Model3D->setGeometry(400, 130, size().width()-400, size().height()-130);
    Model3D->update();
}

void MainWindow::PrzycisnietyWspolrzedne()
{
    AktywnaTrajektoria = false;
    if(PrzelaczAktualizacjeDanych == true)
    {
        QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
        QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));

        QObject::disconnect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
        QObject::disconnect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));
        QObject::disconnect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), Model3D, SLOT(TrajektoriaNoweDane(ISS_Dane)));
    }

    PrzelaczAktualizacjeDanych = false;

    Model3D->SledzenieStacji(true);
    Model3D->RysowaniePunktuSledzacego(true);
    Model3D->RysowanieTrajektorii(false);
    Model3D->RysowanieSiatki(false);
    Model3D->RysowanieWskTrajektorii(false);

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
    AktywnaTrajektoria = true;

    if(PrzelaczAktualizacjeDanych == true)
    {
        QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
        QObject::connect(this, SIGNAL(noweDaneISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));

        QObject::disconnect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
        QObject::disconnect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));
        QObject::disconnect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), Model3D, SLOT(TrajektoriaNoweDane(ISS_Dane)));
    }

    PrzelaczAktualizacjeDanych = false;

    Model3D->RysowanieTrajektorii(true);
    Model3D->RysowaniePunktuSledzacego(true);
    Model3D->RysowanieSiatki(false);
    Model3D->RysowanieWskTrajektorii(false);

    StrefaCzasowa->hide();
    WybranaStrefa->hide();

    StrefaCzasowa->show();
    WybranaStrefa->show();
    StrefaCzasowa->hide();
    WybranaStrefa->hide();

    Model3D->PodswietlenieStrefy(0);
    //Model3D->setGeometry(400, 60, size().width()-400,size().height()-60);
    Model3D->update();

    this->setFocus();
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

    WykresPredkosci->WyczyscWykres();
    WykresWysokosci->WyczyscWykres();

}


bool MainWindow::event(QEvent* Zdarzenie)
{
    QKeyEvent *Przycisk;


    switch(Zdarzenie->type())
    {
        case QEvent::KeyPress:
            Przycisk = static_cast<QKeyEvent* >(Zdarzenie);
            //cout<<Przycisk->key()<<endl;
            if(Przycisk->key() == Qt::Key_Left)
            {
                if(AktywnaTrajektoria == true)
                {
                    if(PrzelaczAktualizacjeDanych == false)
                    {
                        Model3D->SledzenieStacji(false);

                        QObject::disconnect(this, SIGNAL(noweDaneISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
                        QObject::disconnect(this, SIGNAL(noweDaneISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));

                        QObject::connect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
                        QObject::connect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));
                        QObject::connect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), Model3D, SLOT(TrajektoriaNoweDane(ISS_Dane)));

                        Model3D->RysowanieWskTrajektorii(true);
                        MagazynOdczytywaneDane = MagazynDanychStacji->ZwrocGlowe();
                    }

                    PrzelaczAktualizacjeDanych = true;

                    if(MagazynOdczytywaneDane == 0)
                        MagazynOdczytywaneDane = MagazynDanychStacji->ZwrocIloscDanych();

                    MagazynOdczytywaneDane -= 1;

                    ISS_Dane DanePomocnicze = MagazynDanychStacji->ZwrocDane(MagazynOdczytywaneDane);

                    if(ObecnyZakresOsiPredkosci+5 <= DanePomocnicze.ZwrocPredkosc_kmH() || ObecnyZakresOsiPredkosci-5 >= DanePomocnicze.ZwrocPredkosc_kmH())
                    {
                        ObecnyZakresOsiPredkosci = DanePomocnicze.ZwrocPredkosc_kmH();
                        WykresPredkosci->UstawZakresOsiY(DanePomocnicze.ZwrocPredkosc_kmH()-5, DanePomocnicze.ZwrocPredkosc_kmH()+5);
                    }

                    if(ObecnyZakresOsiWysokosci+2 <= DanePomocnicze.ZwrocWysokosc_km() || ObecnyZakresOsiWysokosci-2 >= DanePomocnicze.ZwrocWysokosc_km())
                    {
                        ObecnyZakresOsiWysokosci = DanePomocnicze.ZwrocWysokosc_km();
                        WykresWysokosci->UstawZakresOsiY(DanePomocnicze.ZwrocWysokosc_km()-2, DanePomocnicze.ZwrocWysokosc_km()+2);
                    }

                    if(ObecnyZakresOsiCzasu+30 <= DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60) || ObecnyZakresOsiCzasu-30 >= DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60))
                    {
                        ObecnyZakresOsiCzasu = DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60);
                        WykresPredkosci->UstawZakresOsiX(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60)-30, DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60)+30);
                        WykresWysokosci->UstawZakresOsiX(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60)-30, DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60)+30);
                    }

                    WykresPredkosci->WyczyscWykres();
                    WykresWysokosci->WyczyscWykres();

                    cout<<"Glowa: "<<MagazynDanychStacji->ZwrocGlowe()<<endl;

                    if(MagazynOdczytywaneDane != 0)
                    {
                        if(MagazynDanychStacji->ZwrocGlowe() != MagazynOdczytywaneDane)
                        {
                            DanePomocnicze = MagazynDanychStacji->ZwrocDane(MagazynOdczytywaneDane-1);

                            WykresPredkosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocPredkosc_kmH());
                            WykresWysokosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocWysokosc_km());
                        }

                    }else
                    {
                        if(MagazynDanychStacji->ZwrocGlowe() != 0)
                        {
                            DanePomocnicze = MagazynDanychStacji->ZwrocDane(MagazynDanychStacji->ZwrocIloscDanych()-1);

                            WykresPredkosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocPredkosc_kmH());
                            WykresWysokosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocWysokosc_km());
                        }
                    }

                    DanePomocnicze = MagazynDanychStacji->ZwrocDane(MagazynOdczytywaneDane);

                    WykresPredkosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocPredkosc_kmH());
                    WykresWysokosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocWysokosc_km());

                    if(MagazynOdczytywaneDane != MagazynDanychStacji->ZwrocIloscDanych()-1)
                    {
                        if(MagazynDanychStacji->ZwrocGlowe() != (MagazynOdczytywaneDane+1))
                        {
                            DanePomocnicze = MagazynDanychStacji->ZwrocDane(MagazynOdczytywaneDane+1);

                            WykresPredkosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocPredkosc_kmH());
                            WykresWysokosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocWysokosc_km());
                        }


                    }else
                    {
                        if(MagazynDanychStacji->ZwrocGlowe() != 0)
                        {
                            DanePomocnicze = MagazynDanychStacji->ZwrocDane(0);

                            WykresPredkosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocPredkosc_kmH());
                            WykresWysokosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocWysokosc_km());
                        }

                    }

                    emit daneTrajektoriiISS(MagazynDanychStacji->ZwrocDane(MagazynOdczytywaneDane));
                }
                cout<<"MOD: "<<MagazynOdczytywaneDane<<endl;
            }

            if(Przycisk->key() == Qt::Key_Right)
            {
                if(AktywnaTrajektoria == true)
                {
                    if(PrzelaczAktualizacjeDanych == false)
                    {
                        Model3D->SledzenieStacji(false);

                        QObject::disconnect(this, SIGNAL(noweDaneISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
                        QObject::disconnect(this, SIGNAL(noweDaneISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));

                        QObject::connect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), WyswietlaczPolozeniaISS, SLOT(AktualizujDaneISS(ISS_Dane)));
                        QObject::connect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), CzasPrzelotuISS, SLOT(AktualizujDaneISS(ISS_Dane)));
                        QObject::connect(this, SIGNAL(daneTrajektoriiISS(ISS_Dane)), Model3D, SLOT(TrajektoriaNoweDane(ISS_Dane)));

                        Model3D->RysowanieWskTrajektorii(true);
                        MagazynOdczytywaneDane = MagazynDanychStacji->ZwrocGlowe();
                    }

                    PrzelaczAktualizacjeDanych = true;

                    MagazynOdczytywaneDane += 1;

                    MagazynOdczytywaneDane = MagazynOdczytywaneDane%MagazynDanychStacji->ZwrocIloscDanych();


                    ISS_Dane DanePomocnicze = MagazynDanychStacji->ZwrocDane(MagazynOdczytywaneDane);

                    if(ObecnyZakresOsiPredkosci+5 <= DanePomocnicze.ZwrocPredkosc_kmH() || ObecnyZakresOsiPredkosci-5 >= DanePomocnicze.ZwrocPredkosc_kmH())
                    {
                        ObecnyZakresOsiPredkosci = DanePomocnicze.ZwrocPredkosc_kmH();
                        WykresPredkosci->UstawZakresOsiY(DanePomocnicze.ZwrocPredkosc_kmH()-5, DanePomocnicze.ZwrocPredkosc_kmH()+5);
                    }

                    if(ObecnyZakresOsiWysokosci+2 <= DanePomocnicze.ZwrocWysokosc_km() || ObecnyZakresOsiWysokosci-2 >= DanePomocnicze.ZwrocWysokosc_km())
                    {
                        ObecnyZakresOsiWysokosci = DanePomocnicze.ZwrocWysokosc_km();
                        WykresWysokosci->UstawZakresOsiY(DanePomocnicze.ZwrocWysokosc_km()-2, DanePomocnicze.ZwrocWysokosc_km()+2);
                    }

                    if(ObecnyZakresOsiCzasu+30 <= DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60) || ObecnyZakresOsiCzasu-30 >= DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60))
                    {
                        ObecnyZakresOsiCzasu = DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60);
                        WykresPredkosci->UstawZakresOsiX(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60)-30, DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60)+30);
                        WykresWysokosci->UstawZakresOsiX(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60)-30, DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60)+30);
                    }

                    WykresPredkosci->WyczyscWykres();
                    WykresWysokosci->WyczyscWykres();


                    cout<<"Glowa: "<<MagazynDanychStacji->ZwrocGlowe()<<endl;

                    if(MagazynOdczytywaneDane != 0)
                    {
                        if(MagazynDanychStacji->ZwrocGlowe() != MagazynOdczytywaneDane)
                        {
                            DanePomocnicze = MagazynDanychStacji->ZwrocDane(MagazynOdczytywaneDane-1);

                            WykresPredkosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocPredkosc_kmH());
                            WykresWysokosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocWysokosc_km());
                        }

                    }else
                    {
                        if(MagazynDanychStacji->ZwrocGlowe() != 0)
                        {
                            DanePomocnicze = MagazynDanychStacji->ZwrocDane(MagazynDanychStacji->ZwrocIloscDanych()-1);

                            WykresPredkosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocPredkosc_kmH());
                            WykresWysokosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocWysokosc_km());
                        }
                    }

                    DanePomocnicze = MagazynDanychStacji->ZwrocDane(MagazynOdczytywaneDane);

                    WykresPredkosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocPredkosc_kmH());
                    WykresWysokosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocWysokosc_km());

                    if(MagazynOdczytywaneDane != MagazynDanychStacji->ZwrocIloscDanych()-1)
                    {
                        if(MagazynDanychStacji->ZwrocGlowe() != (MagazynOdczytywaneDane+1))
                        {
                            DanePomocnicze = MagazynDanychStacji->ZwrocDane(MagazynOdczytywaneDane+1);

                            WykresPredkosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocPredkosc_kmH());
                            WykresWysokosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocWysokosc_km());
                        }


                    }else
                    {
                        if(MagazynDanychStacji->ZwrocGlowe() != 0)
                        {
                            DanePomocnicze = MagazynDanychStacji->ZwrocDane(0);

                            WykresPredkosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocPredkosc_kmH());
                            WykresWysokosci->DodajDaneDoWykresu(DanePomocnicze.ZwrocCzasPrzelotu_UTS()+ ((Strefa - 1)*60*60), DanePomocnicze.ZwrocWysokosc_km());
                        }

                    }

                    emit daneTrajektoriiISS(MagazynDanychStacji->ZwrocDane(MagazynOdczytywaneDane));
                }

                cout<<"MOD: "<<MagazynOdczytywaneDane<<endl;
            }
        break;

        default:
        break;
    }

    return QWidget::event(Zdarzenie);
}
