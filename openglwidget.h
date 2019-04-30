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
#include <QResource>

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

    /*!
     * \brief Metoda pozwala ustawić, który shader będzie używany jako Vertex Shader.
	 *
	 * Metoda pozwala ustawić, który shader będzie używany jako Vertex Shader.
	 *
     * \param[in] NazwaShadera - nazwa shadera.
     * \return 0 gdy wszystko się udało, -1 gdy wystąpił błąd.
     */
    int8_t UstawVertexShader(QString NazwaShadera);

    /*!
     * \brief Metoda pozwala ustawić, który shader będzie używany jako Fragment Shader.
	 *
	 * Metoda pozwala ustawić, który shader będzie używany jako Fragment Shader.
	 *
     * \param[in] NazwaShadera - nazwa shadera.
     * \return 0 gdy wszystko się udało, -1 gdy wystąpił błąd.
     */
    int8_t UstawFragmentShader(QString NazwaShadera);

    /*!
     * \brief Linkuje shadery sprawdzając czy wszystkie są poprawnie napisane.
	 *
	 * Linkuje shadery sprawdzając czy wszystkie są poprawnie napisane.
	 *
     * \return 0 gdy wszystko się udało, -1 gdy wystąpił błąd.
     */
    int8_t LinkujShader();

    /*!
     * \brief Tworzenie modelu Ziemi i dodawanie jej do sceny.
	 *
	 * Tworzenie modelu Ziemi i dodawanie jej do sceny.
	 *
     * \param[in] Promien - promien Ziemi.
     */
    void TworzZiemie(GLfloat Promien);

    /*!
     * \brief Tworzy punkt wskazujący miejsce, nad którym znajduje się stacja kosmiczna.
	 *
	 * Tworzy punkt wskazujący miejsce, nad którym znajduje się stacja kosmiczna.
	 *
     */
    void TworzPunktPozycji();

    /*!
     * \brief Tworzy pustą trajektorie, którą można później wypełnić.
	 *
	 * Tworzy pustą trajektorie, którą można później wypełnić.
	 *
     * \param[in] IloscDanych - ilość danych, które mają się mieścić w trajektorii. Na przykład jeżeli trajektoria składa się z 5 punktów i każdy punkt ma 3 współrzędne to ilość danych wynosi 15.
     * \param[in] IDObiektu - identyfikator, do którego zostanie przypisana dana trajektoria.
     */
    void TworzTrajektorie(size_t IloscDanych, GLuint* IDObiektu);

    /*!
     * \brief Tworzy wskaźnik pozwalający wskazać, którego punktu trajektorii dotyczą obecnie wyświetlane dane.
	 *
	 * Tworzy wskaźnik pozwalający wskazać, którego punktu trajektorii dotyczą obecnie wyświetlane dane.
	 *
     */
    void TworzWskaznikTrajektorii();

    /*!
     * \brief Pozwala wypełnić wcześniej stworzoną trajektorie danymi.
	 *
	 * Pozwala wypełnić wcześniej stworzoną trajektorie danymi.
	 *
     * \param[in] Dane - magazyn danych, z którego ma zostać utworzona trajektoria.
     */
    void WypelniTrajektorie(Magazyn_danych Dane);

    /*!
     * \brief Tworz siatkę nad Ziemią pokazującą strefy czasowe.
	 *
	 * Tworz siatkę nad Ziemią pokazującą strefy czasowe.
	 *
     */
    void TworzSiatke();

    /*!
     * \brief Pozwala włączyć śledzenie wskaźnika stacji przez kamerę.
	 *
	 * Pozwala włączyć śledzenie wskaźnika stacji przez kamerę.
	 *
     * \param[in] Sledz - jeżeli parametr przyjmnie wartość true śledzenie zostanie właczone, false wyłączy śledzenie.
     */
    void SledzenieStacji(GLboolean Sledz){fSledzenieStacji = Sledz;}

    /*!
     * \brief Metoda pozwala włączyć rysowanie trajektorii stacji co spowoduje, że stanie się widoczna.
	 *
	 * Metoda pozwala włączyć rysowanie trajektorii stacji co spowoduje, że stanie się widoczna.
	 *
     * \param[in] Rysuj - jeżeli parametr przyjmnie wartość true obiekt zostanie narysowany, false spowoduje, że obiek nie będzie narysowany.
     */
    void RysowanieTrajektorii(GLboolean Rysuj){Scena->RysujObiekt(IDTrajektoria, Rysuj);}

    /*!
     * \brief Metoda pozwala włączyć rysowanie końcowej części trajektorii stacji co spowoduje, że stanie się widoczna.
	 *
	 * Metoda pozwala włączyć rysowanie końcowej części trajektorii stacji co spowoduje, że stanie się widoczna.
	 *
     * \param[in] Rysuj - jeżeli parametr przyjmnie wartość true obiekt zostanie narysowany, false spowoduje, że obiek nie będzie narysowany.
     */
    void RysowanieKoncowkeTrajektorii(GLboolean Rysuj){Scena->RysujObiekt(IDTrajektoriaKoncowka, Rysuj);}

    /*!
     * \brief Metoda pozwala włączyć rysowanie siatki nad modelem Ziemi co spowoduje, że stanie się widoczna.
	 *
	 * Metoda pozwala włączyć rysowanie siatki nad modelem Ziemi co spowoduje, że stanie się widoczna.
	 *
     * \param[in] Rysuj - jeżeli parametr przyjmnie wartość true obiekt zostanie narysowany, false spowoduje, że obiek nie będzie narysowany.
     */
    void RysowanieSiatki(GLboolean Rysuj){Scena->RysujObiekt(IDSiatki, Rysuj);}

    /*!
     * \brief Metoda pozwala włączyć rysowanie punktu śledzącego pozycje stacji co spowoduje, że stanie się widoczny.
	 *
	 * Metoda pozwala włączyć rysowanie punktu śledzącego pozycje stacji co spowoduje, że stanie się widoczny.
	 *
     * \param[in] Rysuj - jeżeli parametr przyjmnie wartość true obiekt zostanie narysowany, false spowoduje, że obiek nie będzie narysowany.
     */
    void RysowaniePunktuSledzacego(GLboolean Rysuj){Scena->RysujObiekt(IDWskaznikStacji, Rysuj);}

    /*!
     * \brief Metoda pozwala włączyć rysowanie wskaźnika trajektorii co spowoduje, że stanie się widoczny.
	 *
	 * Metoda pozwala włączyć rysowanie wskaźnika trajektorii co spowoduje, że stanie się widoczny.
	 *
     * \param[in] Rysuj - jeżeli parametr przyjmnie wartość true obiekt zostanie narysowany, false spowoduje, że obiek nie będzie narysowany.
     */
    void RysowanieWskTrajektorii(GLboolean Rysuj){Scena->RysujObiekt(IDWskTrajektorii, Rysuj);}

    /*!
     * \brief Pozwala ustawić kamerę na podaną strefę czasową.
	 *
	 * Pozwala ustawić kamerę na podaną strefę czasową.
	 *
     * \param[in] Strefa - numer strefy czasowej UTC.
     */
    void UstawKamereNaStrefie(int Strefa);

    /*!
     * \brief Metoda pozwalająca włączyć lub wyłączyc podświetlanie strefy czasowej.
	 *
	 * Metoda pozwalająca włączyć lub wyłączyc podświetlanie strefy czasowej.
	 *
     * \param[in] Podswietl - ustawiony na 0 powoduje, że strefa nie będzie podświetlona, natomiast na 1 powoduje podświetlenie strefy.
     */
    void PodswietlenieStrefy(int Podswietl){Scena->PrzekazIntaDoShedera("WlaczStrefe", Podswietl);}

