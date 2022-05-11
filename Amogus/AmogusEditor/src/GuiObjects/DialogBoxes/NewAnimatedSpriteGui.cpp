#include "NewAnimatedSpriteGui.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>

NewAnimatedSpriteGui::NewAnimatedSpriteGui(Entity entityFor) :
	IGuiObject(nullptr),
	entity(entityFor),
	m_selectedTextureParamsIndex(0),
	inputInterval(0.05f),
	inputColour(1.0f),
	inputShaderName(""),
	inputVertexPath(""),
	inputFragmentPath(""),
	inputGeometryPath("")
{}

void NewAnimatedSpriteGui::CreateGui()
{
	ImGui::OpenPopup("New animated sprite");

	if (ImGui::BeginPopupModal("New animated sprite"))
	{
		ImGui::InputFloat("Interval", &inputInterval);
		if (ImGui::ColorPicker3("Texture colour##", glm::value_ptr(inputColour))) {}

		ImGui::Separator();
		// Texture selection combo box
		{
			std::string previewName("##");
			if (m_inputTextureParams.size() > 0)
			{
				previewName = m_inputTextureParams[m_selectedTextureParamsIndex].inputTextureName;
			}

			if (ImGui::BeginCombo("Textures", previewName.c_str()))
			{
				for (int i = 0; i < m_inputTextureParams.size(); i++)
				{
					bool isSelected = i == m_selectedTextureParamsIndex;

					std::string name("Texture " + std::to_string(i));
					if (m_inputTextureParams[i].inputTextureName != "")
					{
						name = m_inputTextureParams[i].inputTextureName;
					}
					if (ImGui::Selectable(name.c_str(), isSelected))
					{
						m_selectedTextureParamsIndex = i;
					}
				}

				ImGui::EndCombo();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("New texture"))
		{
			m_inputTextureParams.emplace_back();
			
		}

		// Display selected texture's data
		if (m_inputTextureParams.size() > 0 && m_selectedTextureParamsIndex < m_inputTextureParams.size())
		{
			if (ImGui::InputText("Texture name##", m_inputTextureParams[m_selectedTextureParamsIndex].inputTextureName, IM_ARRAYSIZE(m_inputTextureParams[m_selectedTextureParamsIndex].inputTextureName))) {}
			if (ImGui::InputText("Texture filepath##", m_inputTextureParams[m_selectedTextureParamsIndex].inputTextureFilePath, IM_ARRAYSIZE(m_inputTextureParams[m_selectedTextureParamsIndex].inputTextureFilePath))) {}
			if (ImGui::Button("Delete texture"))
			{
				m_inputTextureParams.erase(m_inputTextureParams.begin() + m_selectedTextureParamsIndex);
				m_selectedTextureParamsIndex = 0;
			}
		}

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
	std::vector<Texture2D> textures;
	for (int i = 0; i < m_inputTextureParams.size(); i++)
	{
		std::string textureName = m_inputTextureParams[i].inputTextureName;
		std::string textureFilePath = m_inputTextureParams[i].inputTextureFilePath;

		textures.emplace_back(TextureLoader::CreateTexture2DFromFile(textureName, textureFilePath));
	}

	std::string shaderName(inputShaderName);
	std::string vertexPath(inputVertexPath);
	std::string fragmentPath(inputFragmentPath);
	std::string geometryPath(inputGeometryPath);
	Shader* newShader = ShaderFactory::CreatePipelineShader(shaderName, vertexPath, fragmentPath, geometryPath);

	g_app->m_sceneManager->GetActiveScene()->m_entityManager->AddComponent<AnimatedSprite>(entity, textures, inputInterval, inputColour, newShader);
}