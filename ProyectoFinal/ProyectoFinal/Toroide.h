#pragma once

#ifndef TOROIDE_H_
#define TOROIDE_H_

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
#include "definition.h"

class Toroide
{
public:
	Toroide(float radioToroide, float radioInterno, int resX, int resY);
	//~Toroide();

	void init();
	void load();
	void Renderizar();

private:
	std::vector<VertexColor> vertexC;
	std::vector<GLuint> index;

	float radioToroide, radioInterno;
	int resX, resY;

	GLuint VAO, VBO, EBO;
};

#endif // !TOROIDE_H_