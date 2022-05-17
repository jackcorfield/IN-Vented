#include <Amogus.h>

#include <fstream>
#include <nlohmann/include/nlohmann/json.hpp>
#include "CameraFollowScript.h"
#include "EnemyMovementScript.h"
#include "EnemySpawnerScript.h"
#include "PlayerScript.h"
#include "WeaponScript.h"
#include "Shuriken.h"

class Runtime : public Application
{
public:

	Runtime(int argc, char** argv)
		: Application()
	{
		m_argc = argc;
		m_argv = argv;
	}

	void onInit() override
	{
		srand(std::time(nullptr));

		g_app->m_debugger->Log("Launched Yokai Fortune.", LL_DEBUG);

		processCommandLine();

		// Give entities called "Enemy" the enemy script
		EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
		auto allEntities = entityManager->GetAllActiveEntities();
		
		Entity player = GetEntityByName("Player");
		Entity enemy = GetEntityByName("Enemy");
		Entity enemySpawner = GetEntityByName("Enemy Spawner");
		Entity camera = GetEntityByName("Camera");
		Entity weapon = GetEntityByName("Weapon");

		ScriptComponent* scriptC = entityManager->GetComponent<ScriptComponent>(player);
		if (scriptC)
		{
			scriptC->AttachScript<PlayerScript>(100.0f);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(enemy);
		if (scriptC)
		{
			scriptC->AttachScript<EnemyMovementScript>(15.0f, player);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(enemySpawner);
		if (scriptC)
		{
			scriptC->AttachScript<EnemySpawnerScript>(player);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(camera);
		if (scriptC)
		{
			scriptC->AttachScript<CameraFollowScript>(player);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(weapon);
		if (scriptC)
		{
			Sprite* sprite = entityManager->AddComponent<Sprite>(weapon, TextureLoader::CreateTexture2DFromFile("defaultEntity", "Weapons/Shuriken/Shuriken.png"), glm::vec3(1.0f, 1.0f, 1.0f), ShaderFactory::CreatePipelineShader("defaultSprite", "DefaultSpriteV.glsl", "DefaultSpriteF.glsl"));
			entityManager->GetComponent<ScriptComponent>(weapon)->AttachScript<Shuriken>(player, *sprite, *sprite, entityManager->GetComponent<Transform>(weapon)->m_size);
		}

	}

	void onUpdate(float dt) override
	{

	}

	void onRender(float dt) override
	{

	}

	void onImGui() override
	{
		
	}

private:
	int m_argc;
	char** m_argv;
	
	void processCommandLine()
	{
		// COMMAND LINE
		
		// -help - Shows help
		// -game [gamename] - Loads and runs a game
		// -debug - Enables debug class

		switch (m_argc)
		{
		case 1:
			loadGame("YokaiFortune");
			//Quit();
			break;
		case 2:
			if (m_argv[1] == std::string("-help"))
			{
				printHelp();
			}
			Quit();
			break;
		case 3:
			if (m_argv[1] == std::string("-game"))
			{
				loadGame(m_argv[2]);
			}
			break;
		case 4:
			if (m_argv[1] == std::string("-game"))
			{
				loadGame(m_argv[2]);
				if (m_argv[3] == std::string("-debug"))
				{
					m_debug = true;
				}
			}
			if (m_argv[1] == std::string("-debug"))
			{
				m_debug = true;
				if (m_argv[2] == std::string("-game"))
				{
					loadGame(m_argv[3]);
				}
				else
				{
					printf("ERROR: Invalid command line");
					printHelp();
					Quit();
				}
			}
			break;
		}
	}

	void printHelp()
	{
		printf("YokaiFortune.exe\n-help - Shows help\n-game [gamename] - Loads and runs a game\n-debug - Enables debug class\n");
	}

	void loadGame(const char* gameName)
	{
		std::string path = "Data/Config/" + std::string(gameName) + ".json";
		std::ifstream file(path);
		if (file.is_open())
		{
			nlohmann::json json;
			file >> json;
			file.close();
			
			// Load game
			m_gameName = json["name"];
			
			for (const std::string& sceneName : json["scenes"].get<std::vector<std::string>>())
			{
				SceneImporter::ImportSceneFromFile(sceneName, false);
			}
			
			m_sceneManager->SetActiveScene(json["scenes"][0]);
		}
	}

	Entity GetEntityByName(const std::string& name)
	{
		EntityManager* entityManager = m_sceneManager->GetActiveScene()->m_entityManager;
		auto allNameComponents = entityManager->GetAllComponentsOfType<EntityName>();
		for (EntityName* nameComponent : allNameComponents)
		{
			if (nameComponent->m_name == name)
			{
				return entityManager->GetEntityFromComponent<EntityName>(nameComponent);
			}
		}

		return 0;
	}

	bool m_debug = false;

	std::string m_gameName;
};

Application* CreateApplication(int argc, char** argv)
{
	return new Runtime(argc, argv);
}