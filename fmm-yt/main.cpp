#include <iostream>
#include <vector>
#include <math.h>

const double Eps = 1.0e-8;
const double G = 6.6743 * 1.0e-11;

struct planet
{
	double x, y;
	double mass;
	double force;
};


double distance(planet& a, planet& b)
{
	return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void generate_planets(int n, double a, double b, std::vector<planet>& planets)
{
	planets.resize(n * n);
	double h = (b - a) / n;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			planets[i * n + j].x = a + i * h;
			planets[i * n + j].y = a + j * h;
			planets[i * n + j].mass = 1.0;
		}
	}
}

void p2p(std::vector<planet>& planets)
{
	size_t n = planets.size();

	for (int t = 0; t < n; t++)
	{
		planets[t].force = 0.0;
		for (int s = 0; s < n; s++)
		{
			double r = distance(planets[t], planets[s]);
			if (r > Eps)
				planets[t].force += G * planets[s].mass * planets[t].mass / (r * r);
		}
	}
}

int main()
{
	std::vector<planet> planets;
	generate_planets(10, 0.0, 1.0, planets);
	p2p(planets);

	for (auto& planet : planets)
		std::cout << planet.force << std::endl;

	return 0;
}