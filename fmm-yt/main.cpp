#include <string>
#include <format>

#include <raylib.h>
#include <raymath.h>

#include "fmm.h"
#include "constants.h"

#include "galaxy.h"

const Color background_color = { 50, 52, 55, 255 };
const Color planet_color = { 235, 231, 205, 255 };

int width = 1920;
int height = 1080;
int frame_count = 1;

struct state
{
	std::vector<planet> planets;
	Camera2D camera;
};

void update(state& state, double dt)
{
	double step = 0.0001;
	double sum = 0.0;
	while (sum < dt)
	{
		for (auto& p : state.planets)
			p.force = { 0.0, 0.0 };

		tree t(state.planets);
		assemble_multipoles(&t);
		dual_tree_traversal(&t, true);
		downward_pass(&t);

		size_t size = state.planets.size();
		for (int i = 0; i < size; i++)
		{
			planet& p = state.planets[i];
			auto ai = p.force / p.mass;
			auto vi = p.velocity;
			p.velocity += ai * step;
			p.position += vi * step;
		}

		sum += step;
	}
}

void draw(state& state, int frame_number)
{
	RenderTexture2D target = LoadRenderTexture(width, height);
	int fps = GetFPS();
	BeginTextureMode(target);
		BeginMode2D(state.camera);
		ClearBackground(background_color);

		for (auto& planet : state.planets)
			DrawCircle(planet.position.x, planet.position.y, planet.radius, planet_color);
		EndMode2D();
	EndTextureMode();

	Image frame = LoadImageFromTexture(target.texture);
	ExportImage(frame, std::format("../frames/frame{}.png", frame_number).c_str());
	UnloadImage(frame);
	UnloadRenderTexture(target);
}

void main_loop(state& state)
{
	double dt = 0.1;
	int frame = 0;
	while (frame < frame_count)
	{
		update(state, dt);
		draw(state, frame++);
	}
}

int main()
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetConfigFlags(FLAG_WINDOW_HIDDEN);
	InitWindow(width, height, "P2P");


	Camera2D camera = {};
	camera.offset = { 0, 0 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	camera.target = { -width / 2.0f / camera.zoom, -height / 2.0f / camera.zoom};

	state state;
	state.camera = camera;

	galaxy_config config = {};
	config.center = { 0.0, 0.0 };
	config.radius = 300.0;
	config.core_mass = 1000.0;
	config.star_count = 1000;
	config.star_mass = 10.0;
	config.star_radius = 2.0;
	config.arm_count = 5;
	config.arm_rotation_factor = 3.0;
	config.arm_max_offset = 0.8;

	generate_galaxy(config, state.planets);

	main_loop(state);

	return 0;
}