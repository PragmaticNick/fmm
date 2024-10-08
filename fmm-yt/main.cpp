#include <iostream>
#include <iomanip>
#include <vector>

#include "planet.h"
#include "fmm.h"

int main()
{
	std::vector<planet> planets;
	generate_planets(10, 0.0, 1.0, planets);

	planet target = {};
	target.position = { 500.0, 500.0 };
	target.mass = 10.0;

	p2p(planets, target);

	std::cout << std::setprecision(15) << "(" << target.force.x << ", " << target.force.y << ")" << std::endl;

	multipole m({ 0.25, 0.25 });

	for (int i = 0; i < 100; i++)
		m.add(planets[i].position, planets[i].mass);

	auto fx = m.calc(target.position, target.mass);
	std::cout << "(" << fx << ", " << 0.0 << ")" << std::endl;
	return 0;
}