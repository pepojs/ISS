#ifndef SOBIEKTNASCENIE_H
#define SOBIEKTNASCENIE_H

#include <vector>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <glew.h>

using namespace std;



enum Graf3D_RodzajObiektu
{
    Graf3D_Punkt = 0,
    Graf3D_Krawedz = 1,
    Graf3D_PolaczonaKrawedz = 2,
    Graf3D_Plaszczyzna = 3
};


struct ObiektNaScenie
{
    GLuint IDObiektu;
    GLuint IDvaoObiektu;
    glm::mat4 MacierzTransformacji;
    GLuint IloscPunktow;
    vector<GLuint> Tekstury;
    Graf3D_RodzajObiektu RodzajObiektu;
};

struct ZnakNaScenie
{
    GLuint IDTekstury;
    glm::ivec2 Rozmiar;
    glm::ivec2 Przesuniecie; //Odstep litery w lewo i w gore od punktu poczatku jej zaczepienia
    GLuint Odstep; //Odstep miedzy kolejnymi literami, kolejnymi punktami jej zaczepienia
};

struct NapisNaScenie
{
    GLuint IDNapisu;
    string Napis;
    glm::vec3 KolorNapisu;
    GLfloat Skala;
    GLfloat X;
    GLfloat Y;
};

#endif // SOBIEKTNASCENIE_H
