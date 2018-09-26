#pragma once
#ifndef TRANSFORMACOES_H
#define TRANSFORMACOES_H

//Inclusão da GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

//Definições de Tipo
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

//Constantes
const float pi = 3.14159265359;

//Protótipo das Funções
void esquerda(float degrees, vec3 &eye, vec3 &up);
void paraCima(float degrees, vec3 &eye, vec3 &up);
vec3 prodVetorial(vec3 vet1, vec3 vet2);
vec3 normaliza(vec3 vetor);
mat3 rotaciona(const float degrees, const vec3 &axys);
mat4 translado(vec3 vetor);
mat4 myLookAt(vec3 eye, vec3 up);
mat4 minhaEscala(mat4 mIdent, vec3 coordenadas);




#endif