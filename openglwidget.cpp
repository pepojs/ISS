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

    ObrotX = M_PI_2; ObrotY = M_PI_2;
    PosMyszyX = 0; PosMyszyY = 0;
}

OpenGLWidget::~OpenGLWidget()
{
    delete[] Scena;
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

    //Scena->UstawRzutowanieOrtogonalne(-100.0f, 100.0,-100.0f, 100.0, 0.1f, 150.0f);
    Scena->UstawRzutowaniePerspektywiczne(45.0f, (GLfloat)(SzerokoscOkna)/(GLfloat)(WysokoscOkna*4),  0.1f, 160.0f);
    Scena->UstawKamere(glm::vec3(KameraOdObiektu, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    TworzZiemie(50);
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

            KameraX = KameraOdObiektu*cos(ObrotX)*sin(ObrotY);
            KameraY = KameraOdObiektu*sin(ObrotX);
            KameraZ = KameraOdObiektu*cos(ObrotY);

            update();

        break;
        default:
        break;

    }


    return QOpenGLWidget::event(Zdarzenie);
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

    QString PlikZMapa = QCoreApplication::applicationDirPath() + "/" + "TeksturaZiemi2.jpg";

    IDTekstury = Scena->GenerujTeksture2D(PlikZMapa.toStdString().c_str(), GL_CLAMP_TO_BORDER, GL_LINEAR);

    IDKuli = Scena->DodajObiekt(Ziemia, sizeof(Ziemia), 3, 0, 2, (50+1)*50*2, Graf3D_TasmaCzworokatow);
    Scena->DodajTekstureDoObiektu(IDTekstury, IDKuli);

    Scena->PrzeskalujObiekt(IDKuli, glm::vec3(Promien, Promien, Promien));

}
