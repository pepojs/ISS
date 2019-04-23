#ifndef MAGAZYN_DANYCH_H
#define MAGAZYN_DANYCH_H

/*!
 * \file
 * \brief Definicja klasy Magazyn_danych
*/

#include <vector>
#include <QDateTime>
#include <fstream>
#include <QCoreApplication>
#include <math.h>

#include "iss_dane.h"
#include "http.h"

using namespace std;

/*!
 * \brief Gromadzenie większej ilości dany na temat trajektorii lotu stacji.
 *
 * Klasa pozwala przechowywać dane na temat trajektorii lotu stacji kosmicznej,
 * wczytywać dane z pliku, ściągać z odpowiedniej strony internetowej oraz zapisywać na dysku komputera.
 * Magazyn danych opiera się o wektor, do którego dane są zapisywane jak do kolejki. Głową nazywamy element, który przy dodaniu
 * nowych danych zostanie najpierw zamieniony.
 *
*/

class Magazyn_danych
{
    /*!
     * \brief Wektor pozwalający przechowywać dane na temat stacji z różnych chwil czasu.
     */
    vector<ISS_Dane> MagazynDanych;

    /*!
     * \brief Wskazuje, który element zostanie następny podmieniony przy dodaniu nowych danych.
     */
    uint32_t Glowa;

public:

    /*!
     * \brief Zmienia rozmiar wektora na 5400 elementów oraz ustawia głowę na 0.
     */
    Magazyn_danych();

    /*!
     * \brief Zmienia rozmiar wektora na podaną ilość elementów oraz ustawia głowę na 0.
     * \param[in] IloscDanych - ilość elementów jaką chcemy przechowywać w wektorze.
     */
    Magazyn_danych(size_t IloscDanych);

    /*!
     * \brief Zapisuje dane z ustawioną częstotliwością na 60 sekund.
     */
    ~Magazyn_danych();

    /*!
     * \brief Wypełnia brakującą część wektora danych, danymi pobranymi z sieci.
     * \param[in] Html - wskaźnik na klasę Http potrzebny do pobrania danych.
     * \param[in] Czestotliwosc - odstęp czasu pomiedzy kolejnymi pobieranymi danami, wyrażony w sekundach.
     * \param[in] IloscDanychWypelnionych - ilość danych znajdujących sie już w wektorze z danymi.
     * \param[in] CzasDoTylu_S - czas w sekundach jaki zostanie odjęty od obecnego czasu. Od różnicy zacznie się wypełnianie wektora, kiedy nie posiada on żadnych danych.
     */
    void WypelniDanymiZSieci(Http* Html, uint32_t Czestotliwosc, uint32_t IloscDanychWypelnionych, uint32_t CzasDoTylu_S);

    /*!
     * \brief Metoda pozwala zapisać obecny stan wektora z danymi do pliku o nazwie DaneStacji.txt
     * \param[in] Czestotliwosc - odstęp czasu pomiedzy kolejnymi pobieranymi danami, wyrażony w sekundach.
     */
    void ZapiszDane(uint32_t Czestotliwosc);

    /*!
     * \brief WypelniDanymiZPliku
     * \param[in] Html - wskaźnik na klasę Http potrzebny do pobrania danych.
     * \param[in] CzasDoTylu_S - czas w sekundach jaki zostanie odjęty od obecnego czasu. Od ich różnicy zacznie się wypełnianie wektora.
     * \return Zwraca ilość wczytanych rekordów danych. Jeżeli zwróci -1 to znaczy, że pliku nie udało się otorzyć do odczytu.
     */
    long int WypelniDanymiZPliku(Http* Html, uint32_t CzasDoTylu_S);

    /*!
     * \brief Zwraca rozmiar wektora z danymi.
     * \return Rozmiar wektora.
     */
    size_t ZwrocIloscDanych(){return MagazynDanych.size();}

    /*!
     * \brief Zwraca dane z wektora znajdujące się na podanej pozycji.
     * \param[in] Ktore - pozacja w wektorze, na której znajdują się interesujące nas dane.
     * \return Wybrane dane na temat stacji.
     */
    ISS_Dane ZwrocDane(size_t Ktore){return MagazynDanych[Ktore];}

    /*!
     * \brief Zwraca pozycję głowy.
     * Głowa to miejsce gdzie zostaną zapisane kolejne dane, które zostaną przekazane do wektora.
     * \return Wartość Głowy.
     */
    uint32_t ZwrocGlowe(){return Glowa;}

    /*!
     * \brief Pozwala zamienić obecne dane nowymi w miejscu, na który wskazuje głowa.
     * \param[in] NoweDane - dane, które należy zapisać w wektorze.
     */
    void DodajNoweDane(ISS_Dane NoweDane);


};

#endif // MAGAZYN_DANYCH_H
