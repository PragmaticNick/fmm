#include "fmm.h"

#include "constants.h"

#include <iostream>

void p2p(std::vector<planet>& planets)
{
	size_t n = planets.size();
	for (auto& t : planets)
	{
		for (auto& s : planets)
		{
			glm::dvec2 forceDir = glm::normalize(s.position - t.position);
			double r = glm::distance(s.position, t.position);
            if (r > Eps)
                 t.force += forceDir * G * s.mass * t.mass / (r * r);
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
        for (auto i : node->planet_indices)
        {
            auto& p = node->planets[i];
            node->m->add(p.position, p.mass);
        }

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

void dual_tree_traversal(node* source, node* target, bool m2l)
{
    if (source->is_leaf() && target->is_leaf())
    {
        for (auto ti : target->planet_indices)   
            for (auto si : source->planet_indices)
            {
                auto& t = target->planets[ti];
                auto& s = source->planets[si];
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
        if (m2l) {
            target->l->add(source->m);
            return;
        }

        for (auto ti : target->planet_indices)
        {
            auto& t = target->planets[ti];
            t.force += source->m->calc(t.position, t.mass);
        }
        return;
    }

    if (source->is_leaf())
    {
        dual_tree_traversal(source, target->left, m2l);
        dual_tree_traversal(source, target->right, m2l);
        return;
    }

    if (target->is_leaf())
    {
        dual_tree_traversal(source->left, target, m2l);
        dual_tree_traversal(source->right, target, m2l);
        return;
    }

    if (target_box->radius > source_box->radius)
    {
        dual_tree_traversal(source, target->left, m2l);
        dual_tree_traversal(source, target->right, m2l);
    }
    else
    {
        dual_tree_traversal(source->left, target, m2l);
        dual_tree_traversal(source->right, target, m2l);
    }
}

void dual_tree_traversal(tree* t, bool m2l)
{
    dual_tree_traversal(t->root, t->root, m2l);
}

void downward_pass(node* node)
{
    if (node->is_leaf())
    {
        for (auto i : node->planet_indices)
        {
            auto& p = node->planets[i];
            p.force += node->l->calc(p.position, p.mass);
        }
        return;
    }

    node->left->l->add(node->l);
    node->right->l->add(node->l);

    downward_pass(node->left);
    downward_pass(node->right);
}


void downward_pass(tree* t)
{
    downward_pass(t->root);
}
