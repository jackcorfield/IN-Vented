#include "NewTileMapGui.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>

NewTileMapGui::NewTileMapGui(Entity entityFor) :
	entity(entityFor),
	inputTileSize(16.0f, 16.0f),
	inputMapSize(3, 3)
{}

void NewTileMapGui::CreateGui()
{
	ImGui::OpenPopup("New tile map");

	if (ImGui::BeginPopupModal("New tile map"))
	{
		if (ImGui::DragFloat2("Tile size", glm::value_ptr(inputTileSize), 0.5f)) {}
		if (ImGui::InputInt("Map width (in tiles)", &inputMapSize.x)) {}
		if (ImGui::InputInt("Map height (in tiles)", &inputMapSize.y)) {}

		if (ImGui::Button("Add tile map##"))
		{
			AddTileMap();
			close = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			close = true;
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::EndPopup();
	}
}

void NewTileMapGui::AddTileMap()
{
	EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

	std::vector<Entity> tiles;

	for (int y = 0; y < inputMapSize.y; y++)
	{
		for (int x = 0; x < inputMapSize.x; x++)
		{
			Entity newTile = entityManager->CreateEntity();

			entityManager->AddComponent<Tile>(newTile);

			tiles.emplace_back(newTile);
		}
	}

	entityManager->AddComponent<TileMap>(entity, inputTileSize, inputMapSize, tiles);
}