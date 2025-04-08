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
	dual_tree_traversal(t, false);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "TREECODE = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

	std::vector<glm::dvec2> forces;
	for (auto& p : planets)
		forces.push_back(p.force);

	return forces;
}

std::vector<glm::dvec2> fmm(std::vector<planet>& planets)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	tree* t = new tree(planets);
	assemble_multipoles(t);
	dual_tree_traversal(t, true);
	downward_pass(t);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "FMM = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

	std::vector<glm::dvec2> forces;
	for (auto& p : planets)
		forces.push_back(p.force);

	return forces;
}

void test1() {
	int n = 100;
	std::vector<planet> sources1, sources2;
	random_planets(n, 0.0, 1.0, sources1);
	random_planets(n, 4.0, 5.0, sources2);

	int m = 1000;
	std::vector<planet> targets;
	random_planets(m, 100.0, 101.0, targets);

	multipole m1({ 0.5, 0.5 });
	multipole m2({ 4.5, 4.5 });
	for (int i = 0; i < n; i++)
	{
		m1.add(sources1[i].position, sources1[i].mass);
		m2.add(sources2[i].position, sources2[i].mass);
	}

	tree t(targets);
	t.root->l->add(&m1);
	t.root->l->add(&m2);
	//t.root->left->left->l->add(&m2);
	//t.root->left->right->l->add(&m2);
	//t.root->right->left->l->add(&m2);
	//t.root->right->right->l->add(&m2);
	downward_pass(&t);

	std::vector<glm::dvec2> fmm_target_forces(m);
	for (int i = 0; i < m; i++)
		fmm_target_forces[i] = targets[i].force;

	sources1.insert(sources1.end(), sources2.begin(), sources2.end());
	for (int t = 0; t < m; t++)
		p2p(sources1, targets[t]);

	std::vector<glm::dvec2> p2p_target_forces(m);
	for (int i = 0; i < m; i++)
		p2p_target_forces[i] = targets[i].force;

	double e = error(p2p_target_forces, fmm_target_forces);
	std::cout << std::setprecision(5) << "Error: " << e << "%" << std::endl;
	std::cout << std::endl;
}

void test2() {
	int n = 4;
	double m = 100000.0;
	std::vector<planet> sources = {
		planet({-4.0, 0.0}, m),
		planet({-2.0, 0.0}, m),
		planet({2.0, 0.0}, m),
		planet({4.0, 0.0}, m),
	};

	auto p2p_forces = direct(sources);
	for (auto& p : sources) p.force = { 0.0, 0.0 };
	auto tree_forces = treecode(sources);
	for (auto& p : sources) p.force = { 0.0, 0.0 };
	auto fmm_forces = fmm(sources);

	// Error
	std::cout << std::setprecision(5) << "P2P vs TREECODE: " << error(p2p_forces, tree_forces) << "%" << std::endl;
	std::cout << std::setprecision(5) << "P2P vs FMM: " << error(p2p_forces, fmm_forces) << "%" << std::endl;
}

int main()
{
	for (int n = 100; n <= 1000000; n *= 10)
	{
		std::cout << "Planet count: " << n << std::endl;
		std::vector<planet> planets;
		random_planets(n, 0.0, 1.0, planets);

		auto p2p_forces = direct(planets);
		for (auto& p : planets) p.force = { 0.0, 0.0 };
		auto tree_forces = treecode(planets);
		for (auto& p : planets) p.force = { 0.0, 0.0 };
		auto fmm_forces = fmm(planets);

		std::cout << std::endl;
		std::cout << std::setprecision(5) << "P2P vs TREECODE: " << error(p2p_forces, tree_forces) << "%" << std::endl;
		std::cout << std::setprecision(5) << "P2P vs FMM: " << error(p2p_forces, fmm_forces) << "%" << std::endl;
		std::cout << std::endl;
	}

	return 0;
}