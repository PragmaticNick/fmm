#include <iostream>
#include <iomanip>
#include <vector>

#include <glm/glm.hpp>

#include "planet.h"
#include "fmm.h"

int main()
{
	std::vector<planet> planets;
	generate_planets(10, 1000999, 1100999, planets);

	planet target = {};
	target.position = { 50.0, 0.0 };
	target.mass = 10.0;

	p2p(planets, target);

	std::cout << std::setprecision(15) << "(" << target.force.x << ", " << target.force.y << ")" << std::endl;

	glm::dvec2 center = { 0.0, 0.0 };
	for (auto& planet : planets)
		center += planet.position;

	center /= planets.size();
	multipole m(center);

	for (int i = 0; i < 100; i++)
		m.add(planets[i].position, planets[i].mass);

	auto f = m.calc(target.position, target.mass);
	std::cout << "(" << f.x << ", " << f.y << ")" << std::endl;
	return 0;
}