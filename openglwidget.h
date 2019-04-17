#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

/*!
 * \file
 * \brief Definicja klasy OpenGLWidget
 *
*/

#include "KGrafika3D.h"
#include "modele.h"
#include "iss_dane.h"
#include "magazyn_danych.h"

#include <QWidget>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QCoreApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>

/*!
 * \brief Klasa pozwalająca wyświetlać model 3D Ziemi oraz inne elementy grafiki 3D.
 *
 *  Klasa pozwala renderować model 3D Ziemi, jest odpowiedzialna za obsługę myszki pozwalającej obracać model Ziemi.
 *  Pozwala równierz na tworzenie oraz wyświeltanie elementów takich jak wskaźniki obecnej pozycji stacji oraz jej trajektoria.
*/

class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget* Rodzic = nullptr);
    ~OpenGLWidget();

    int8_t UstawVertexShader(QString NazwaShadera);
    int8_t UstawFragmentShader(QString NazwaShadera);
    int8_t LinkujShader();

    /*!
     * \brief Tworzenie modelu Ziemi i dodawanie jej do sceny.
     * \param[in] Promien - promien Ziemi.
     */
    void TworzZiemie(GLfloat Promien);

    /*!
     * \brief Tworzy punkt wskazujący miejsce, nad którym znajduje się stacja kosmiczna
     */
    void TworzPunktPozycji();

    /*!
     * \brief Tworzy pustą trajektorie, którą można później wypełnić.
     */
    void TworzTrajektorie();

    /*!
     * \brief Tworzy wskaźnik pozwalający wskazać, którego punktu trajektorii dotyczą obecnie wyświetlane dane.
     */
    void TworzWskaznikTrajektorii();

    /*!
     * \brief Pozwala wypełnić wcześniej stworzoną trajektorie danymi.
     * \param[in] Dane - magazyn danych, z którego ma zostać utworzona trajektoria.
     */
    void WypelniTrajektorie(Magazyn_danych Dane);

    /*!
     * \brief Tworz siatkę nad Ziemią pokazującą strefy czasowe.
     */
    void TworzSiatke();

    /*!
     * \brief Pozwala włączyć śledzenie wskaźnika stacji przez kamerę.
     * \param[in] Sledz - jeżeli parametr przyjmnie wartość true śledzenie zostanie właczone, false wyłączy śledzenie.
     */
    void SledzenieStacji(GLboolean Sledz){fSledzenieStacji = Sledz;}

    /*!
     * \brief Metoda pozwala włączyć rysowanie trajektorii stacji co spowoduje, że stanie się widoczna.
     * \param[in] Rysuj - jeżeli parametr przyjmnie wartość true obiekt zostanie narysowany, false spowoduje, że obiek nie będzie narysowany.
     */
    void RysowanieTrajektorii(GLboolean Rysuj){Scena->RysujObiekt(IDTrajektoria, Rysuj);}

    /*!
     * \brief Metoda pozwala włączyć rysowanie siatki nad modelem Ziemi co spowoduje, że stanie się widoczna.
     * \param[in] Rysuj - jeżeli parametr przyjmnie wartość true obiekt zostanie narysowany, false spowoduje, że obiek nie będzie narysowany.
     */
    void RysowanieSiatki(GLboolean Rysuj){Scena->RysujObiekt(IDSiatki, Rysuj);}

    /*!
     * \brief Metoda pozwala włączyć rysowanie punktu śledzącego pozycje stacji co spowoduje, że stanie się widoczny.
     * \param[in] Rysuj - jeżeli parametr przyjmnie wartość true obiekt zostanie narysowany, false spowoduje, że obiek nie będzie narysowany.
     */
    void RysowaniePunktuSledzacego(GLboolean Rysuj){Scena->RysujObiekt(IDWskaznikStacji, Rysuj);}

    /*!
     * \brief Metoda pozwala włączyć rysowanie wskaźnika trajektorii co spowoduje, że stanie się widoczny.
     * \param[in] Rysuj - jeżeli parametr przyjmnie wartość true obiekt zostanie narysowany, false spowoduje, że obiek nie będzie narysowany.
     */
    void RysowanieWskTrajektorii(GLboolean Rysuj){Scena->RysujObiekt(IDWskTrajektorii, Rysuj);}

    /*!
     * \brief Pozwala ustawić kamerę na podaną strefę czasową.
     * \param[in] Strefa - numer strefy czasowej UTC.
     */
    void UstawKamereNaStrefie(int Strefa);

    /*!
     * \brief Metoda pozwalająca włączyć lub wyłączyc podświetlanie strefy czasowej.
     * \param[in] Podswietl - ustawiony na 0 powoduje, że strefa nie będzie podświetlona, natomiast na 1 powoduje podświetlenie strefy.
     */
    void PodswietlenieStrefy(int Podswietl){Scena->PrzekazIntaDoShedera("WlaczStrefe", Podswietl);}

