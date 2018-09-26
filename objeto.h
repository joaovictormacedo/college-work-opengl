#pragma once //pragma once para que o objeto seja compilado apenas uma vez
#ifndef OBJETO_H
#define OBJETO_H

//Inclusão Bibliotecas
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/*//Definições de Tipo
typedef glm::vec3 vec3;
using namespace std;

//Escopo de arquivo
extern vector <glm::vec3> vertices;
extern vector <glm::vec3> normais;
extern vector <unsigned int> indices;

//Protótipo de Funções
void parse(const char*);*/

// O objeto a ser renderizado deve ser informado como um arquivo OBJ.
// Estude sobre o formato de arquivos OBJ na Wikipedia eh suficiente.
void parse(const char* filepath, std::vector <glm::vec3> &vertices,
	std::vector <glm::vec3> &normais, std::vector <unsigned int> &indices);

void parse2(const char* filepath, std::vector <glm::vec3> &vertices,
	std::vector <glm::vec3> &normais, std::vector <unsigned int> &indices);

#endif