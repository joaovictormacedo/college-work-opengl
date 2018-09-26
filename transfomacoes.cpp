#include "transformacoes.h"

using namespace  std;

//Local Space -> Model Matriz
//World Space -> View Matriz
//View Space -> Projection Matrix
//Clip Space -> ViewPort -> Screen 


//Função Escala
mat4 minhaEscala(mat4 mIndent, vec3 coordenadas) {
	mat4 result;
	result[0] = mIndent[0] * coordenadas[0];
	result[1] = mIndent[1] * coordenadas[1];
	result[2] = mIndent[2] * coordenadas[2];
	result[3] = mIndent[3];
	return result;
}

// function to display 4x4 matrix
void imrimeMatriz3x3(string titulo, mat3 m4)
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
	vec3 axisNormalizado = normalize(axis);

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
	mat3 rotacaoXYZ =mat3(cosseno + pow(x, 2)*(1 - cosseno), y*x*(1 - cosseno) + z * seno, z*x*(1 - cosseno) - y * seno,
						  x*y*(1-cosseno) - z*seno, cosseno + pow(y, 2)*(1-cosseno), z*y*(1-cosseno) + x*seno,
						  x*z*(1-cosseno) + y*seno, y*z*(1-cosseno) - x*seno, cosseno + pow(z, 2)*(1-cosseno)
				        	);

	//imrimeMatriz3x3("MatrizXYZ", rotacaoXYZ);

	return rotacaoXYZ;									
}


// rotaciona a camera pela esquerda da "bola de cristal"
void esquerda(float degrees, vec3& eye, vec3& up) {
	eye = eye * rotaciona(degrees, up);
}


// rotaciona a camera por cima da "bola de cristal"
void paraCima(float degrees, vec3& eye, vec3& up) {

	//Calcula o produto vetorial 
	vec3 axis = prodVetorial(up, eye);
	mat3 rotacionaMatriz = rotaciona(degrees, axis);

	eye = eye * rotacionaMatriz;
	up = up * rotacionaMatriz;
}


// Função que calcula o produto vetorial entre dois vetores
vec3 prodVetorial(vec3 vet1, vec3 vet2 ) {
	
	vec3 produto = vec3(vet1.y * vet2.z - vet2.y * vet1.z,
						vet1.z * vet2.x - vet2.z * vet1.x, 
						vet1.x * vet2.y - vet2.x * vet1.y);
	return produto;
}

// Função de Normalização 
vec3 normaliza(vec3 vetor) {

	//"a" ao quadrado, mais "b" ao quadrado é igual a "c" ao quadrado.

	float magVetor = glm::sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + pow(vetor.z, 2));

	vec3 normalizado = vec3(vetor.x / magVetor, vetor.y / magVetor, vetor.z / magVetor);

	return normalizado;

}

//Função de Translação 
mat4 translado(vec3 vetor) {
	return mat4(1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				vetor.x, vetor.y, vetor.z, 1
				);
}

// Função que realiza as transformações de câmera, cria uma matriz de visualização que olha para um determinado alvo. 
mat4 myLookAt(vec3 eye, vec3 up) {
	
	//DOT - produto escalar
	//Cross - produto vetorial
	//Normalize - Normalização

	/*	Posição do Olho: e, eye
		Direção do Olhar: g = o - e, "o" centro de observação
		Orientação da Camêra: t
	*/

	//Sistema de Coordenadas da camêra com origem em e - Aula3:8
	vec3 D = normaliza(eye); //Vetor Direção
	vec3 R = normaliza(prodVetorial(up, D));  //Vetor Right
	vec3 U = normaliza(prodVetorial(D, R)); //Vetor UP


	//Rotaciona o sistema de cordenadas do mundo para para que ele se alinhe com o SC da câmera
	mat4 matrizRotacao = mat4(R.x, U.x, D.x, 0,
						      R.y, U.y, D.y, 0,
							  R.z, U.z, D.z, 0,
							   0, 0, 0, 1);

	//Realiza a translação da coordenada do olho para a origem 
	mat4 matrizTranslacao = mat4(1, 0, 0, 0,
							     0, 1, 0, 0,
								 0, 0, 1, 0,
								-eye.x, -eye.y, -eye.z, 1); 
	
	//Matriz que transforma os pontos do objeto e posiçiona de modo
	//que observamos a origem do SC Mundo, como se estivessemos posicionados no SC camera
	mat4 mCam = matrizRotacao * matrizTranslacao;


	return mCam;
}