protected:

    /*!
     * \brief Metoda inicjująca działanie OpenGL.
	 *
	 * Metoda inicjująca działanie OpenGL.
     */
    void initializeGL();

    /*!
     * \brief Metoda wywoływana podczas zmiany okna.
	 *
	 * Metoda wywoływana podczas zmiany okna.
	 *
     * \param[in] Szerokosc - nowa szerokość okna.
     * \param[in] Wysokosc - nowa wysokość okna.
     */
    void resizeGL(int Szerokosc, int Wysokosc);

    /*!
     * \brief Metoda pozwalająca rysować modele 3D.
	 *
	 * Metoda pozwalająca rysować modele 3D.
     */
    void paintGL();

    /*!
     * \brief Metoda obsługująca zdarzenia związane z obrotem modelu Ziemi.
	 *
	 * Metoda obsługująca zdarzenia związane z obrotem modelu Ziemi.
	 *
     * \param[in] Zdarzenie - wskaźnik na odbierane zdarzenia.
     * \return Jeżeli zdarzenie zostanie rozpoznane zwraca true, inaczej zwraca false.
     */
    bool event(QEvent* Zdarzenie);

private slots:

    /*!
     * \brief Slot pozwalający aktualizować obecną pozycje wskaźnika stacji kosmicznej.
	 *
	 * Slot pozwalający aktualizować obecną pozycje wskaźnika stacji kosmicznej.
	 *
     * \param NoweDane - dane zawierające aktualną pozycję stacji kosmicznej.
     */
    void NoweDaneISS(ISS_Dane NoweDane);

    /*!
     * \brief Slot pozwalający uaktualniać dane w trajektorii stacji kosmicznej.
	 *
	 * Slot pozwalający uaktualniać dane w trajektorii stacji kosmicznej.
	 *
     * \param[in] NoweDane - dane, które zostaną zamienione z najstarszymi danymi w trajektorii stacji.
    */
void TrajektoriaNoweDane(ISS_Dane NoweDane);


