#include "NewAnimatedSpriteGui.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>

NewAnimatedSpriteGui::NewAnimatedSpriteGui(Entity entityFor) :
	IGuiObject(nullptr),
	entity(entityFor),
	inputColour(1.0f),
	inputShaderName("DefaultSprite"),
	inputVertexPath("DefaultSpriteV.glsl"),
	inputFragmentPath("DefaultSpriteF.glsl"),
	inputGeometryPath(""),
	inputTextureName(""),
	inputTextureFilePath(""),
	inputFrameSize(0.0f)
{}

void NewAnimatedSpriteGui::CreateGui()
{
	ImGui::OpenPopup("New animated sprite");

	if (ImGui::BeginPopupModal("New animated sprite"))
	{
		if (ImGui::ColorPicker3("Texture colour##", glm::value_ptr(inputColour))) {}

		ImGui::Separator();

		// Texture data
		if (ImGui::InputText("Spritesheet name##", inputTextureName, IM_ARRAYSIZE(inputTextureName))) {}
		if (ImGui::InputText("Spritesheet filepath##", inputTextureFilePath, IM_ARRAYSIZE(inputTextureFilePath))) {}
		if (ImGui::InputFloat2("Frame size##", glm::value_ptr(inputFrameSize))) {}

		ImGui::Separator();
		ImGui::Text("Shader");
		if (ImGui::InputText("Shader name##", inputShaderName, IM_ARRAYSIZE(inputShaderName))) {}
		if (ImGui::InputText("Vertex filepath##", inputVertexPath, IM_ARRAYSIZE(inputVertexPath))) {}
		if (ImGui::InputText("Fragment filepath##", inputFragmentPath, IM_ARRAYSIZE(inputFragmentPath))) {}
		if (ImGui::InputText("Geometry filepath##", inputGeometryPath, IM_ARRAYSIZE(inputGeometryPath))) {}

		if (ImGui::Button("Add animated sprite##"))
		{
			AddAnimatedSprite();
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

void NewAnimatedSpriteGui::AddAnimatedSprite()
{
	Texture2D spritesheet = TextureLoader::CreateTexture2DFromFile(inputTextureName, inputTextureFilePath);

	std::string shaderName(inputShaderName);
	std::string vertexPath(inputVertexPath);
	std::string fragmentPath(inputFragmentPath);
	std::string geometryPath(inputGeometryPath);
	Shader* newShader = ShaderFactory::CreatePipelineShader(shaderName, vertexPath, fragmentPath, geometryPath);

	AnimatedSprite* component = g_app->m_sceneManager->GetActiveScene()->m_entityManager->AddComponent<AnimatedSprite>(entity, spritesheet, inputFrameSize, inputColour, newShader);
}