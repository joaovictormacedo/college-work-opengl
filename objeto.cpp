#include "objeto.h"



// Funcao para parsear o arquivo OBJ
// Peguei da internet, possiveis adaptacoes podem ser necessarias
void parse(const char * filepath, std::vector <glm::vec3> &vertices,
	std::vector <glm::vec3> &normais, std::vector <unsigned int> &indices)

{
	FILE* fp;
	float x, y, z;
	int fx, fy, fz, ignore;
	int c1, c2;
	float minY = INFINITY, minZ = INFINITY, minX = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY, maxX = -INFINITY;

	fp = fopen(filepath, "rb");

	if (fp == NULL) {
		std::cerr << "Erro carregando arquivo " << filepath << std::endl;
		std::getchar();
		exit(-1);
	}

	while (!feof(fp)) {
		c1 = fgetc(fp);
		while (!(c1 == 'v' || c1 == 'f')) {
			c1 = fgetc(fp);
			if (feof(fp))
				break;
		}
		c2 = fgetc(fp);

		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			vertices.push_back(glm::vec3(x, y, z));
			if (x < minX) minX = x;
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (x > maxX) maxX = x;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			normais.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if (c1 == 'f')
		{
			fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
			indices.push_back(fx - 1);
			indices.push_back(fy - 1);
			indices.push_back(fz - 1);
		}
	}

	fclose(fp);
	// Centralizando o objeto
	float avgX = (minX + maxX) / 2.0;
	float avgY = (minY + maxY) / 2.0f;
	float avgZ = (minZ + maxZ) / 2.0f;

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		glm::vec3 shiftedVertex = vertices[i] - glm::vec3(avgX, avgY, avgZ);
		vertices[i] = shiftedVertex;
	}
}

void parse2(const char * filepath, std::vector <glm::vec3> &vertices,
	std::vector <glm::vec3> &normais, std::vector <unsigned int> &indices)

{
	FILE* fp;
	float x, y, z;
	int fx, fy, fz, ignore;
	int c1, c2;
	float minY = INFINITY, minZ = INFINITY, minX = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY, maxX = -INFINITY;

	fp = fopen(filepath, "rb");

	if (fp == NULL) {
		std::cerr << "Erro carregando arquivo " << filepath << std::endl;
		std::getchar();
		exit(-1);
	}

	while (!feof(fp)) {
		c1 = fgetc(fp);
		while (!(c1 == 'v' || c1 == 'f')) {
			c1 = fgetc(fp);
			if (feof(fp))
				break;
		}
		c2 = fgetc(fp);

		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			vertices.push_back(glm::vec3(x, y, z));
			if (x < minX) minX = x;
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (x > maxX) maxX = x;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			normais.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if (c1 == 'f')
		{
			fscanf(fp, "%d/%d/%d %d/%d/%d %d/$d/%d %d/$d/%d", &fx, &ignore, &ignore, &fy, &ignore, &ignore, &fz, &ignore, &ignore, &ignore, &ignore, &ignore);
			indices.push_back(fx - 1);
			indices.push_back(fy - 1);
			indices.push_back(fz - 1);
		}
	}

	fclose(fp);
	// Centralizando o objeto
	float avgX = (minX + maxX) / 2.0;
	float avgY = (minY + maxY) / 2.0f;
	float avgZ = (minZ + maxZ) / 2.0f;

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		glm::vec3 shiftedVertex = vertices[i] - glm::vec3(avgX, avgY, avgZ);
		vertices[i] = shiftedVertex;
	}
}








/*
//Definição de Variáveis Globais
std::vector <glm::vec3> vertices;
std::vector <glm::vec3> normais;
std::vector <unsigned int> indices;

void parse(const char * filepath) {
	FILE* fp;
	float x, y, z;
	int fx, fy, fz, ignore;
	int c1, c2;
	float minY = INFINITY, minZ = INFINITY, minX = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY, maxX = -INFINITY;

	fp = fopen(filepath, "rb");

	if (fp == NULL) {
		std::cerr << "Erro carregando arquivo " << filepath << std::endl;
		std::getchar();
		exit(-1);
	}

	while (!feof(fp)) {
		c1 = fgetc(fp);
		while (!(c1 == 'v' || c1 == 'f')) {
			c1 = fgetc(fp);
			if (feof(fp))
				break;
		}
		c2 = fgetc(fp);

		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			vertices.push_back(glm::vec3(x, y, z));
			if (z < minX) minX = x;
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (x > maxX) maxX = x;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			normais.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if (c1 == 'f')
		{
			fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
			indices.push_back(fx - 1);
			indices.push_back(fy - 1);
			indices.push_back(fz - 1);
		}
	}

	fclose(fp);
	// Centralizando o objeto
	float avgX = (minX + maxX) / 2.0;
	float avgY = (minY + maxY) / 2.0f;
	float avgZ = (minZ + maxZ) / 2.0f;

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		glm::vec3 shiftedVertex = vertices[i] - glm::vec3(avgX, avgY, avgZ);
		vertices[i] = shiftedVertex;
	}
}*/