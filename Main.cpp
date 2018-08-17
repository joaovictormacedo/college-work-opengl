
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
const int amountinit = 1.5f;

//Variáveis Globais
int w, h; //Largura e altura inicial da janela
int amount; //Quantidade de rotação para cada vez que pressionar a tecla
float Zoom = 45.0f;
GLint program; //Referência para o programa dos shaders
GLuint VAO; //Referência ao Vertex Array Object, gerenciará os vértices
GLuint VBO; //Referência para o Vertex Buffer Object, buffer para armazenar os vértices
GLuint EBO; //Referência para o Element Buffer Object, buffer interno de vértices
GLuint projectionLoc, modelLoc, viewLoc;
vec3 eye;
vec3 up;
mat4 model, view, projection;

	

//Callback - Eventos do Usuário
void callback_teclado(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	switch (key) {
		case GLFW_KEY_A:
			//Zoom+
			if (Zoom >= 1.0f && Zoom <= 45.0f)
				Zoom -= 1.0f;
			if (Zoom <= 1.0f)
				Zoom = 1.0f;
			if (Zoom >= 45.0f)
				Zoom = 45.0f;
			break;
		case GLFW_KEY_D:
			//Zoom-
			if (Zoom >= 1.0f && Zoom <= 45.0f)
				Zoom += 1.0f;
			if (Zoom <= 1.0f)
				Zoom = 1.0f;
			if (Zoom >= 45.0f)
				Zoom = 45.0f;
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
			Zoom = 45.0f;
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
	
	projection = glm::perspective(glm::radians(45.0f), (float)w/(float)h, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
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
	program				= createProgram(vertShader, fragShader);

	//Carrega o Objeto
	parse("./teapot.obj");

	//Recupera a localização da transformação
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");

	//Cria um Vertex Array Object, um Vertex Buffer Object e um Element Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//ativa o Vertex Array Object VAO
	glBindVertexArray(VAO);

	//Povoa o VBO com os vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	//Povoa o EBO com os indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);


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
	GLfloat cor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, cor);

	// usa os shaders criados anteriormente em init()
	glUseProgram(program);
	glBindVertexArray(VAO);

	//monta a matriz modelo
	model = minhaEscala(glm::mat4(1.0f), glm::vec3(2.5f, 2.5f, 2.5f));

	view = myLookAt(eye, up);

	projection = glm::perspective(glm::radians(Zoom), (float)w / (float)h, 0.1f, 100.0f);

	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f));


	//Passa a matriz para o vertex shader 
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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
	glfwTerminate();

	return 0;
}