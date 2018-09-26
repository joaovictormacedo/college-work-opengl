//Inclusão de Bibliotecas
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shaders.h"
#include "objeto.h"
#include "transformacoes.h"

//Variáveis Globais
int w, h; //Largura e altura inicial da janela
int amount; //Quantidade de rotação para cada vez que pressionar a tecla
float distancia = 30.0f;  //Distancia Inicial do Objeto
using namespace std;
vec3 eye;
vec3 up;
vec3 target;

//Constantes
const vec3 eyeinit(0.0f, 0.0f, 10.0f);
const vec3 upinit(0.0f, 1.0f, 0.0f);
const vec3 targetInit(0.0f, 0.0f, 0.0f);
const int amountinit = 1.5f;


/* Monkey */
GLuint projectionLoc_monkey, modelLoc_monkey, viewLoc_monkey, lightColorLoc_monkey;
std::vector <glm::vec3> vertices_monkey;
std::vector <glm::vec3> normais_monkey;
std::vector <unsigned int> indices_monkey;
mat4 model_monkey, view_monkey, projection_monkey;
GLuint VAO, VBO, EBO;
glm::vec3 monkeyPos(0.0f, -180.0f, 0.0f);
GLint program;


/* Montain */
GLuint projectionLoc_montain, modelLoc_montain, viewLoc_montain, lightColorLoc_montain;
std::vector <glm::vec3> vertices_montain;
std::vector <glm::vec3> normais_montain;
std::vector <unsigned int> indices_montain;
mat4 model_montain, view_montain, projection_montain;
GLuint VAO_montain, VBO_montain, EBO_montain;
glm::vec3 montainPos(0.0f, 0.5f, -6.0f);
//GLfloat lightColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLuint program_montain;

/* Light */
GLuint lampProjectionLoc, lampModelLoc, lampViewLoc, lampLightColor;
std::vector <glm::vec3> vertices_lamp;
std::vector <glm::vec3> normais_lamp;
std::vector <unsigned int> indices_lamp;
mat4 lampModel, lampView, lampProjection;
glm::vec3 lightPos(2.3f, 1.8f, -10.0f);
GLfloat lightColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLuint lampVAO, lampVBO, lampEBO;
GLuint programLamp; 


/* Protótipo das Funções */
void initMonkey();
void initLamp();
void initMontain();

void monkeyRender();
void lampRender();
void montainRender();

//Callback - Eventos do Usuário
void callback_teclado(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	switch (key) {
		case GLFW_KEY_A:
			//distancia+
			if (distancia >= 1.0f && distancia <= 45.0f)
				distancia -= 1.0f;
			if (distancia <= 1.0f)
				distancia = 1.0f;
			if (distancia >= 45.0f)
				distancia = 45.0f;
			break;
		case GLFW_KEY_D:
			//distancia-
			if (distancia >= 1.0f && distancia <= 45.0f)
				distancia += 1.0f;
			if (distancia <= 1.0f)
				distancia = 1.0f;
			if (distancia >= 45.0f)
				distancia = 45.0f;
			break;

		case GLFW_KEY_6:
			target = vec3(1.0f, 0.0f, 0.0f);
			break;

		case GLFW_KEY_LEFT:
			esquerda(amount, eye, up);
			break;
		case GLFW_KEY_RIGHT:
			esquerda(-amount, eye, up);
			break;
		case GLFW_KEY_UP:
			paraCima(amount, eye, up);
			break;
		case GLFW_KEY_DOWN:
			paraCima(-amount, eye, up);
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(win, true);
			break;
		case GLFW_KEY_R:
			eye = eyeinit;
			up = upinit;
			amount = amountinit;
			distancia = 45.0f;
			break;
	}
}

//Callback - Redimensionamento
void callback_redimensionamento(GLFWwindow* win, int width, int height) {
	//Para tela de retina
	//glfwGetFramebufferSize(win, &width, &height);
	
	w = width;
	h = height;
	glViewport(0, 0, w, h);

	// Abaixo defina a matrix de projecao
	// e a passe para o programa shader usando glUniformMatrix4fv(). 
	
	//projection = glm::perspective(glm::radians(45.0f), (float)w/(float)h, 0.1f, 100.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(projectionLoc_monkey, 1, GL_FALSE, glm::value_ptr(projection_monkey));
	glUniformMatrix4fv(lampProjectionLoc, 1, GL_FALSE, glm::value_ptr(lampProjection));
}

