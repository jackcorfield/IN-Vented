#include "NewTileMapGui.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>

NewTileMapGui::NewTileMapGui(Entity entityFor) :
	entity(entityFor)
{}

void NewTileMapGui::CreateGui()
{
	if (ImGui::Begin("New tile map"))
	{
		if (ImGui::DragFloat2("Tile size", glm::value_ptr(inputTileSize), 0.5f)) {}
		if (ImGui::InputInt("Map width (in tiles)", &inputMapWidth)) {}
		if (ImGui::InputInt("Map height (in tiles)", &inputMapHeight)) {}

		if (ImGui::Button("Add tile map##"))
		{
			AddTileMap();
			close = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			close = true;
		}
	}

	ImGui::End();
}

void NewTileMapGui::AddTileMap()
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	glm::vec2 mapSize;
	mapSize.x = inputTileSize.x * inputMapWidth;
	mapSize.y = inputTileSize.y * inputMapHeight;

	std::vector<Entity> tiles;

	for (int y = 0; y < inputMapHeight; y++)
	{
		for (int x = 0; x < inputMapWidth; x++)
		{
			Entity newTile = entityManager->CreateEntity();

			entityManager->AddComponent<Tile>(newTile);

			tiles.emplace_back(newTile);
		}
	}

	entityManager->AddComponent<TileMap>(entity, inputTileSize, mapSize, tiles);
}