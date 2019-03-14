#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* Rodzic)
    : QOpenGLWidget (Rodzic)
{
    ;
}

OpenGLWidget::~OpenGLWidget()
{
    ;
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLWidget::resizeGL(int Szerokosc, int Wysokosc)
{
    glViewport(0,0,Szerokosc,Wysokosc);
}
