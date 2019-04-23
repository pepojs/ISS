#ifndef WYKRES_H
#define WYKRES_H

/*!
 * \file
 * \brief Definicja klasy Wykres
*/

#include <QWidget>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QtCharts/QDateTimeAxis>
#include <QEvent>
#include <QMouseEvent>

#include "iss_dane.h"

QT_CHARTS_USE_NAMESPACE

/*!
 * \brief Pozwala tworzyć wykresu wartości zależnej od czasu.
 *
 * Klasa pozwala na tworzenie wykresów, konfiguracje jego wyglądu, zakresu wyświetlanych danych oraz dodawania nowych punktów.
 * Wykres ma postać linii łączących podane punkty ze sobą.
 *
*/

class Wykres : public QWidget
{
Q_OBJECT
public:

    /*!
     * \brief Kostruktor tworzący domyslny wykres.
     * \param[in] Rodzic - wskaźnik na rodzica.
     */
    Wykres(QWidget* Rodzic = nullptr);

    /*!
     * \brief Zmienia kolor oraz grubość rysowanej linii wykresu.
     * \param[in] NowyKolor - nowy kolor linii wykresu.
     * \param[in] NowyRozmiar - nowa grubość linii wykresu.
     */
    void ZmienWykres(QRgb NowyKolor, int NowyRozmiar);

    /*!
     * \brief Zmienia kolor tła wykresu.
     * \param[in] NowyKolor - nowy kolor tła wykresu.
     */
    void ZmienKolorTla(QRgb NowyKolor);

    /*!
     * \brief Pozwala zmienić tytuł wykresu.
     * \param[in] NowyTytul - tytuł wykresu.
     */
    void UstawTytulWykresu(QString NowyTytul);

    /*!
     * \brief Pozwala zmienić kolor tytułu wykresu.
     * \param[in] NowyKolor - nowy kolor.
     */
    void ZmienKolorTytulu(QRgb NowyKolor);

    /*!
     * \brief Pozwala zmienić rozmiar czcionki tytułu wykresu.
     * \param[in] NowyRozmiar - nowy rozmiar czcionki.
     */
    void ZmienRozmiarTytulu(int NowyRozmiar);

    /*!
     * \brief Ustawienie minimalnej oraz maksymalnej wartości osi X.
     * \param[in] Min - minimalna wartość osi X.
     * \param[in] Max - maksymalna wartość osi X.
     */
    void UstawZakresOsiX(uint Min, uint Max);

    /*!
     * \brief Ustawienie minimalnej oraz maksymalnej wartości osi Y.
     * \param[in] Min - minimalna wartość osi Y.
     * \param[in] Max - maksymalna wartość osi Y.
     */
    void UstawZakresOsiY(double Min, double Max);

    /*!
     * \brief Ustawia liczbę punktów na podziałce osi X.
     * \param[in] NowaIlosc - nowa liczba punktów.
     */
    void ZmienIloscPodzialekOsiX(int NowaIlosc);

    /*!
     * \brief Ustawia liczbę punktów na podziałce osi Y.
     * \param[in] NowaIlosc - nowa liczba punktów.
     */
    void ZmienIloscPodzialekOsiY(int NowaIlosc);

    /*!
     * \brief Pozwala zmienić rozmiar podpisu osi X.
     * \param[in] NowyRozmiar - nowy rozmiar.
     */
    void ZmienRozmiarPodpisuOsiX(int NowyRozmiar);

    /*!
     * \brief Pozwala zmienić rozmiar podpisu osi Y.
     * \param[in] NowyRozmiar - nowy rozmiar.
     */
    void ZmienRozmiarPodpisuOsiY(int NowyRozmiar);

    /*!
     * \brief Zmienia grubość osi X
     * \param[in] NowyRozmiar - nowa grubość.
     */
    void ZmienGruboscOsiX(int NowyRozmiar);

    /*!
     * \brief Zmienia grubość osi Y
     * \param[in] NowyRozmiar - nowa grubość.
     */
    void ZmienGruboscOsiY(int NowyRozmiar);

    /*!
     * \brief Zmienia kolor osi X.
     * \param[in] NowyKolor - nowy kolor.
     */
    void ZmienKolorOsiX(QRgb NowyKolor);

    /*!
     * \brief Zmienia kolor osi X.
     * \param[in] NowyKolor - nowy kolor.
     */
    void ZmienKolorOsiY(QRgb NowyKolor);

    /*!
     * \brief Zmienia podpis osi X.
     * \param[in] NowyPodpis - nowy podpis osi X.
     */
    void ZmienPodpisOsiX(QString NowyPodpis);

    /*!
     * \brief Zmienia podpis osi Y.
     * \param[in] NowyPodpis - nowy podpis osi Y.
     */
    void ZmienPodpisOsiY(QString NowyPodpis);

    /*!
     * \brief Dodaje nowy punkt do wykresu.
     * \param[in] X - czas podany w formacie unixowym w sekundach.
     * \param[in] Y - wartość zmiennej.
     */
    void DodajDaneDoWykresu(uint X, double Y);

    /*!
     * \brief Usuwa wszystkie punkty z wykresu.
     */
    void WyczyscWykres();

    /*!
     * \brief Włącza siatkę na wykresie.
     * \param[in] Wlacz - jeżeli true włącza siatke, jeżeli falce wyłącza.
     */
    void WidokSiatki(bool Wlacz);

signals:
    /*!
     * \brief Sygnał emitowany po naciśnięciu myszką na wykres.
     */
    void Kliknieto();

protected:
    /*!
     * \brief Metoda obsługująca zdarzenia związane z kliknieciem myszką na wykres.
     * \param[in] Zdarzenie - wskaźnik na odbierane zdarzenia.
     * \return Jeżeli zdarzenie zostanie rozpoznane zwraca true, inaczej zwraca false.
     */
    bool event(QEvent* Zdarzenie);

private:

    /*!
     * \brief Wskaźnik na obiekt klasy QChartView pozwalający na wyświetlenie wykresu.
     */
    QChartView* WidokWykresu;

    /*!
     * \brief Wskaźnik na obiekt klasy QChart pozwalający tworzyć wykres.
     */
    QChart* PoleWykresu;

    /*!
     * \brief Wskaxnik na obiekt klasy QLineSeries pozwalający tworzyć liniową serie danych.
     */
    QLineSeries* LiniaWykresu;

    /*!
     * \brief Wskaźnik na obiekt kalsy QDateTimeAxis pozwalający stworzyć oraz skonfigurować oś X jak oś pokazująca czas.
     */
    QDateTimeAxis* OsX;

    /*!
     * \brief Wskaźnik na obiekt kalsy QValueAxis pozwalający stworzyć oraz skonfigurować oś Y.
     */
    QValueAxis* OsY;

    /*!
     * \brief Wskaźnik na warstwę, na której znajduje się wykres.
     */
    QGridLayout* WarstwaGlowna;
};

#endif // WYKRES_H
