/* Monkey */
GLuint projectionLoc_monkey, modelLoc_monkey, viewLoc_monkey, lightColorLoc_monkey;
std::vector <glm::vec3> vertices_monkey;
std::vector <glm::vec3> normais_monkey;
std::vector <unsigned int> indices_monkey;
mat4 model_monkey, view_monkey, projection_monkey;


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
