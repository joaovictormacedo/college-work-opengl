#include "transformacoes.h"

using namespace  std;

mat4 minhaEscala(mat4 mIndent, vec3 coordenadas) {
	mat4 result;
	result[0] = mIndent[0] * coordenadas[0];
	result[1] = mIndent[1] * coordenadas[1];
	result[2] = mIndent[2] * coordenadas[2];
	result[3] = mIndent[3];
	return result;
}

// function to display 4x4 matrix
void imrimeMatriz3x3(string titulo, glm::mat3 m4)
{
	printf("%s\n", titulo.c_str());
	for (int col = 0; col < 3; ++col) {
		cout << "| ";
		for (int row = 0; row < 3; ++row) {
			cout << m4[row][col] << '\t';
		}
		cout << '\n';
	}
	cout << '\n';
}


// funcao de rotacao.  
mat3 rotaciona(const float degree, const glm::vec3& axis) {

	//Normalizando o vetor
	glm::vec3 axisNormalizado = glm::normalize(axis);


	//Recuperando os elementos do vetor e armazenando e variaveis separadas
	float x = axisNormalizado.x;
	float y = axisNormalizado.y;
	float z = axisNormalizado.z;

	//Convertendo o angulo de graus para radianos usando a formula  x° = pi/180
	float anguloRadianos = degree * (pi / 180.0f);


	//Definindo os senos e cossenos utilizados na matriz de rotação
	float seno = sin(anguloRadianos);
	float cosseno = cos(anguloRadianos);


	//Definindo a matriz de translação
	//glm::mat3 matrizTranslacao = glm::mat3(1,0, -x);


	/*Rotacionando torno do eixo z
	glm::mat3 rotacaoZ = glm::mat3(cosseno*x - seno*y, 0, 0, 0, seno*x + cosseno * y, 0, 0, 0, z);
	

	//Rotacionando em torno do eixo y 
	glm::mat3 rotacaoY = glm::mat3(cosseno*x + seno * z, 0, 0, 0, y, 0, 0, 0, (1 - seno)*x + cosseno * y);


	//Rotacionando em torno do eixo X
	glm::mat3 rotacaoX = glm::mat3(x, 0, 0, 0, cosseno*y - seno * z, 0, 0, 0, seno*y + cosseno * z);


	//Calculando o produto da rotação dos 3 eixos obtemos 
	glm::mat3 rotacaoFinal = rotacaoX * rotacaoY * rotacaoZ;*/

	


	//Calculando a matriz final de rotação a partir do vetor axis normalizado 
	glm::mat3 rotacaoXYZ = glm::mat3(cosseno + pow(x, 2)*(1 - cosseno), y*x*(1 - cosseno) + z * seno, z*x*(1 - cosseno) - y * seno,
									  x*y*(1-cosseno) - z*seno, cosseno + pow(y, 2)*(1-cosseno), z*y*(1-cosseno) + x*seno,
									  x*z*(1-cosseno) + y*seno, y*z*(1-cosseno) - x*seno, cosseno + pow(z, 2)*(1-cosseno)
									);



	imrimeMatriz3x3("MatrizXYZ", rotacaoXYZ);

	
	
	return rotacaoXYZ;									
}






// rotaciona a camera pela esquerda da "bola de cristal"
void esquerda(float degrees, glm::vec3& eye, glm::vec3& up) {
	eye = eye * rotaciona(degrees, up);
}

// rotaciona a camera por cima da "bola de cristal"
void paraCima(float degrees, glm::vec3& eye, glm::vec3& up) {

	//Calcula o produto vetorial 
	glm::vec3 axis = glm::cross(eye, up);


	glm::mat3 rotacionaMatriz = rotaciona(degrees, axis);

	eye = eye * rotacionaMatriz;
	up = up * rotacionaMatriz;
}


// sua implementacao de glm::lookAt
// sempre olhando para a origem
/*glm::mat4 myLookAt(glm::vec3 eye, glm::vec3 up) {
glm::vec3 w = glm::normalize(eye);
up = glm::normalize(up);

//glm::vec3 u = glm::normalize(glm::cross(up, w));
glm::vec3 u = glm::cross(up, w);
glm::vec3 v = glm::normalize(glm::cross(w, u));
glm::mat4 transformM =
glm::mat4(
u.x, u.y, u.z, -glm::dot(u, eye),
v.x, v.y, v.z, -glm::dot(v, eye),
w.x, w.y, w.z, -glm::dot(w, eye),
0.0f, 0.0f, 0.0f, 1.0f
);

return transformM;
}*/

// https://stackoverflow.com/questions/19740463/lookat-function-im-going-crazy
// http://www.songho.ca/opengl/gl_transform.html


glm::mat4 myLookAt(glm::vec3 eye, glm::vec3 up) {
	


	glm::vec3 w = glm::normalize(eye); //Direção que está olhando
	glm::vec3 u = glm::normalize(up); //Para cima
	glm::vec3 v = glm::normalize(glm::cross(w, u)); // vetor certo

	u = glm::cross(up, w);

	glm::mat4 rotation = glm::mat4(
		u.x, u.y, u.z, 0,
		v.x, v.y, v.z, 0,
		w.x, w.y, w.z, 0,
		0, 0, 0, 1);
	rotation = glm::transpose(rotation);

	glm::mat4 translation = glm::mat4(
		1, 0, 0, -eye.x,
		0, 1, 0, -eye.y,
		0, 0, 1, -eye.z,
		0, 0, 0, 1);
	translation = glm::transpose(translation);

	glm::mat4 transformation = rotation * translation;

	return transformation;
}