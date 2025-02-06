#pragma once

#include <vector>
#include <algorithm>

#include "constants.h"
#include "multipole.h"
#include "planet.h"


struct area
{
	glm::dvec2 center;
	double radius;
	area(std::vector<planet>& planets)
	{
		glm::dvec2 min = planets[0].position;
		glm::dvec2 max = planets[0].position;
		for (auto& p : planets)
		{
			max.x = p.position.x > max.x ? p.position.x : max.x;
			max.y = p.position.y > max.y ? p.position.y : max.y;
			min.x = p.position.x < min.x ? p.position.x : min.x;
			min.y = p.position.y < min.y ? p.position.y : min.y;
		}

		center = (min + max) / 2.0;
		radius = glm::distance(min, max) / 2.0;
	}

	bool far_from(area* other)
	{
		return 2.0 * std::max(radius, other->radius) / glm::distance(center, other->center) < Theta;
	}
};

struct node
{
	node* left;
	node* right;

	area* node_area;
	std::vector<planet> planets;
	multipole* m;

	node(node* left, node* right, area* node_area, std::vector<planet> planets) :
		left(left), 
		right(right), 
		node_area(node_area),
		planets(planets), 
		m(new multipole(node_area->center))
	{}

	bool is_leaf()
	{
		return left == nullptr && right == nullptr;
	}
};

struct tree
{
	node* root;
	int depth;

	tree(std::vector<planet>& planets)
	{
		root = fill_tree(0, planets);
	}

	node* fill_tree()
	{
	}

	node *fill_tree(int depth, std::vector<planet>& planets)
	{
		area* node_area = new area(planets);
		if (planets.size() <= N0)
		{
			this->depth = depth;
			return new node(nullptr, nullptr, node_area, planets);
		}

		int k = 2;
		int axis = depth % k;
		std::sort(planets.begin(), planets.end(), [&](const planet& a, const planet& b) 
			{ return a.position[axis] < b.position[axis]; });
		
		int split = planets.size() / 2;
		std::vector<planet> left_particles(planets.begin(), planets.begin() + split);
		std::vector<planet> right_particles(planets.begin() + split, planets.end());

		node* left = fill_tree(depth + 1, left_particles);
		node* right = fill_tree(depth + 1, right_particles);
		return new node(left, right, node_area, planets);
	}
};
