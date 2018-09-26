#pragma once
#include <iostream>
#include <string>

#ifndef INCLUDE_SHADERS 
#define INCLUDE_SHADERS 

std::string shaderSrcRead(const char * filename);
void programErrors(const GLint program);
void shaderErrors(const GLint shader);
GLuint compileShader(GLenum type, const char * filename);
GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);

#endif 