private:

    /*!
     * \brief Wskaźnik na obiekt kalsy Grafika3D pozwalający na dokonywaniu operacji na grafice 3D, w tym dodawaniu nowych obiektów do sceny.
	 *
	 * Wskaźnik na obiekt kalsy Grafika3D pozwalający na dokonywaniu operacji na grafice 3D, w tym dodawaniu nowych obiektów do sceny.
     */
    Grafika3D* Scena;

    /*!
     * \brief Szerokość okna (widżetu), w którym wyświetlany jest model.
	 *
	 * Szerokość okna (widżetu), w którym wyświetlany jest model.
     */
    int SzerokoscOkna;

    /*!
     * \brief Wysokość okna (widżetu), w którym wyświetlany jest model.
	 *
	 * Wysokość okna (widżetu), w którym wyświetlany jest model.
     */
    int WysokoscOkna;

    /*!
     * \brief Odległość kamery od środka modelu Ziemi.
	 *
	 * Odległość kamery od środka modelu Ziemi.
     */
    GLfloat KameraOdObiektu;

    /*!
     * \brief Wpółrzędna X położenia kamery w przestrzeni 3D.
	 *
	 * Wpółrzędna X położenia kamery w przestrzeni 3D.
     */
    GLfloat KameraX;

    /*!
     * \brief Wpółrzędna Y położenia kamery w przestrzeni 3D.
	 *
	 * Wpółrzędna Y położenia kamery w przestrzeni 3D.
     */
    GLfloat KameraY;

    /*!
     * \brief Wpółrzędna Z położenia kamery w przestrzeni 3D.
	 *
	 * Wpółrzędna Z położenia kamery w przestrzeni 3D.
     */
    GLfloat KameraZ;

    /*!
     * \brief Kąt o jaki obrócony jest model Ziemi względem osi Y.
	 *
	 * Kąt o jaki obrócony jest model Ziemi względem osi Y.
     */
    GLfloat ObrotX;

    /*!
     * \brief Kąt o jaki obrócony jest model Ziemi względem osi X.
	 *
	 * Kąt o jaki obrócony jest model Ziemi względem osi X.
     */
    GLfloat ObrotY;

    /*!
     * \brief Współrzędna X ostatniej zapamiętanej pozycji myszki po kliknięciu lewego przycisku myszki.
	 *
	 * Współrzędna X ostatniej zapamiętanej pozycji myszki po kliknięciu lewego przycisku myszki.
     */
    int PosMyszyX;

    /*!
     * \brief Współrzędna Y ostatniej zapamiętanej pozycji myszki po kliknięciu lewego przycisku myszki.
	 *
	 * Współrzędna Y ostatniej zapamiętanej pozycji myszki po kliknięciu lewego przycisku myszki.
     */
    int PosMyszyY;

    /*!
     * \brief Promień modelu Ziemi.
	 *
	 * Promień modelu Ziemi.
     */
    GLfloat PromienKuli;

    /*!
     * \brief Szerokość geograficzna ostatniego punktu, nad którym znajdowała się stacja.
	 *
	 * Szerokość geograficzna ostatniego punktu, nad którym znajdowała się stacja.
     */
    GLfloat KatSzer;

    /*!
     * \brief Długość geograficzna ostatniego punktu, nad którym znajdowała się stacja.
	 *
	 * Długość geograficzna ostatniego punktu, nad którym znajdowała się stacja.
     */
    GLfloat KatDlug;

    /*!
     * \brief Identyfikator trajektorii uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
	 *
	 * Identyfikator trajektorii uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
     */
    GLuint IDTrajektoria;

    /*!
     * \brief Przesunięcie pozwalające zapisywać w odpowiedniej kolejności dane do końcowej części trajektorii stacji.
	 *
	 * Przesunięcie pozwalające zapisywać w odpowiedniej kolejności dane do końcowej części trajektorii stacji.
     */
    GLuint OffsetTrajektori;

    /*!
     * \brief Identyfikator siatki uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
	 *
	 * Identyfikator siatki uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
     */
    GLuint IDSiatki;

    /*!
     * \brief Identyfikator wskaźnika trajektorii uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
	 *
	 * Identyfikator wskaźnika trajektorii uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
     */
    GLuint IDWskTrajektorii;

    /*!
     * \brief Identyfikator wskaźnika położenia stacji uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
	 *
	 * Identyfikator wskaźnika położenia stacji uzyskany podczas dodawania obiektu do sceny. Jest potrzebny do modyfikacji obiektu.
     */
    GLuint IDWskaznikStacji;

    /*!
     * \brief Identyfikator końcowej części trajektorii stacji.
	 *
	 * Identyfikator końcowej części trajektorii stacji.
     */
    GLuint IDTrajektoriaKoncowka;

    /*!
     * \brief Pole używane jako flaga, ustawione powoduje śledznie pozycji stacji przez kamere.
	 *
	 * Pole używane jako flaga, ustawione powoduje śledznie pozycji stacji przez kamere.
     */
    GLuint fSledzenieStacji;

};

#endif // OPENGLWIDGET_H
