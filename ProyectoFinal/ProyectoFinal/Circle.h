#pragma once

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
#include "definition.h"

class Circle
{
public:
	Circle(float radio, float x0, float y0, float z0);
	//~Toroide();

	void init();
	void load();
	void Renderizar();

private:
	std::vector<GLfloat> vertex;
	std::vector<GLuint> index;

	void setPixel(float x, float y, float z);
	float radio, x0, y0, z0;

	GLuint VAO, VBO, EBO;
};

#endif // !TOROIDE_H_#pragma once
