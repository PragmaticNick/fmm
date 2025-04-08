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
	Texture2D planet_texture;
};

double galaxy_radius = 500.0;

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
	p2p(state.planets);

	size_t size = state.planets.size();
	for (int i = 0; i < size; i++)
	{
		planet& p = state.planets[i];
		double step = 1.0e-4;
		double sum = 0.0;
		while (sum < dt)
		{
			auto ai = p.force / p.mass;
			auto vi = p.velocity;
			p.velocity += ai * Step;
			p.position += vi * Step;
			sum += step;	
		}
	}

	tree t(targets);
	t.root->l->add(&m1);
	t.root->l->add(&m2);
	//t.root->left->left->l->add(&m2);
	//t.root->left->right->l->add(&m2);
	//t.root->right->left->l->add(&m2);
	//t.root->right->right->l->add(&m2);
	downward_pass(&t);

	std::vector<glm::dvec2> fmm_target_forces(m);
	for (int i = 0; i < m; i++)
		fmm_target_forces[i] = targets[i].force;

	sources1.insert(sources1.end(), sources2.begin(), sources2.end());
	for (int t = 0; t < m; t++)
		p2p(sources1, targets[t]);

	std::vector<glm::dvec2> p2p_target_forces(m);
	for (int i = 0; i < m; i++)
		p2p_target_forces[i] = targets[i].force;

	double e = error(p2p_target_forces, fmm_target_forces);
	std::cout << std::setprecision(5) << "Error: " << e << "%" << std::endl;
	std::cout << std::endl;
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
		{
			/*double r = planet.radius;
			Rectangle src = { 0, 0, state.planet_texture.width, state.planet_texture.height };
			Rectangle dst = { planet.position.x - 3 * r, planet.position.y - 3 * r, r * 6, r * 6 };
			DrawTexturePro(state.planet_texture, src, dst, { 0, 0 }, 0.0f, RAYWHITE);*/
			DrawCircle(planet.position.x, planet.position.y, planet.radius, planet_color);
		}
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

	Image image = LoadImage("./glow.png");
	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);

	state state;
	state.planet_texture = texture;

	Camera2D camera = {};
	camera.offset = { 0, 0 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	camera.target = { -screenWidth / 2.0f / camera.zoom, -screenHeight / 2.0f / camera.zoom};

	state.camera = camera;

	//planet earth = {};
	//earth.mass = 5.972e+24;
	//earth.position = { 0, 0 };
	//earth.velocity = { 0.0, 0.0 };
	//earth.radius = 6371 * 1000;

	//state.planets.push_back(earth);

	//planet moon = { };
	//moon.mass = 7.36e+22;
	//moon.position = { 384400 * 1000, 0 };
	//moon.velocity = { 0.0, 1020.0 };
	//moon.radius = 1737 * 1000;

	//state.planets.push_back(moon);
	generate_galaxy(state.planets);

	main_loop(state);

	return 0;
}