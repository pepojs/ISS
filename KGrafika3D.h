#ifndef KGRAFIKA3D_H
#define KGRAFIKA3D_H

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include "SOIL.h"
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#define GLEW_STATIC
#include <glew.h>

#include "Shader.h"
#include "SObiektNaScenie.h"

#include <QSize>
#include <QWidget>

using namespace std;

class Grafika3D
{
    Shader shader, shaderTekstu;
    vector<GLuint> vbo;
    vector<GLuint> vao;
    vector<ObiektNaScenie> ObiektyNaScenie;
    vector<NapisNaScenie> NapisyNaScenie;
    map<GLchar, ZnakNaScenie> ZnakiNaScenie;

    GLuint VAOCzcionki, VBOCzcionki;
    glm::vec3 PozycjaKamery; //Pozycja kamery na scenie (wspolrzedne globalne)
    glm::vec3 CelKamery; //Punkt, na ktory "patrzy" kamera
    glm::vec3 OsZKamery; //Wektor wskazujacy kierunek osi wychodzacej z gornej czesci kamery, prostopadly do gornej powierzchni

    public:
    ~Grafika3D();
    Shader* ZwrocAdresShadera(){return &shader;}

    //***Metody Tworzenie, kopiowanie, dodawanie i usuwanie Obiektow***
    GLuint TworzObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu);
    GLuint TworzObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu,
                       GLvoid* TablicaKolorow, size_t RozmiarTablicyKolorow, uint8_t IloscWspolrzednychKoloru);
    GLuint TworzObiekt(GLvoid* WspolnaTablica, size_t RozmiarTablicy, uint8_t IloscWspolrzednychPunktu, uint8_t IloscWspolrzednychKoloru,
                       uint8_t IloscWspolrzednychTekstury);
    GLuint DodajObiektDoSceny(GLuint vaoObiektu, GLuint IloscPunktowWObiekcie);
    GLuint DodajObiektDoSceny(GLuint vaoObiektu, GLuint IloscPunktowWObiekcie, Graf3D_RodzajObiektu RodzajObiektu);

    void UsunObiektZeSceny(GLuint IDObiektu);
    GLuint KopiujObiekt(GLuint IDObiektu);

    //***Metoda laczaca tworzenie i dodawanie obiektu na scene
    GLuint DodajObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu, GLuint IloscPunktowWObiekcie);
    GLuint DodajObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu, GLuint IloscPunktowWObiekcie,
                       Graf3D_RodzajObiektu RodzajObiektu);

    GLuint DodajObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu, GLvoid* TablicaKolorow,
                       size_t RozmiarTablicyKolorow, uint8_t IloscWspolrzednychKoloru, GLuint IloscPunktowWObiekcie);
    GLuint DodajObiekt(GLvoid* TablicaPunktow, size_t RozmiarTablicyPunktow, uint8_t IloscWspolrzednychPunktu, GLvoid* TablicaKolorow,
                       size_t RozmiarTablicyKolorow, uint8_t IloscWspolrzednychKoloru, GLuint IloscPunktowWObiekcie, Graf3D_RodzajObiektu RodzajObiektu);

    GLuint DodajObiekt(GLvoid* WspolnaTablica, size_t RozmiarTablicy, uint8_t IloscWspolrzednychPunktu, uint8_t IloscWspolrzednychKoloru,
                       uint8_t IloscWspolrzednychTekstury, GLuint IloscPunktowWObiekcie);
    GLuint DodajObiekt(GLvoid* WspolnaTablica, size_t RozmiarTablicy, uint8_t IloscWspolrzednychPunktu, uint8_t IloscWspolrzednychKoloru,
                       uint8_t IloscWspolrzednychTekstury, GLuint IloscPunktowWObiekcie, Graf3D_RodzajObiektu RodzajObiektu);

    //***Metody Transformacja Obiektu
    void PrzesunObiekt(GLuint IDObiektu, glm::vec3 Przesuniecie);
    void ObrocObiekt(GLuint IDObiektu, GLfloat KatObrotu, glm::vec3 OsObrotu);
    void PrzeskalujObiekt(GLuint IDObiektu, glm::vec3 WektorSkali);
    void UstawMacierzTransformacji(GLuint IDObiektu, glm::mat4 NowaMacierzTransformacji);

    //***Metody Rzutowanie***
    void UstawRzutowanieOrtogonalne(GLfloat Lewa, GLfloat Prawa, GLfloat Dol, GLfloat Gora, GLfloat Przod, GLfloat Tyl);
    void UstawRzutowaniePerspektywiczne(GLfloat Kat, GLfloat ProporcjeEkranu, GLfloat Przod, GLfloat Tyl);

    //***Metody Kamera***
    void UstawKamere(glm::vec3 Pozycja, glm::vec3 Cel, glm::vec3 OsZ);
    glm::vec3 ZwrocPozycjeKamery(){return PozycjaKamery;}
    glm::vec3 ZwrocCelKamery(){return CelKamery;}
    glm::vec3 ZwrocOsZKamery(){return OsZKamery;}

    //***Metody Tekstury**
    GLuint GenerujTeksture2D(const char* NazwaObrazka, GLint ZawijanieTekstury, GLint FiltracjaTekstury);
    void DodajTekstureDoObiektu(GLuint IDTekstury, GLuint IDObiektu);
    void UsunTekstureZObiektu(GLuint IDTekstury, GLuint IDObiektu);

    //***Metody Napisy***
    void WczytajCzciake(const char* ScierzkaCzcionki, GLuint RozmiarCzcionki,GLuint IloscZnakowDoWczytania, GLuint NumerAtrybutuShadera);
    void TworzObiektCzcionka(GLuint NumerAtrybutuShadera);
    void RenderujTekst(GLint WysokoscOkna, GLint SzerokoscOkna, string Napis, GLfloat x, GLfloat y, GLfloat Skala, glm::vec3 Kolor);
    GLuint DodajNapisDoSceny(string Napis, GLfloat x, GLfloat y, GLfloat Skala, glm::vec3 Kolor);
    void UsunNapisZeSceny(GLuint IDNapisu);
    void UsunNapisZeSceny(string Napis);

    void Rysuj(GLint SzerokoscOkna, GLint WysokoscOkan);


};


#endif
