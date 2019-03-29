#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

using namespace std;
using namespace glm;

class Shader
{
    GLuint shaderProgram;
    vector <uint32_t> shaderIDs;

    string WczytajPlik(const char* plik);
    int StworzShader(const char* nazwaPliku, GLenum shaderTyp);
    void WypiszBledyKompilacji(int32_t shaderID);
    uint8_t KompilujShader(int shaderID);
    void WypiszBledyLinkowania(int32_t shaderID);

    public:

    Shader(): shaderProgram(0){;}
    uint8_t Init();
    uint8_t WczytajShader(const char* nazwaPliku, GLenum shaderTyp);
    uint8_t LinkowanieShader();
    void Sprzatanie();
    void Uzyjprogramu();
    GLuint ZwrocProgramID(){return shaderProgram;}

    void PrzekazMacierz4x4(const char* NazwaWShaderze, mat4 macierz);
    void PrzekazInt(const char* NazwaWShederze, int Wartosc);
};

#endif
