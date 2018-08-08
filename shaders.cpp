#include <iostream>
#include <fstream>
#include <cstring>
#include <glad/glad.h>
#include "shaders.h"

using namespace std;

/* Le o arquivo fonte do shader para uma string c++ */
string shaderSrcRead(const char * filename) {
	string str, ret = "";
	ifstream in;
	in.open(filename);

	if (in.is_open()) {
		getline(in, str);
		while (in) {
			ret += str + "\n"; //concatena as linhas do arquivo
			getline(in, str);
		}
		return ret;
	}
	else {
		cerr << "Erro ao abrir o arquivo " << filename << "\n";
		throw 2;
	}
}

/* Compila um shader, vertex ou fragment ambos na mesma função */
GLuint compileShader(GLenum type, const char * filename) {
	string str = shaderSrcRead(filename);
	const GLchar *cstr = str.c_str(); //converte para um char[] com um \0 no final 

	GLuint shader = glCreateShader(type); //cria o shader
	glShaderSource(shader, 1, &cstr, NULL); //copia o codigo no objeto 
	glCompileShader(shader); //compila o shader

							 //verifica se houve erros de compilação
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		shaderErrors(shader);
		throw 3;
	}
	return shader;
}

void shaderErrors(const GLint shader) {
	GLint length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length); //recupera o tamanho do log de erro

	GLchar * log;
	log = new GLchar[length + 1];
	glGetShaderInfoLog(shader, length, &length, log);
	cout << "Erro de compilação abaixo:\n" << log << "\n";
	delete[] log;
}

GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) {

	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		programErrors(program);
		throw 4;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}


void programErrors(const GLint program) {
	GLint length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

	GLchar * log;
	log = new GLchar[length + 1];
	glGetProgramInfoLog(program, length, &length, log);
	cout << "Erro de linkagem abaixo\n" << log << "\n";
	delete[] log;
}