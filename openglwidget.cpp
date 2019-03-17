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

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    TworzZiemie(80);
}

void OpenGLWidget::paintGL()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Scena->Rysuj(300,300);

}
void OpenGLWidget::resizeGL(int Szerokosc, int Wysokosc)
{
    glViewport(0,0,Szerokosc,Wysokosc);
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

    Scena->UstawRzutowanieOrtogonalne(-100.0f, 100.0,-100.0f, 100.0, 0.1f, 150.0f);
    Scena->UstawKamere(glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Scena->PrzeskalujObiekt(IDKuli, glm::vec3(Promien, Promien, Promien));

}
