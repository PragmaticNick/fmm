#include <iostream>
#include <iomanip>
#include <vector>

#include <glm/glm.hpp>

#include "planet.h"
#include "fmm.h"
#include "tree.h"

double error(std::vector<planet>& a, std::vector<planet>& b)
{
	int N = a.size();
	double error = 0.0;

	for (int i = 0; i < N; i++)
		error += distance(a[i].force, b[i].force) / (dot(a[i].force, a[i].force));

	return error;
}

int main()
{
	std::vector<planet> p2p_planets;
	generate_planets(5, 0.0, 2.0, p2p_planets);
	p2p(p2p_planets);

	std::vector<planet> fmm_planets;
	generate_planets(5, 0.0, 2.0, fmm_planets);

	tree* t = new tree(fmm_planets);
	assemble_multipoles(t);
	dual_tree_traversal(t);

	double e = error(p2p_planets, t->root->planets);

	std::cout << std::setprecision(15) << "Error: " << e << std::endl;

	return 0;
}