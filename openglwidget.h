#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

public:
    OpenGLWidget(QWidget* Rodzic = nullptr);
    ~OpenGLWidget();

protected:
    void initializeGL();
    void resizeGL(int Szerokosc, int Wysokosc);
    void paintGL();
};

#endif // OPENGLWIDGET_H
