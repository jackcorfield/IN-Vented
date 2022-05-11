#include "NewSpriteGui.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>

NewSpriteGui::NewSpriteGui(Entity entityFor) :
	IGuiObject(nullptr),
	entity(entityFor),
	inputTextureName(""),
	inputTextureFilePath(""),
	inputShaderName(""),
	inputVertexPath(""),
	inputFragmentPath(""),
	inputGeometryPath("")
{}

void NewSpriteGui::CreateGui()
{
	ImGui::OpenPopup("New sprite");

	if (ImGui::BeginPopupModal("New sprite"))
	{
		ImGui::Text("Texture");
		if (ImGui::InputText("Texture name##", inputTextureName, IM_ARRAYSIZE(inputTextureName))) {}
		if (ImGui::InputText("Texture filepath##", inputTextureFilePath, IM_ARRAYSIZE(inputTextureFilePath))) {}
		if (ImGui::ColorPicker3("Texture colour##", glm::value_ptr(colour))) {}

		ImGui::Separator();
		ImGui::Text("Shader");
		if (ImGui::InputText("Shader name##", inputShaderName, IM_ARRAYSIZE(inputShaderName))) {}
		if (ImGui::InputText("Vertex filepath##", inputVertexPath, IM_ARRAYSIZE(inputVertexPath))) {}
		if (ImGui::InputText("Fragment filepath##", inputFragmentPath, IM_ARRAYSIZE(inputFragmentPath))) {}
		if (ImGui::InputText("Geometry filepath##", inputGeometryPath, IM_ARRAYSIZE(inputGeometryPath))) {}

		if (ImGui::Button("Add sprite##"))
		{
			AddSprite();
			close = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel##"))
		{
			close = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void NewSpriteGui::AddSprite()
{
	// Create new texture and shader first; if these fail, don't add the Sprite
	std::string textureName(inputTextureName);
	std::string textureFilePath(inputTextureFilePath);

	std::string shaderName(inputShaderName);
	std::string vertexPath(inputVertexPath);
	std::string fragmentPath(inputFragmentPath);
	std::string geometryPath(inputGeometryPath);

	Texture2D newTexture = TextureLoader::CreateTexture2DFromFile(textureName, textureFilePath);
	Shader* newShader = ShaderFactory::CreatePipelineShader(shaderName, vertexPath, fragmentPath, geometryPath);
	if (newTexture.m_id != 0)
	{
		EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;

		entityManager->AddComponent<Sprite>(entity, newTexture, colour, newShader);
	}
}