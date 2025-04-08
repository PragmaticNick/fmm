#pragma once

#include <vector>

#include "constants.h"
#include "multipole.h"
#include "planet.h"
#include "tree.h"

void p2p(std::vector<planet>& planets);
void p2p(std::vector<planet>& sources, planet& t);
glm::dvec2 p2p(planet& s, planet& t);

void assemble_multipoles(tree* t);
void dual_tree_traversal(tree* t, bool m2l);
void downward_pass(tree* t);