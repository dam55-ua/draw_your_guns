#include <MainGameState.hpp>
#include <algorithm>
#include <iostream>
#include <vector>

namespace
{
	constexpr int terrain_segment_count = 40;
	constexpr int terrain_anchor_count = 12;
	constexpr int minimum_height_percent = 28;
	constexpr int maximum_height_percent = 52;
	constexpr int boundary_variation_percent = 25;
	constexpr int grass_height = 8;
	constexpr int base_width_percent = 6;
	constexpr int base_height_percent = 42;
	constexpr int foundation_height_percent = 5;
	constexpr int flag_pole_height = 18;
	constexpr int flag_pole_width = 2;
	constexpr int flag_width = 12;
	constexpr int flag_height = 8;

	struct TerrainAnchor
	{
		float x;
		int heightPercent;
	};
}

MainGameState::MainGameState()
{
}

void MainGameState::init()
{
	// El terreno no cambia cada frame: se genera una vez al entrar en este estado.
	generateTerrain();
}

void MainGameState::handleInput()
{

}

void MainGameState::update(float deltaTime)
{

}

void MainGameState::render()
{
	BeginDrawing();
	ClearBackground(SKYBLUE);

	// La tierra forma el cuerpo del mapa y la franja verde representa la superficie con césped.
	for (const TerrainSegment& segment : terrain)
	{
		DrawRectangleRec(segment.rectangle, BROWN);
		DrawRectangle(static_cast<int>(segment.rectangle.x), static_cast<int>(segment.rectangle.y),
			static_cast<int>(segment.rectangle.width), grass_height, GREEN);
	}

	// Franja visual de roca para reservar una zona que luego será indestructible.
	const int foundation_height = GetScreenHeight() * foundation_height_percent / 100;
	DrawRectangle(0, GetScreenHeight() - foundation_height, GetScreenWidth(), foundation_height, GRAY);

	for (int i = 0; i < static_cast<int>(spawnBases.size()); ++i)
	{
		const Rectangle platform = spawnBases[i].platform;
		const Color team_color = i == 0 ? BLUE : RED;
		const float pole_x = platform.x + platform.width / 2.0f;
		const float pole_top = platform.y - flag_pole_height;

		DrawRectangleRec(platform, team_color);
		DrawRectangle(static_cast<int>(pole_x), static_cast<int>(pole_top),
			flag_pole_width, flag_pole_height, DARKGRAY);
		DrawTriangle(
			{ pole_x + flag_pole_width, pole_top },
			{ pole_x + flag_pole_width, pole_top + flag_height },
			{ pole_x + flag_pole_width + flag_width, pole_top + flag_height / 2.0f },
			team_color
		);
	}

	EndDrawing();
}

Vector2 MainGameState::getPlayerSpawnPosition(int playerIndex) const
{
	if (playerIndex < 0 || playerIndex >= static_cast<int>(spawnBases.size()))
	{
		return { 0.0f, 0.0f };
	}

	return spawnBases[playerIndex].spawnPosition;
}

