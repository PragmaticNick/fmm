#include "fmm.h"

#include "constants.h"

#include <iostream>

void p2p(std::vector<planet>& planets)
{
	size_t n = planets.size();
	for (int t = 0; t < n; t++)
	{
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

void assemble_multipoles(node* node)
{
    if (node->is_leaf())
    {
        for (auto& p : node->planets)
            node->m->add(p.position, p.mass);

        return;
    }

    assemble_multipoles(node->left);
    assemble_multipoles(node->right);

    node->m->add(node->left->m);
    node->m->add(node->right->m);
}

void assemble_multipoles(tree* t)
{
    assemble_multipoles(t->root);
}

void dual_tree_traversal(node* source, node* target)
{
    if (source->is_leaf() && target->is_leaf())
    {
        for (auto& t : target->planets)
            for (auto& s : source->planets)
            {
                glm::dvec2 forceDir = glm::normalize(s.position - t.position);
                double r = glm::distance(t.position, s.position);
                if (r > Eps)
                    t.force += forceDir * G * s.mass * t.mass / (r * r);
            }

        return;
    }

    area* source_box = source->node_area;
    area* target_box = target->node_area;
    bool admissible = source_box->far_from(target_box);
    if (admissible)
    {
        for (auto& t : target->planets)
            t.force += source->m->calc(t.position, t.mass);

        return;
    }

    if (source->is_leaf())
    {
        dual_tree_traversal(source, target->left);
        dual_tree_traversal(source, target->right);
        return;
    }

    if (target->is_leaf())
    {
        dual_tree_traversal(source->left, target);
        dual_tree_traversal(source->right, target);
        return;
    }

    if (target_box->radius > source_box->radius)
    {
        dual_tree_traversal(source, target->left);
        dual_tree_traversal(source, target->right);
    }
    else
    {
        dual_tree_traversal(source->left, target);
        dual_tree_traversal(source->right, target);
    }
}

void dual_tree_traversal(tree* t)
{
    dual_tree_traversal(t->root, t->root);
}