//Função Inicial
void init() {

	//Inicializa as variáveis
	eye = eyeinit;
	up = upinit;
	amount = amountinit;
	w = 800;
	h = 600;

	/* Funções que Inicializa os objetos da cena */
	initMonkey();
	initLamp();
	initMontain();

	

}

//Função de Renderização
void renderiza()
{

	// preenche buffer (pinta os pixels) associado ao fundo com branco
	//GLfloat cor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glClearBufferfv(GL_COLOR, 0, cor);
	
	// preenche buffer (pinta os pixels) associado ao fundo com branco
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lampRender();
	monkeyRender();
	montainRender();


}

//Função Principal
int main() {

	/* Inicialização e configuração do GLFW */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/* Criação da Janela */
	GLFWwindow* win = glfwCreateWindow(800, 600, "Exercicios 1", NULL, NULL);
	if (win == NULL) {
		std::cout << "Falha na criação da janela!" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Torna a janla o contexto da thread chamadora */
	glfwMakeContextCurrent(win);

	/* Inicialização do GLAD */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Falha na inicialização do GLAD" << std::endl;
		return -1;
	}

	/* Registro de callbacks */
	glfwSetKeyCallback(win, callback_teclado);
	glfwSetFramebufferSizeCallback(win, callback_redimensionamento);

	/* Função de Inicialização */
	init();

	/* Loop de renderização */
	while (!glfwWindowShouldClose(win))
	{

		/* Renderiza a cena na função renderiza */
		renderiza();


		glfwSwapBuffers(win);
		glfwWaitEvents();
		//glfwPollEvents();
		//glfwWaitEventsTimeout(0.5);

	}

	/* Libera recursos alocados */
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(program);

	glDeleteBuffers(1, &lampVBO);
	glDeleteBuffers(1, &lampEBO);
	glDeleteVertexArrays(1, &lampVAO);
	glDeleteProgram(programLamp);
	glfwTerminate();

	return 0;
}



