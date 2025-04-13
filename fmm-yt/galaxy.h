#pragma once

#include <glm/glm.hpp>

struct galaxy_config {
	glm::dvec2 center;
	double radius;
	int star_count;
	double core_mass;

	int arm_count;
	double arm_rotation_factor;
	double arm_max_offset;

	double star_mass;
	double star_radius;
};

void generate_galaxy(galaxy_config& config, std::vector<planet>& output)
{
	planet core = {};
	core.position = config.center;
	core.mass = config.core_mass;
	core.radius = config.star_radius * 3;
	output.push_back(core);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0.0, 1.0);

	double arm_angle = 2.0 * PI / config.arm_count;


	for (int i = 0; i < config.star_count; i++)
	{
		double r = dist(gen);
		double phi = 2.0 * PI * dist(gen);
		double arm_offset = config.arm_max_offset * (dist(gen) - 0.5) / r;

		double squared_arm_offset = arm_offset * arm_offset;
		if (arm_offset < 0) squared_arm_offset *= -1;

		phi = (int)(phi / arm_angle) * arm_angle + r * config.arm_rotation_factor + squared_arm_offset;

		r = r * config.radius;
		double x = config.center.x + r * cos(phi);
		double y = config.center.y + r * sin(phi);

		planet p = {};
		p.position = { x, y };
		p.mass = config.star_mass;
		p.radius = config.star_radius;

		output.push_back(p);
	}
}