protected:
    void initializeGL();
    void resizeGL(int Szerokosc, int Wysokosc);
    void paintGL();
    bool event(QEvent* Zdarzenie);

private slots:
    void NoweDaneISS(ISS_Dane NoweDane);
    void TrajektoriaNoweDane(ISS_Dane NoweDane);


private:

    /*!
     * \brief Scena - wskaźnik na obiekt kalsy Grafika3D pozwalający na dokonywaniu operacji na grafice 3D, w tym dodawaniu nowych obiektów do sceny.
     */
    Grafika3D* Scena;

    /*!
     * \brief Szerokość okna (widżetu), w którym wyświetlany jest model.
     */
    int SzerokoscOkna;

    /*!
     * \brief Wysokość okna (widżetu), w którym wyświetlany jest model.
     */
    int WysokoscOkna;

    /*!
     * \brief KameraOdObiektu - odległość kamery od środka modelu Ziemi.
     */
    GLfloat KameraOdObiektu;

    /*!
     * \brief KameraX - wpółrzędna X położenia kamery w przestrzeni 3D.
     */
    GLfloat KameraX;

    /*!
     * \brief KameraY - wpółrzędna Y położenia kamery w przestrzeni 3D.
     */
    GLfloat KameraY;

    /*!
     * \brief KameraZ - wpółrzędna Z położenia kamery w przestrzeni 3D.
     */
    GLfloat KameraZ;

    /*!
     * \brief ObrotX - kąt o jaki obrócony jest model Ziemi względem osi Y.
     */
    GLfloat ObrotX;

    /*!
     * \brief ObrotY - kąt o jaki obrócony jest model Ziemi względem osi X.
     */
    GLfloat ObrotY;

    /*!
     * \brief PosMyszyX - współrzędna X ostatniej zapamiętanej pozycji myszki po kliknięciu lewego przycisku myszki.
     */
    int PosMyszyX;

    /*!
     * \brief PosMyszyY - współrzędna Y ostatniej zapamiętanej pozycji myszki po kliknięciu lewego przycisku myszki.
     */
    int PosMyszyY;

    /*!
     * \brief PromienKuli - Promień modelu Ziemi.
     */
    GLfloat PromienKuli;

    /*!
     * \brief KatSzer - szerokość geograficzna ostatniego punktu, nad którym znajdowała się stacja.
     */
    GLfloat KatSzer;

    /*!
     * \brief KatDlug - długość geograficzna ostatniego punktu, nad którym znajdowała się stacja.
     */
    GLfloat KatDlug;

    /*!
     * \brief IDTrajektoria - identyfikator trajektorii uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
     */
    GLuint IDTrajektoria;

    /*!
     * \brief OffsetTrajektori - przesunięcie na pozycję pozwalającą zapisywać dane do ostatniej części trajektorii tak aby aktualizować jej początek.
     */
    GLuint OffsetTrajektori;

    /*!
     * \brief IDSiatki - identyfikator siatki uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
     */
    GLuint IDSiatki;

    /*!
     * \brief IDWskTrajektorii - identyfikator wskaźnika trajektorii uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
     */
    GLuint IDWskTrajektorii;

    /*!
     * \brief IDWskaznikStacji - identyfikator wskaźnika położenia stacji uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
     */
    GLuint IDWskaznikStacji;

    /*!
     * \brief fSledzenieStacji - pole używane jako flaga, ustawione powoduje śledznie pozycji stacji przez kamere.
     */
    GLuint fSledzenieStacji;

};

#endif // OPENGLWIDGET_H