void MainGameState::generateTerrain()
{
	terrain.clear();

	const int screen_width = GetScreenWidth();
	const int screen_height = GetScreenHeight();
	const int segment_width = screen_width / terrain_segment_count;
	const int base_width = screen_width * base_width_percent / 100;
	const int base_surface_y = screen_height - screen_height * base_height_percent / 100;

	for (int i = 0; i < static_cast<int>(spawnBases.size()); ++i)
	{
		const int left_x = i == 0 ? 0 : screen_width - base_width;
		const int center_x = left_x + base_width / 2;
		spawnBases[i].platform = {
			static_cast<float>(left_x), static_cast<float>(base_surface_y),
			static_cast<float>(base_width), static_cast<float>(grass_height)
		};
		spawnBases[i].spawnPosition = { static_cast<float>(center_x), static_cast<float>(base_surface_y) };
	}

	std::vector<int> boundaries(terrain_segment_count + 1);
	// Los extremos fijos hacen que los tramos cubran todo el ancho de la ventana.
	boundaries.front() = 0;
	boundaries.back() = screen_width;

	// Se desplazan ligeramente los límites interiores para evitar tramos idénticos.
	for (int i = 1; i < terrain_segment_count; ++i)
	{
		const int ideal_boundary = i * screen_width / terrain_segment_count;
		const int variation = segment_width * boundary_variation_percent / 100;
		const int lower_bound = boundaries[i - 1] + 1;
		const int upper_bound = screen_width - (terrain_segment_count - i);
		boundaries[i] = GetRandomValue(ideal_boundary - variation, ideal_boundary + variation);
		boundaries[i] = boundaries[i] < lower_bound ? lower_bound : boundaries[i];
		boundaries[i] = boundaries[i] > upper_bound ? upper_bound : boundaries[i];
	}

	// Se fuerzan límites en los extremos de cada base para conservar una plataforma plana.
	for (const SpawnBase& base : spawnBases)
	{
		boundaries.push_back(static_cast<int>(base.platform.x));
		boundaries.push_back(static_cast<int>(base.platform.x + base.platform.width));
	}
	std::sort(boundaries.begin(), boundaries.end());
	boundaries.erase(std::unique(boundaries.begin(), boundaries.end()), boundaries.end());

	// Las dos bases son anclas fijas; el resto de las alturas se genera aleatoriamente fuera de ellas.
	std::vector<TerrainAnchor> anchors;
	for (const SpawnBase& base : spawnBases)
	{
		anchors.push_back({ base.platform.x, base_height_percent });
		anchors.push_back({ base.platform.x + base.platform.width, base_height_percent });
	}

	for (int i = 1; i < terrain_anchor_count - 1; ++i)
	{
		const float x = static_cast<float>(i * screen_width / (terrain_anchor_count - 1));
		bool inside_base = false;
		for (const SpawnBase& base : spawnBases)
		{
			if (x >= base.platform.x && x <= base.platform.x + base.platform.width)
			{
				inside_base = true;
				break;
			}
		}

		if (!inside_base)
		{
			anchors.push_back({ x, GetRandomValue(minimum_height_percent, maximum_height_percent) });
		}
	}
	std::sort(anchors.begin(), anchors.end(), [](const TerrainAnchor& first, const TerrainAnchor& second)
	{
		return first.x < second.x;
	});

	for (size_t i = 0; i + 1 < boundaries.size(); ++i)
	{
		const float left_x = static_cast<float>(boundaries[i]);
		const float right_x = static_cast<float>(boundaries[i + 1]);
		const float sample_x = (left_x + right_x) * 0.5f;
		auto right_anchor = std::lower_bound(anchors.begin(), anchors.end(), sample_x,
			[](const TerrainAnchor& anchor, float x) { return anchor.x < x; });
		if (right_anchor == anchors.begin()) ++right_anchor;
		if (right_anchor == anchors.end()) right_anchor = anchors.end() - 1;
		const TerrainAnchor& left_anchor = *(right_anchor - 1);
		const float blend = (sample_x - left_anchor.x) / (right_anchor->x - left_anchor.x);
		int height_percent = static_cast<int>(left_anchor.heightPercent * (1.0f - blend) +
			right_anchor->heightPercent * blend + 0.5f);

		bool inside_base = false;
		for (const SpawnBase& base : spawnBases)
		{
			if (sample_x >= base.platform.x && sample_x <= base.platform.x + base.platform.width)
			{
				inside_base = true;
				break;
			}
		}
		if (!inside_base) height_percent += GetRandomValue(-2, 2);
		height_percent = height_percent < minimum_height_percent ? minimum_height_percent : height_percent;
		height_percent = height_percent > maximum_height_percent ? maximum_height_percent : height_percent;

		const int height = screen_height * height_percent / 100;
		// En coordenadas de pantalla, aumentar Y baja el rectángulo; por eso se ancla al fondo.
		terrain.push_back({
			{ left_x, static_cast<float>(screen_height - height), right_x - left_x, static_cast<float>(height) }
		});
	}
}