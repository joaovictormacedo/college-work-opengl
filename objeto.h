#pragma once //pragma once para que o objeto seja compilado apenas uma vez
#ifndef OBJETO_H
#define OBJETO_H

//Inclus�o Bibliotecas
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//Defini��es de Tipo
typedef glm::vec3 vec3;
using namespace std;

//Escopo de arquivo
extern vector <glm::vec3> vertices;
extern vector <glm::vec3> normais;
extern vector <unsigned int> indices;

//Prot�tipo de Fun��es
void parse(const char*);

#endif