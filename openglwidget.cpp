#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* Rodzic)
    : QOpenGLWidget (Rodzic)
{
    QSurfaceFormat Format;
    Format.setVersion(4,3);
    Format.setProfile(QSurfaceFormat::CoreProfile);
    Format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    Format.setSwapInterval(1);
    Format.setDepthBufferSize(24);
    Format.setStencilBufferSize(8);
    Format.setRenderableType(QSurfaceFormat::OpenGL);

    QSurfaceFormat::setDefaultFormat(Format);

    Scena = new Grafika3D;

    SzerokoscOkna = size().width();
    WysokoscOkna = size().height();

    KameraOdObiektu = 125.0f;
    KameraX = 125.0f;
    KameraY = 0.0f;
    KameraZ = 0.0f;

    ObrotX = 0; ObrotY = M_PI_2;
    PosMyszyX = 0; PosMyszyY = 0;
    KatSzer = 0; KatDlug = 0;

    fSledzenieStacji = false;
    OffsetTrajektori = 0;

    PromienKuli = 50.0f;
}

OpenGLWidget::~OpenGLWidget()
{
    Scena->ZwrocAdresShadera()->Sprzatanie();
    Scena->CzyscPoGrafice3D();
}

void OpenGLWidget::initializeGL()
{
    glewExperimental=GL_TRUE;
    GLenum err = glewInit();

    if (GLEW_OK != err)
        cerr<<"Error: "<< glewGetErrorString(err)<<endl;

    UstawVertexShader("Vert.glsl");
    UstawFragmentShader("Frag.glsl");
    LinkujShader();

    Scena->ZwrocAdresShadera()->Uzyjprogramu();

    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Scena->UstawRzutowaniePerspektywiczne(45.0f, (GLfloat)(SzerokoscOkna)/(GLfloat)(WysokoscOkna*4),  0.1f, 160.0f);
    Scena->UstawKamere(glm::vec3(KameraOdObiektu, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    TworzZiemie(PromienKuli);
    TworzPunktPozycji();
    TworzTrajektorie();
    Scena->RysujObiekt(IDTrajektoria, false);
    TworzSiatke();
    Scena->RysujObiekt(IDSiatki, false);

    //Scena->UstawRzutowanieOrtogonalne(-100.0f, 100.0,-100.0f, 100.0, 0.1f, 160.0f);
}

void OpenGLWidget::paintGL()
{
    Scena->UstawKamere(glm::vec3(KameraX, KameraY, KameraZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    Scena->Rysuj(SzerokoscOkna,WysokoscOkna);

}
void OpenGLWidget::resizeGL(int Szerokosc, int Wysokosc)
{
    SzerokoscOkna = Szerokosc;
    WysokoscOkna = Wysokosc;
    glViewport(0,0,Szerokosc,Wysokosc);
    Scena->Rysuj(Szerokosc,Wysokosc);
}


bool OpenGLWidget::event(QEvent* Zdarzenie)
{

    QMouseEvent *Mysz;
    QWheelEvent *Scroll;
    switch (Zdarzenie->type())
    {
        case QEvent::MouseButtonPress:
            Mysz = static_cast<QMouseEvent* >(Zdarzenie);

            if(Mysz->button() == Qt::LeftButton)
            {
                PosMyszyX = Mysz->pos().x();
                PosMyszyY = Mysz->pos().y();
            }
        break;

        case QEvent::MouseMove:
            Mysz = static_cast<QMouseEvent* >(Zdarzenie);

            if((Mysz->buttons() & Qt::LeftButton))
            {
               fSledzenieStacji = false;

               ObrotX += (GLfloat)(PosMyszyX - Mysz->pos().x())*0.01f;
               ObrotY += (GLfloat)(PosMyszyY - Mysz->pos().y())*0.01f;

               if(abs(ObrotX) >= 2*M_PI) ObrotX = 0;
               if(abs(ObrotY) >= 2*M_PI) ObrotY = 0;

               KameraX = KameraOdObiektu*cos(ObrotX)*sin(ObrotY);
               KameraY = KameraOdObiektu*sin(ObrotX);
               KameraZ = KameraOdObiektu*cos(ObrotY);

               PosMyszyX = Mysz->pos().x();
               PosMyszyY = Mysz->pos().y();
               update();

            }

        break;

        case QEvent::Wheel:
            Scroll = static_cast<QWheelEvent* >(Zdarzenie);


            KameraOdObiektu += (GLfloat)Scroll->delta()/-30.0f;

            if(KameraOdObiektu <= 55) KameraOdObiektu = 55;
            if(KameraOdObiektu >= 130) KameraOdObiektu = 130;

            if(fSledzenieStacji == false)
            {
                KameraX = KameraOdObiektu*cos(ObrotX)*sin(ObrotY);
                KameraY = KameraOdObiektu*sin(ObrotX);
                KameraZ = KameraOdObiektu*cos(ObrotY);

            }else
            {
                KameraX = KameraOdObiektu*cos(KatDlug)*cos(KatSzer);
                KameraY = KameraOdObiektu*cos(KatSzer)*sin(KatDlug);
                KameraZ = KameraOdObiektu*sin(KatSzer);
            }


            update();

        break;
        default:
        break;

    }


    return QOpenGLWidget::event(Zdarzenie);
}

void OpenGLWidget::NoweDaneISS(ISS_Dane NoweDane)
{
    //Szerokosc - 7.0f (porawka)
    GLfloat Szer = glm::radians(NoweDane.SzerokoscGeo);
    GLfloat Dlug = glm::radians(NoweDane.DlugoscGeo);
    GLfloat Wyso = 0;
    //Zerowanie pozycji
    Scena->PrzesunObiekt(IDWskaznikStacji, glm::vec3(0.0f, 0.0f , PromienKuli));
    Scena->ObrocObiekt(IDWskaznikStacji, -KatSzer , glm::vec3(1.0f, 0.0f, 0.0f));
    Scena->ObrocObiekt(IDWskaznikStacji, -KatDlug, glm::vec3(0.0f, 1.0f, 0.0f));


    //Nowa pozycja
    Scena->ObrocObiekt(IDWskaznikStacji, Dlug, glm::vec3(0.0f, 1.0f, 0.0f));
    Scena->ObrocObiekt(IDWskaznikStacji, Szer , glm::vec3(1.0f, 0.0f, 0.0f));
    Scena->PrzesunObiekt(IDWskaznikStacji, glm::vec3(0.0f, 0.0f , -PromienKuli));

    KatDlug = Dlug;
    KatSzer = Szer;

    if(fSledzenieStacji == true)
    {
        KameraX = KameraOdObiektu*cos(Dlug)*cos(Szer);
        KameraY = KameraOdObiektu*cos(Szer)*sin(Dlug);
        KameraZ = KameraOdObiektu*sin(Szer);
    }


    if(Dlug != 0 && Szer != 0)
    {
        vector<GLfloat> Punkt(3);
        //6 371 prom->50 400->x prom/400 = 50/x 400*50/prom =x
        Wyso = (NoweDane.Wysokosc*PromienKuli)/6371.0f;
        Punkt[0] = (Wyso+PromienKuli)*cos(Dlug)*cos(Szer);
        Punkt[1] = (Wyso+PromienKuli)*cos(Szer)*sin(Dlug);
        Punkt[2] = (Wyso+PromienKuli)*sin(Szer);

        Scena->UaktualniDaneObiektu(IDTrajektoria, (5400+OffsetTrajektori)*3*sizeof(GLfloat), Punkt.size()*sizeof(GLfloat), &Punkt[0]);
        OffsetTrajektori += 1;
        OffsetTrajektori = OffsetTrajektori%10;
    }else
    {
        OffsetTrajektori = 0;
    }




    update();

}

int8_t OpenGLWidget::UstawVertexShader(QString NazwaShadera)
{
    QString Sciezka = QCoreApplication::applicationDirPath() + "/" + NazwaShadera;
    Scena->ZwrocAdresShadera()->Init();

    if(!Scena->ZwrocAdresShadera()->WczytajShader(Sciezka.toStdString().c_str(), GL_VERTEX_SHADER))
        return -1;

    return 0;
}

int8_t OpenGLWidget::UstawFragmentShader(QString NazwaShadera)
{
    QString Sciezka = QCoreApplication::applicationDirPath() + "/" + NazwaShadera;
    Scena->ZwrocAdresShadera()->Init();

    if(!Scena->ZwrocAdresShadera()->WczytajShader(Sciezka.toStdString().c_str(), GL_FRAGMENT_SHADER))
        return -1;

    return 0;
}

int8_t OpenGLWidget::LinkujShader()
{
    if(!Scena->ZwrocAdresShadera()->LinkowanieShader())
        return  -1;

    return 0;
}

void OpenGLWidget::TworzZiemie(GLfloat Promien)
{
    GLuint IDKuli;
    GLuint IDTekstury;
    QString PlikZMapa = QCoreApplication::applicationDirPath() + "/" + "Ziemia3000x1500.png";

    IDTekstury = Scena->GenerujTeksture2D(PlikZMapa.toStdString().c_str(), GL_REPEAT, GL_LINEAR); //GL_CLAMP_TO_BORDER

    IDKuli = Scena->DodajObiekt(Ziemia, sizeof(Ziemia), 3, 0, 2, (50+1)*50*2, Graf3D_TasmaCzworokatow);
    Scena->DodajTekstureDoObiektu(IDTekstury, IDKuli);

    Scena->PrzeskalujObiekt(IDKuli, glm::vec3(Promien, Promien, Promien));
    Scena->ObrocObiekt(IDKuli, M_PI, glm::vec3(0.0f, 0.0f, 1.0f));

}

void OpenGLWidget::TworzPunktPozycji()
{

    IDWskaznikStacji = Scena->DodajObiekt(Okrag, sizeof(Okrag), 3, 4, 0, 51, Graf3D_PolaczonaKrawedz);
    glLineWidth(5);

    //Scena->PrzeskalujObiekt(IDPunktuPozycji, glm::vec3(30.0f, 30.0f, 30.0f));
    Scena->ObrocObiekt(IDWskaznikStacji, -M_PI_2, glm::vec3(0.0f, 0.0f, 1.0f));
    Scena->PrzesunObiekt(IDWskaznikStacji, glm::vec3(0.0f, PromienKuli, 0.0f));
    Scena->ObrocObiekt(IDWskaznikStacji, M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));


}

void OpenGLWidget::TworzTrajektorie()
{
    vector<GLfloat> Trajektoria(16230);

    IDTrajektoria = Scena->DodajObiektD(&Trajektoria[0], Trajektoria.size()*sizeof(GLfloat), 3, 0 ,0, 5410, Graf3D_PolaczonaKrawedz);

}

void OpenGLWidget::WypelniTrajektorie(Magazyn_danych Dane)
{
    vector<GLfloat> Trajektoria(16230);
    ISS_Dane NoweDane;
    GLfloat Szer = 0;
    GLfloat Dlug = 0;
    GLfloat Wyso = 0;

    for(size_t i = 0; i < Dane.ZwrocIloscDanych(); i++)
    {
        NoweDane = Dane.ZwrocDane(i);
        Wyso = (NoweDane.Wysokosc*PromienKuli)/6371.0f;
        Szer = glm::radians(NoweDane.SzerokoscGeo);
        Dlug = glm::radians(NoweDane.DlugoscGeo);

        Trajektoria[(i*3)] = (Wyso+PromienKuli)*cos(Dlug)*cos(Szer);
        Trajektoria[(i*3)+1] = (Wyso+PromienKuli)*cos(Szer)*sin(Dlug);
        Trajektoria[(i*3)+2] = (Wyso+PromienKuli)*sin(Szer);

    }
    IDTrajektoria = Scena->DodajObiektD(&Trajektoria[0], Trajektoria.size()*sizeof(GLfloat), 3, 0 ,0, 5410, Graf3D_PolaczonaKrawedz);

}

void OpenGLWidget::TworzSiatke()
{
    IDSiatki = Scena->DodajObiekt(ModelSiatki, sizeof(ModelSiatki), 3, 0, 0, 51*24*3, Graf3D_PolaczonaKrawedz);
    Scena->PrzeskalujObiekt(IDSiatki, glm::vec3(PromienKuli+0.1f, PromienKuli+0.1f, PromienKuli+0.1f));
    Scena->ObrocObiekt(IDSiatki, -M_PI/24.0f, glm::vec3(0.0f, 0.0f, 1.0f));
}

void OpenGLWidget::UstawKamereNaStrefie(int Strefa)
{
    GLfloat Dlugosc = (GLfloat)(glm::pi<double>()/12.0f * Strefa);

    KameraX = KameraOdObiektu*cos(Dlugosc);
    KameraY = KameraOdObiektu*sin(Dlugosc);
    KameraZ = 0;

    ObrotX = Dlugosc;
    ObrotY = M_PI_2;

    Scena->PrzekazIntaDoShedera("NumerStrefy", (Strefa+12)%24);
}
