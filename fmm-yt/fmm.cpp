#include "fmm.h"

#include "constants.h"

void p2p(std::vector<planet>& planets)
{
	size_t n = planets.size();
	for (int t = 0; t < n; t++)
	{
		planets[t].force = { 0.0, 0.0 };
		for (int s = 0; s < n; s++)
		{
			glm::dvec2 forceDir = glm::normalize(planets[s].position - planets[t].position);
			double r = glm::distance(planets[t].position, planets[s].position);
			if (r > Eps)
				planets[t].force += forceDir * G * planets[s].mass * planets[t].mass / (r * r);
		}
	}
}

void p2p(std::vector<planet>& sources, planet& t)
{
	size_t n = sources.size();
	t.force = { 0.0, 0.0 };
	for (auto& s : sources)
	{
		glm::dvec2 forceDir = glm::normalize(s.position - t.position);
		double r = glm::distance(t.position, s.position);
		if (r > Eps)
			t.force += forceDir * G * s.mass * t.mass / (r * r);
	}
}