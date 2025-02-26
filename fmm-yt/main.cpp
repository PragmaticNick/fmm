#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

#include <glm/glm.hpp>

#include "planet.h"
#include "fmm.h"
#include "tree.h"

double error(std::vector<glm::dvec2>& a, std::vector<glm::dvec2>& b)
{
	int N = a.size();
	double error = 0.0;

	for (int i = 0; i < N; i++)
		error += distance(a[i], b[i]) / glm::length(a[i]);

	return error / N * 100;
}

std::vector<glm::dvec2> direct(std::vector<planet>& planets)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	p2p(planets);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "P2P      = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

	std::vector<glm::dvec2> forces;
	for (auto& p : planets)
		forces.push_back(p.force);

	return forces;

}

std::vector<glm::dvec2> treecode(std::vector<planet>& planets)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	tree* t = new tree(planets);
	assemble_multipoles(t);
	dual_tree_traversal(t);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "TREECODE = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

	std::vector<glm::dvec2> forces;
	for (auto& p : planets)
		forces.push_back(p.force);

	return forces;
}

int main()
{
	for (int n = 10; n <= 100000; n *= 10)
	{
		std::cout << "Planet count: " << n << std::endl;
		std::vector<planet> planets;
		random_planets(n, 0.0, 1.0, planets);

		auto p2p_forces = direct(planets);
		for (auto& p : planets)
			p.force = { 0.0, 0.0 };
		auto fmm_forces = treecode(planets);

		double e = error(p2p_forces, fmm_forces);
		std::cout << std::setprecision(5) << "Error: " << e << "%" << std::endl;
		std::cout << std::endl;
	}

	return 0;
}