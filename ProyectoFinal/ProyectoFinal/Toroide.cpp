#include "Toroide.h"

Toroide::Toroide(float radioToroide, float radioInterno, int resX, int resY) :
	radioToroide(radioToroide), radioInterno(radioInterno), resX(resX), resY(resY), VAO(0), VBO(0), EBO(0) {
}

void Toroide::init()
{
	float x, y, z; //Coordenadas de los vértices
	float U, V, theta, phi; //Parámetros necesarios
	U = 2 * M_PI / resX;
	V = 2 * M_PI / resY;

	vertexC.resize((resX + 1) * (resY + 1));
	index.resize((resX * resY + resX) * 6);

	for (int i = 0; i <= resY; i++)
	{
		phi = V * i;
		for (int j = 0; j <= resX; j++)
		{
			theta = U * j;

			x = (radioToroide + radioInterno * cos(phi)) * cos(theta);
			y = (radioToroide + radioInterno * cos(phi)) * sin(theta);
			z = radioInterno * sin(phi);

			vertexC[i * (resX + 1) + j].position = glm::vec3(x, y, z);
			vertexC[i * (resX + 1) + j].color = glm::sphericalRand(1.0);
		}
	}

	for (int i = 0; i < resX * resY + resX; ++i) {
		index[i * 6] = i;
		index[i * 6 + 1] = i + resX + 1;
		index[i * 6 + 2] = i + resX;
		index[i * 6 + 3] = i + resX + 1;
		index[i * 6 + 4] = i;
		index[i * 6 + 5] = i + 1;
	}
}

void Toroide::load() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	size_t stride;

	size_t offset1 = 0;
	size_t offset2 = 0;
	size_t offset3 = 0;

	glBufferData(GL_ARRAY_BUFFER, vertexC.size() * sizeof(glm::vec3) * 2,
		vertexC.data(),
		GL_STATIC_DRAW);
	stride = sizeof(vertexC[0]);
	offset1 = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
		index.data(),
		GL_STATIC_DRAW);

	// First attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset1);
	glEnableVertexAttribArray(0);
	// Second attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
		(GLvoid*)offset2);
	glEnableVertexAttribArray(1);
	// Thrid attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset3);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

}

void Toroide::Renderizar()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * 0));
	glBindVertexArray(0);
}