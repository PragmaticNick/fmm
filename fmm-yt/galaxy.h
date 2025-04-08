#pragma once

#include <glm/glm.hpp>

struct circle_info
{
	double r;
	int count;
	double planet_radius;
};

void generate_circle(int n, double r, std::vector<planet>& output)
{
	double phi_step = 2.0 * PI / n;

	for (int i = 0; i < n; i++)
	{
		double phi = i * phi_step;
		planet p = {};
		p.position.x = r * cos(phi);
		p.position.y = r * sin(phi);
		p.mass = 1.0e+5;
		p.radius = 3.0;
		p.velocity = -glm::normalize(glm::dvec2(p.position.y, -p.position.x));
		p.velocity *= 80;
		output.push_back(p);
	}
}

void generate_galaxy(std::vector<planet>& output)
{
	planet center = {};
	center.position = { 0.0, 0.0 };
	center.velocity = { 0.0, 0.0 };
	center.radius = 40.0;
	center.mass = 1e+16;
	output.push_back(center);

	int circle_count = 20;

	double r_initial = 90.0;
	double r_step = 2.0;

	for (int i = 0; i < circle_count; i++)
	{
		int count = 5 + i * 5;
		double r = r_initial + r_step * i;

		generate_circle(count, r, output);
	}


}