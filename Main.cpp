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

using namespace std;

//Constantes
const vec3 eyeinit(0.0f, 0.0f, 7.0f);
const vec3 upinit(0.0f, 1.0f, 0.0f);
const vec3 targetInit(0.0f, 0.0f, 0.0f);
const int amountinit = 1.5f;

//Variáveis Globais
int w, h; //Largura e altura inicial da janela
int amount; //Quantidade de rotação para cada vez que pressionar a tecla
float distancia = 30.0f;  //Distancia Inicial do Objeto
GLint program, programLamp; //Referência para o programa dos shaders
GLuint VAO, lampVAO ; //Referência ao Vertex Array Object, gerenciará os vértices
GLuint VBO, lampVBO; //Referência para o Vertex Buffer Object, buffer para armazenar os vértices
GLuint EBO, lampEBO; //Referência para o Element Buffer Object, buffer interno de vértices
GLuint projectionLoc, modelLoc, viewLoc, lightColorLoc;
GLuint lampProjectionLoc, lampModelLoc, lampViewLoc, lampLightColor;

vec3 eye;
vec3 up;
vec3 target;

//vertices, normais e indices para o teapot
std::vector <glm::vec3> vertices;
std::vector <glm::vec3> normais;
std::vector <unsigned int> indices;

//vertices normais e indices para a "lampada"
std::vector <glm::vec3> vertices_lamp;
std::vector <glm::vec3> normais_lamp;
std::vector <unsigned int> indices_lamp;

// lighting
glm::vec3 lightPos(3.3f, 3.3f, 0.0f);
GLfloat lightColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

mat4 model, view, projection;
mat4 lampModel, lampView, lampProjection;

	

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

	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(lampProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}


//Função Inicial
void init() {

	//Inicializa as variáveis
	eye = eyeinit;
	up = upinit;
	amount = amountinit;
	w = 800;
	h = 600;

	//Compila e linka os shaders
	GLint vertShader	= compileShader(GL_VERTEX_SHADER, "./shader.ver");
	GLint fragShader	= compileShader(GL_FRAGMENT_SHADER, "./shader.fra");

	GLint vertShaderLamp = compileShader(GL_VERTEX_SHADER, "./shader_lamp.ver");
	GLint fragShaderLamp = compileShader(GL_FRAGMENT_SHADER, "./shader_lamp.fra");

	program				= createProgram(vertShader, fragShader);
	programLamp 		 = createProgram(vertShader, fragShader);


	// le o arquivo OBJ do teapot
	parse2("./monkeyspider.obj", vertices, normais, indices);//./TreeCartoon.obj

	//le arquivo OBJ da esfera
	parse("./sphere.obj", vertices_lamp, normais_lamp, indices_lamp);

	//Recupera a localização da transformação
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	lightColorLoc = glGetUniformLocation(program, "lightColor");

	//Recupera a localização da transformação
	lampProjectionLoc = glGetUniformLocation(programLamp, "projection");
	lampViewLoc = glGetUniformLocation(programLamp, "view");
	lampModelLoc = glGetUniformLocation(programLamp, "model");
	lampLightColor = glGetUniformLocation(programLamp, "lightC");



	//Cria um Vertex Array Object, um Vertex Buffer Object e um Element Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Cria um Vertex Array Object, um Vertex Buffer Object e um Element Buffer Object
	glGenVertexArrays(1, &lampVAO);
	glGenBuffers(1, &lampVBO);
	glGenBuffers(1, &lampEBO);

	//ativa o Vertex Array Object VAO
	glBindVertexArray(VAO);

	//Povoa o VBO com os vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	//Povoa o EBO com os indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	///////////////////////// Lamp

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

//Função de Renderização
void renderiza()
{

	// preenche buffer (pinta os pixels) associado ao fundo com branco
	//GLfloat cor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glClearBufferfv(GL_COLOR, 0, cor);
	
	// preenche buffer (pinta os pixels) associado ao fundo com branco
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// usa os shaders criados anteriormente em init()
	glUseProgram(program);
	glBindVertexArray(VAO);

	//Matriz Modelo
	model = minhaEscala(glm::mat4(1.0f), glm::vec3(0.12f, 0.12f, 0.12f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	//Matriz de Visualização
	view = myLookAt(eye, up);

	//Matriz de Projeçao
	projection = glm::perspective(glm::radians(100.f), (float)w / (float)h, 0.1f, 100.0f);

	//Passa as matrizes para o vertex shader 
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	/////////////////////////////////////////////lamp
	// usa os shaders criados anteriormente em init()

	glUseProgram(programLamp);
	glBindVertexArray(lampVAO);

	//Matriz Modelo
	lampModel = minhaEscala(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
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