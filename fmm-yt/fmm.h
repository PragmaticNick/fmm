#pragma once

#include <vector>

#include "constants.h"
#include "multipole.h"
#include "planet.h"
#include "tree.h"

void p2p(std::vector<planet>& planets);
void p2p(std::vector<planet>& sources, planet& t);

void assemble_multipoles(tree* t);
void dual_tree_traversal(tree* t);