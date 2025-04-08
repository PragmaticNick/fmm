#pragma once

#include <vector>
#include <algorithm>
#include <numeric>

#include "constants.h"
#include "multipole.h"
#include "local.h"
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
	std::vector<planet>& planets;
	std::vector<int> planet_indices;
	multipole* m;
	local* l;

	node(node* left, node* right, area* node_area, std::vector<planet>& planets, std::vector<int> planet_indices) :
		left(left), 
		right(right), 
		node_area(node_area),
		planets(planets),
		planet_indices(planet_indices),
		m(new multipole(node_area->center)),
		l(new local(node_area->center))
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

	std::vector<planet>& planets;

	tree(std::vector<planet>& planets): planets(planets)
	{
		std::vector<int> indices(planets.size());
		std::iota(indices.begin(), indices.end(), 0);
		root = fill_tree(0, indices);
	}

	node *fill_tree(int depth, std::vector<int>& indices)
	{
		int size = indices.size();
		std::vector<planet> current_planets(size);
		for (int i = 0; i < size; i++)
			current_planets[i] = planets[indices[i]];

		area* node_area = new area(current_planets);
		if (size <= N0)
		{
			this->depth = depth;
			return new node(nullptr, nullptr, node_area, planets, indices);
		}

		int k = 2;
		int axis = depth % k;
		std::sort(indices.begin(), indices.end(), [&](int i, int j)
			{ return planets[i].position[axis] < planets[j].position[axis]; });
		
		int split = indices.size() / 2;
		std::vector<int> left_indices(indices.begin(), indices.begin() + split);
		std::vector<int> right_indices(indices.begin() + split, indices.end());

		node* left = fill_tree(depth + 1, left_indices);
		node* right = fill_tree(depth + 1, right_indices);
		return new node(left, right, node_area, planets, indices);
	}
};
