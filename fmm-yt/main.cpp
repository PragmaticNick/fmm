#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

#include <glm/glm.hpp>

#include "planet.h"
#include "fmm.h"
#include "tree.h"
#include "local.h"

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
	std::vector<planet> planets1, planets2, planets3, targets;
	random_planets(100, 0.0, 1.0, planets1);
	random_planets(100, 1.0, 2.0, planets2);
	random_planets(100, 2.0, 3.0, planets3);
	random_planets(100, 103.0, 104.0, targets);

	auto m1 = new multipole({ 0.5, 0.5 });
	auto m2 = new multipole({ 1.5, 1.5 });
	auto m3 = new multipole({ 2.5, 2.5 });
	for (int i = 0; i < 100; i++) {
		m1->add(planets1[i].position, planets1[i].mass);
		m2->add(planets2[i].position, planets2[i].mass);
		m3->add(planets3[i].position, planets3[i].mass);
	}

	auto l = new local({ 103.5, 103.5 });
	l->add(*m1);
	l->add(*m2);
	l->add(*m3);

	std::vector<glm::dvec2> fmm_forces(100);
	for (int i = 0; i < 100; i++)
		fmm_forces[i] = l->calc(targets[i].position, targets[i].mass);

	planets1.insert(planets1.end(), planets2.begin(), planets2.end());
	planets1.insert(planets1.end(), planets3.begin(), planets3.end());
	std::vector<glm::dvec2> p2p_forces(100);
	for (int i = 0; i < 100; i++)
	{
		p2p(planets1, targets[i]);
		p2p_forces[i] = targets[i].force;
	}

	std::cout << "Error: " << error(p2p_forces, fmm_forces) << std::endl;

	return 0;
}