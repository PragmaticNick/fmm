#pragma once

#include "glm/glm.hpp"

struct planet
{
	glm::dvec2 position;
	glm::dvec2 velocity;
	double mass;
	double radius;
	glm::dvec2 force;
};


inline void generate_planets(int n, double a, double b, std::vector<planet>& planets)
{
	planets.resize(n * n);
	double h = (b - a) / n;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			planets[i * n + j].position.x = a + i * h;
			planets[i * n + j].position.y = a + j * h;
			planets[i * n + j].mass = 1.0;
		}
	}
}