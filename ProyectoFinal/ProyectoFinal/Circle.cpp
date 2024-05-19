#include "Circle.h"

Circle::Circle(float radio, float x0, float y0, float z0) :
	radio(radio), x0(x0), y0(y0), z0(z0), VAO(0), VBO(0), EBO(0) {

}

void Circle::setPixel(float x, float y, float z) {
	// Agregar las coordenadas y los cinco valores 0.0 al vector
	vertex.push_back(static_cast<GLfloat>(x));
	vertex.push_back(static_cast<GLfloat>(y));
	vertex.push_back(static_cast<GLfloat>(z));
	for (int i = 0; i < 5; ++i) {
		vertex.push_back(1.0f);
	}
}

void Circle::init() {
	float x = radio;
	float y = 0;
	float error = 0;

	while (x >= y) {
		setPixel((x0 + x), (y0 + y), z0);
		setPixel((x0 + y), (y0 + x), z0);
		setPixel((x0 - y), (y0 + x), z0);
		setPixel((x0 - x), (y0 + y), z0);
		setPixel((x0 - x), (y0 - y), z0);
		setPixel((x0 - y), (y0 - x), z0);
		setPixel((x0 + y), (y0 - x), z0);
		setPixel((x0 + x), (y0 - y), z0);

		if (error <= 0) {
			y += 0.01;
			error += 2 * y + 1;
		}

		if (error > 0) {
			x -= 0.01;
			error -= 2 * x + 1;
		}
	}

	index.resize(vertex.size() * 2 + 2);
	for (int i = 0; i <= vertex.size(); i++)
	{
		/*
		if (i == vertex.size())
		{
			index[i] = i;
			index[i + 1] = 0;
		}
		else
		{
			index[i] = i;
			index[i + 1] = i + 1;
		}*/
		index[i] = i;
	}
}


void Circle::load() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	size_t stride;

	size_t offset1 = 0;
	size_t offset2 = 3;
	size_t offset3 = 5;

	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat),
		vertex.data(),
		GL_STATIC_DRAW);
	stride = sizeof(vertex[0]);
	offset1 = 0;

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
		index.data(),
		GL_STATIC_DRAW);

	// First attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * 8, (GLvoid*)offset1);
	glEnableVertexAttribArray(0);
	// Second attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * 8, (GLvoid*)offset2);
	glEnableVertexAttribArray(1);
	// Thrid attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride * 8, (GLvoid*)offset3);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

}

void Circle::Renderizar()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_POINTS, vertex.size(), GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * 0));
	glBindVertexArray(0);
}