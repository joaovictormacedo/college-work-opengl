#include "transformacoes.h"


mat4 minhaEscala(mat4 mIndent, vec3 coordenadas) {
	mat4 result;
	result[0] = mIndent[0] * coordenadas[0];
	result[1] = mIndent[1] * coordenadas[1];
	result[2] = mIndent[2] * coordenadas[2];
	result[3] = mIndent[3];
	return result;
}


// funcao de rotacao.  
mat3 rotaciona(const float degree, const glm::vec3& axis) {
	// implemente usando a forma eixo-angulo apresentada em sala
	glm::vec3 a = glm::normalize(axis);
	float sine = sin(glm::radians(degree));
	float cosine = cos(glm::radians(degree));
	float x = a.x, y = a.y, z = a.z;

	glm::mat3 aat = glm::mat3(x*x, y*x, z*x,
		x*y, y*y, z*y,
		x*z, y*z, z*z);

	glm::mat3 aStar = glm::mat3(0, -z, y,
		z, 0, -x,
		-y, x, 0);

	glm::mat3 rotateM = glm::mat3(cosine) + (1 - cosine) * aat + sine * aStar;

	return rotateM;
}






// rotaciona a camera pela esquerda da "bola de cristal"
void esquerda(float degrees, glm::vec3& eye, glm::vec3& up) {
	eye = eye * rotaciona(degrees, up);
}

// rotaciona a camera por cima da "bola de cristal"
void paraCima(float degrees, glm::vec3& eye, glm::vec3& up) {
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

glm::mat4 myLookAt(glm::vec3 eye, glm::vec3 up) {
	glm::vec3 w = glm::normalize(eye);
	up = glm::normalize(up);
	glm::vec3 u = glm::cross(up, w);
	glm::vec3 v = glm::normalize(glm::cross(w, u));

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