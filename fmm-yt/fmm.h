#pragma once

#include <vector>

#include "constants.h"
#include "planet.h"

struct multipole
{
	glm::dvec2 center = { 0.0, 0.0 };
	double values[6] = { };

	multipole(glm::dvec2 center) : center(center) { }

	void add(glm::dvec2 pos, double mass)
	{
		auto dpos = pos - center;

		values[0] += mass;
		values[1] += dpos.x * mass;
		values[2] += dpos.y * mass;
		values[3] += dpos.x * dpos.x * mass / 2;
		values[4] += dpos.x * dpos.y * mass;
		values[5] += dpos.y * dpos.y * mass / 2;
	}

	glm::dvec2 calc(glm::dvec2 target_pos, double mt)
	{
		auto dpos = target_pos - center;
		glm::dvec2 result = { 0.0, 0.0 };

		double a = dpos.x;
		double b = dpos.y;

		double d = glm::length(dpos);
		double d3 = d * d * d;
		double d5 = d3 * d * d;
		double d7 = d5 * d * d;

		result.x += values[0] * (-a) / d3;
		result.x += values[1] * (b * b - 2 * a * a) / d5;
		result.x += values[2] * -3 * a * b / d5;
		result.x += values[3] * (9 * a * b * b - 6 * a * a * a) / d7;
		result.x += values[4] * (3 * b * b - 12 * a * a * b) / d7;
		result.x += values[5] * (3 * a * a * a - 12 * a * b * b) / d7;

		result.y += values[0] * (-b) / d3;
		result.y += values[1] * -3 * a * b / d5;
		result.y += values[2] * (a * a - 2 * b * b) / d5;
		result.y += values[3] * (3 * b * b * b - 12 * a * b * b) / d7;
		result.y += values[4] * (3 * a * a * a - 12 * a * b * b) / d7;
		result.y += values[5] * (9 * a * a * b - 6 * b * b * b) / d7;

		return G * mt * result;
	}
};

void p2p(std::vector<planet>& planets);
void p2p(std::vector<planet>& sources, planet& t);
