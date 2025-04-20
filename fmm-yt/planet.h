#pragma once

#include <random>

#include <glm/glm.hpp>
#include <raylib.h>

struct planet
{
	glm::dvec2 position;
	glm::dvec2 velocity;
	double mass;
	double radius;
	glm::dvec2 force;

	Color color;
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

inline void random_planets(int n, double a, double b, std::vector<planet>& planets)
{
	std::uniform_real_distribution<double> uniform(0.0, 1.0);
	std::default_random_engine engine;

	planets.resize(n);
	for (int i = 0; i < n; i++)
	{
		planets[i].position.x = a + uniform(engine) * (b - a);
		planets[i].position.y = a + uniform(engine) * (b - a);
		planets[i].mass = 1.0;
	}
}