/* Objeto Monkey */
void initMonkey() {

	//Compila e linka os shaders
	GLint vertShader = compileShader(GL_VERTEX_SHADER, "./shader.ver");
	GLint fragShader = compileShader(GL_FRAGMENT_SHADER, "./shader.fra");

	program = createProgram(vertShader, fragShader);

	// le o arquivo OBJ do teapot
	parse2("./monkeyspider.obj", vertices_monkey, normais_monkey, indices_monkey);


	//Recupera a localização da transformação
	projectionLoc_monkey = glGetUniformLocation(program, "projection");
	viewLoc_monkey = glGetUniformLocation(program, "view");
	modelLoc_monkey = glGetUniformLocation(program, "model");
	lightColorLoc_monkey = glGetUniformLocation(program, "lightColor");


	//Cria um Vertex Array Object, um Vertex Buffer Object e um Element Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//ativa o Vertex Array Object VAO
	glBindVertexArray(VAO);

	//Povoa o VBO com os vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices_monkey.size(), &vertices_monkey[0], GL_STATIC_DRAW);

	//Povoa o EBO com os indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices_monkey.size(), &indices_monkey[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}
void monkeyRender() {
	// usa os shaders criados anteriormente em init()
	glUseProgram(program);
	glBindVertexArray(VAO);

	//Matriz Modelo
	model_monkey = minhaEscala(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	model_monkey = glm::translate(model_monkey, monkeyPos);

	//Matriz de Visualização
	view_monkey = myLookAt(eye, up);


	//Matriz de Projeçao
	projection_monkey = glm::perspective(glm::radians(distancia), (float)w / (float)h, 0.1f, 100.0f);

	//Passa as matrizes para o vertex shader 
	glUniformMatrix4fv(modelLoc_monkey, 1, GL_FALSE, glm::value_ptr(model_monkey));
	glUniformMatrix4fv(viewLoc_monkey, 1, GL_FALSE, glm::value_ptr(view_monkey));
	glUniformMatrix4fv(projectionLoc_monkey, 1, GL_FALSE, glm::value_ptr(projection_monkey));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indices_monkey.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/* Objeto Lamp */
void initLamp() {
	GLint vertShaderLamp = compileShader(GL_VERTEX_SHADER, "./shader_lamp.ver");
	GLint fragShaderLamp = compileShader(GL_FRAGMENT_SHADER, "./shader_lamp.fra");
	programLamp = createProgram(vertShaderLamp, fragShaderLamp);

	//le arquivo OBJ da esfera
	parse("./sphere.obj", vertices_lamp, normais_lamp, indices_lamp);


	//Recupera a localização da transformação
	lampProjectionLoc = glGetUniformLocation(programLamp, "projection");
	lampViewLoc = glGetUniformLocation(programLamp, "view");
	lampModelLoc = glGetUniformLocation(programLamp, "model");
	lampLightColor = glGetUniformLocation(programLamp, "lightC");

	//Cria um Vertex Array Object, um Vertex Buffer Object e um Element Buffer Object
	glGenVertexArrays(1, &lampVAO);
	glGenBuffers(1, &lampVBO);
	glGenBuffers(1, &lampEBO);

	//ativa o Vertex Array Object VAO
	glBindVertexArray(lampVAO);

	//Povoa o VBO com os vertices
	glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices_lamp.size(), &vertices_lamp[0], GL_STATIC_DRAW);

	//Povoa o EBO com os indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lampEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices_lamp.size(), &indices_lamp[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void lampRender() {
	/////////////////////////////////////////////lamp
	// usa os shaders criados anteriormente em init()

	glUseProgram(programLamp);
	glBindVertexArray(lampVAO);

	//Matriz Modelo
	lampModel = minhaEscala(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	lampModel = glm::translate(lampModel, lightPos);

	//Matriz de Visualização
	lampView = myLookAt(eye, up);

	//Matriz de Projeçao
	lampProjection = glm::perspective(glm::radians(distancia), (float)w / (float)h, 0.1f, 100.0f);

	//Passa as matrizes para o vertex shader 
	glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(lampModel));
	glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(lampView));
	glUniformMatrix4fv(lampProjectionLoc, 1, GL_FALSE, glm::value_ptr(lampProjection));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indices_lamp.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/* Objeto Montain */
void initMontain() {
	GLint vertShaderMontain = compileShader(GL_VERTEX_SHADER, "./shader.ver");
	GLint fragShaderMontain = compileShader(GL_FRAGMENT_SHADER, "./shader.fra");
	program_montain = createProgram(vertShaderMontain, fragShaderMontain);

	//le arquivo OBJ da esfera
	parse("./mountain.obj", vertices_montain, normais_montain, indices_montain);


	//Recupera a localização da transformação
	projectionLoc_montain = glGetUniformLocation(program_montain, "projection");
	viewLoc_montain = glGetUniformLocation(program_montain, "view");
	modelLoc_montain = glGetUniformLocation(program_montain, "model");
	lightColorLoc_montain = glGetUniformLocation(program_montain, "lightColor");

	//Cria um Vertex Array Object, um Vertex Buffer Object e um Element Buffer Object
	glGenVertexArrays(1, &VAO_montain);
	glGenBuffers(1, &VBO_montain);
	glGenBuffers(1, &EBO_montain);

	//ativa o Vertex Array Object VAO
	glBindVertexArray(VAO_montain);

	//Povoa o VBO com os vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_montain);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices_montain.size(), &vertices_montain[0], GL_STATIC_DRAW);

	//Povoa o EBO com os indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_montain);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices_montain.size(), &indices_montain[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void montainRender() {
	/////////////////////////////////////////////lamp
	// usa os shaders criados anteriormente em init()

	glUseProgram(program_montain);
	glBindVertexArray(VAO_montain);

	//Matriz Modelo
	model_montain = minhaEscala(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	model_montain = glm::translate(model_montain, montainPos);

	//Matriz de Visualização
	view_montain = myLookAt(eye, up);

	//Matriz de Projeçao
	projection_montain = glm::perspective(glm::radians(distancia), (float)w / (float)h, 0.1f, 100.0f);

	//Passa as matrizes para o vertex shader 
	glUniformMatrix4fv(modelLoc_montain, 1, GL_FALSE, glm::value_ptr(model_montain));
	glUniformMatrix4fv(viewLoc_montain, 1, GL_FALSE, glm::value_ptr(view_montain));
	glUniformMatrix4fv(projectionLoc_montain, 1, GL_FALSE, glm::value_ptr(projection_montain));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indices_montain.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}