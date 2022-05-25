#include <Amogus.h>

#include <fstream>
#include <nlohmann/include/nlohmann/json.hpp>
#include "CameraFollowScript.h"
#include "EnemyMovementScript.h"
#include "EnemySpawnerScript.h"
#include "TimerScript.h"

#include "PlayerScript.h"
#include "WeaponScript.h"
#include "HealthBarScript.h"
#include "XpBarScript.h"

#include "Shuriken.h"
#include "HackingDevice.h"
#include "LaserGun.h"
#include "NeonKatana.h"
#include "Grenade.h"

#include "OpticalImplant.h"
#include "Passport.h"
#include "Ragnite.h"
#include "PowerGlove.h"
#include "PowerGem.h"

#include "StartMenuButton.h"
#include "QuitButtonScript.h"
#include "OptionsMenu.h"

class Runtime : public Application
{
public:

	Runtime(int argc, char** argv)
		: Application()
	{
		m_argc = argc;
		m_argv = argv;
	}

	Audio* audio;

	void onInit() override
	{
		srand(std::time(nullptr));

		g_app->m_debugger->Log("Launched Yokai Fortune.", LL_DEBUG);

		processCommandLine();
		
		EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
		g_app->m_audioManager->SetVolume(g_app->m_audioManager->m_sfx, 1.f);
		g_app->m_audioManager->SetVolume(g_app->m_audioManager->m_bgm, 1.f);

		//Start UI
		Entity sMenu = GetEntityByName("StartMenu");
		Entity oMenu = GetEntityByName("OptionsMenu");

		audio = entityManager->AddComponent<Audio>(sMenu, "bgm/05.mp3", g_app->m_audioManager->m_system, g_app->m_audioManager->m_bgm);
		g_app->m_audioManager->LoopOn(audio->m_sound);
		g_app->m_audioManager->PlayAudio(audio->m_sound, audio->m_group, audio->m_channel);
		ScriptComponent* scriptC = entityManager->GetComponent<ScriptComponent>(oMenu);
		if (scriptC)
		{
			scriptC->AttachScript<OptionsMenu>();
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(sMenu);
		if (scriptC)
		{
			scriptC->AttachScript<StartMenuButton>(oMenu);
		}
	}

	void loadMainScene()
	{
		g_app->m_audioManager->RemoveAudio(audio->m_sound);
		g_app->m_audioManager->StopAudio(audio->m_channel);

		EntityManager* entityManager = g_app->m_sceneManager->GetActiveScene()->m_entityManager;
		auto allEntities = entityManager->GetAllActiveEntities();

		//UI
		Entity gOver = GetEntityByName("GameOverScreen");
		Entity kCount = GetEntityByName("KillCount");

		Entity player = GetEntityByName("Player");
		Entity enemy = GetEntityByName("Enemy");
		Entity enemySpawner = GetEntityByName("Enemy Spawner");
		Entity camera = GetEntityByName("Camera");
		Entity xpManager = GetEntityByName("XpManager");
		Entity timer = GetEntityByName("Timer");
		Entity healthBar = GetEntityByName("HealthBar");
		Entity xpBar = GetEntityByName("XpBar");

		//Weapons
		Entity shuriken = GetEntityByName("Shuriken");
		Entity hDevice = GetEntityByName("HackingDevice");
		Entity lGun = GetEntityByName("LaserGun");
		Entity nKatana = GetEntityByName("NeonKatana");
		Entity gGrenade = GetEntityByName("Grenade");

		//Equipment
		Entity oImplant = GetEntityByName("OpticalImplant");
		Entity passport = GetEntityByName("Passport");
		Entity ragnite = GetEntityByName("Ragnite");
		Entity pGlove = GetEntityByName("PowerGlove");
		Entity pGem = GetEntityByName("PowerGem");

		audio = entityManager->AddComponent<Audio>(player, "bgm/02.mp3", g_app->m_audioManager->m_system, g_app->m_audioManager->m_bgm);
		g_app->m_audioManager->LoopOn(audio->m_sound);
		g_app->m_audioManager->PlayAudio(audio->m_sound, audio->m_group, audio->m_channel);

		ScriptComponent* scriptC = entityManager->GetComponent<ScriptComponent>(player);	
		scriptC = entityManager->GetComponent<ScriptComponent>(player);
		if (scriptC)
		{
			scriptC->AttachScript<PlayerScript>(gOver, 100.0f);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(xpManager);
		if (scriptC)
		{
			scriptC->AttachScript<XpManager>(player, 100, 10);
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

		scriptC = entityManager->GetComponent<ScriptComponent>(gOver);
		if (scriptC)
		{
			scriptC->AttachScript<QuitButtonScript>();
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(timer);
		if (scriptC)
		{
			scriptC->AttachScript<TimerScript>(timer, player);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(healthBar);
		if (scriptC)
		{
			scriptC->AttachScript<HealthBarScript>(player);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(xpBar);
		if (scriptC)
		{
			scriptC->AttachScript<XpBarScript>(player);
		}

#pragma region Weapon Scripts

		scriptC = entityManager->GetComponent<ScriptComponent>(gGrenade);
		if (scriptC)
		{
			//scriptC->AttachScript<Grenade>(player, gGrenade, kCount);
		}
		// need to be ordered in draw order
		scriptC = entityManager->GetComponent<ScriptComponent>(nKatana);
		if (scriptC)
		{
			//scriptC->AttachScript<NeonKatana>(player, kCount, kCount);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(shuriken);
		if (scriptC)
		{
			//scriptC->AttachScript<Shuriken>(player, kCount, kCount);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(lGun);
		if (scriptC)
		{
			//scriptC->AttachScript<LaserGun>(player, kCount, kCount);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(hDevice);
		if (scriptC)
		{
			scriptC->AttachScript<HackingDevice>(player, kCount, kCount);
		}

#pragma endregion

#pragma region Equipment Scripts

		scriptC = entityManager->GetComponent<ScriptComponent>(oImplant);
		if (scriptC)
		{
			//scriptC->AttachScript<OpticalImplant>(player, oImplant);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(passport);
		if (scriptC)
		{
			//scriptC->AttachScript<Passport>(player, passport);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(ragnite);
		if (scriptC)
		{
			//scriptC->AttachScript<Ragnite>(player, ragnite);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(pGlove);
		if (scriptC)
		{
			//scriptC->AttachScript<PowerGlove>(player, pGlove);
		}

		scriptC = entityManager->GetComponent<ScriptComponent>(pGem);
		if (scriptC)
		{
			//scriptC->AttachScript<PowerGem>(player, pGem);
		}

#pragma endregion
	}

	void onUpdate(float dt) override
	{
		g_app->m_audioManager->m_system->update();

		Entity sMenu = GetEntityByName("StartMenu");
		if (sMenu != 0)
		{
			StartMenuButton* menuButton = (StartMenuButton*)g_app->m_sceneManager->GetActiveScene()->m_entityManager->GetComponent<ScriptComponent>(sMenu)->GetAttachedScript();
			if (menuButton->loadMainScene)
			{
				menuButton->loadMainScene = false;
				g_app->m_sceneManager->SetActiveScene("MainScene");
				loadMainScene();
			}
		}
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