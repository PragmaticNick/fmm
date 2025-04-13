#include <string>
#include <format>

#include <raylib.h>
#include <raymath.h>

#include "fmm.h"
#include "constants.h"

#include "galaxy.h"

const Color background_color = { 50, 52, 55, 255 };
const Color planet_color = { 235, 231, 205, 255 };

struct state
{
	std::vector<planet> planets;
	Camera2D camera;
};


void input(state& state)
{
	float wheel = GetMouseWheelMove();
	if (wheel != 0)
	{
		Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), state.camera);
		state.camera.offset = GetMousePosition();
		state.camera.target = mouseWorldPos;
		
		float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
		if (wheel < 0) scaleFactor = 1.0f / scaleFactor;
		state.camera.zoom = state.camera.zoom * scaleFactor;
	}
}

void update(state& state, double dt)
{
	double step = 0.0001;
	double sum = 0.0;
	while (sum < dt)
	{
		for (auto& p : state.planets)
			p.force = { 0.0, 0.0 };

		p2p(state.planets);

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

void update_old(state& state, double dt)
{
	p2p(state.planets);

	size_t size = state.planets.size();
	for (int i = 0; i < size; i++)
	{
		planet& p = state.planets[i];
		auto a = p.force / p.mass;
		auto v = p.velocity;
		p.velocity += a * dt;
		p.position += v * dt;
	}
}


void draw(state& state)
{
	int fps = GetFPS();
	BeginDrawing();
		BeginMode2D(state.camera);
		ClearBackground(background_color);

		for (auto& planet : state.planets)
			DrawCircle(planet.position.x, planet.position.y, planet.radius, planet_color);
		EndMode2D();

		std::string fps_string = "FPS: " + std::to_string(GetFPS());
		std::string planet_string = "Planets: " + std::to_string(state.planets.size());
		DrawText(fps_string.c_str(), 0, 0, 40, RAYWHITE);
		DrawText(planet_string.c_str(), 0, 40, 40, RAYWHITE);
	EndDrawing();
}

void main_loop(state& state)
{
	while (!WindowShouldClose())
	{
		input(state);
		float dt = GetFrameTime();
		update(state, dt);
		draw(state);
	}
}

int main()
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	int screenWidth = 1600;
	int screenHeight = 900;
	InitWindow(screenWidth, screenHeight, "P2P");


	Camera2D camera = {};
	camera.offset = { 0, 0 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	camera.target = { -screenWidth / 2.0f / camera.zoom, -screenHeight / 2.0f / camera.zoom};

	state state;
	state.camera = camera;

	generate_galaxy(state.planets);

	main_loop(state);

	return 0;
}