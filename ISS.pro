#-------------------------------------------------
#
# Project created by QtCreator 2019-03-02T14:37:33
#
#-------------------------------------------------

QT       += core gui
QT       += network charts opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ISS
TEMPLATE = app
DEPENDPATH += C:/OpenSSL-Win64
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    http.cpp \
    lokalizator.cpp \
    zegar.cpp \
    wykres.cpp \
    openglwidget.cpp \
    KGrafika3D.cpp \
    Shader.cpp

HEADERS += \
        mainwindow.h \
    http.h \
    iss_dane.h \
    lokalizator.h \
    zegar.h \
    wykres.h \
    openglwidget.h \
    KGrafika3D.h \
    Shader.h \
    SObiektNaScenie.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += "D:\Program Files (x86)\CodeBlocks\MinGW\downloaded\soil\soil\Simple OpenGL Image Library\src" \
"C:\Users\Piotr\Downloads\freetype-windows-binaries-master\freetype-windows-binaries-master\include" \
"D:\Program Files (x86)\CodeBlocks\MinGW\downloaded\glew-2.1.0-win32\glew-2.1.0-win32\glew-2.1.0\include\GL" \
"D:\Program Files (x86)\CodeBlocks\MinGW\downloaded\glm-0.9.9.3\glm\glm"


LIBS += -L"D:\Program Files (x86)\CodeBlocks\MinGW\downloaded\soil\soil\Simple OpenGL Image Library\lib"\
-L"D:\Program Files (x86)\CodeBlocks\MinGW\downloaded\freetype-windows-binaries-master\freetype-windows-binaries-master\win64" -lfreetype

LIBS += -L"D:\Program Files (x86)\CodeBlocks\MinGW\downloaded\glew-2.1.0-win32\glew-2.1.0-win32\glew-2.1.0\lib\Release\Win32\glew32s.lib"\
-L"D:\Program Files (x86)\CodeBlocks\MinGW\downloaded\glew-2.1.0-win32\glew-2.1.0-win32\glew-2.1.0\lib\Release\x64"\
-L"D:\Program Files (x86)\CodeBlocks\MinGW\lib\libglu32.a"\


LIBS += -lglu32 -lSOIL -lglew32 -lopengl32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
