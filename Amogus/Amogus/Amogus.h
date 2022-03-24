#pragma once

// Core
#include "src/Core/source.h"
#include "src/Core/Timer.h"

// ECS
#include "src/ECS/EntityManager.h"
// - Components
#include "src/ECS/Components/AnimatedSprite.h"
#include "src/ECS/Components/Audio.h"
#include "src/ECS/Components/BoxCollider.h"
#include "src/ECS/Components/Camera.h"
#include "src/ECS/Components/CircleCollider.h"
#include "src/ECS/Components/EntityName.h"
#include "src/ECS/Components/Physics.h"
#include "src/ECS/Components/PlayerMovement.h"
#include "src/ECS/Components/ScriptComponent.h"
#include "src/ECS/Components/Sprite.h"
#include "src/ECS/Components/Tile.h"
#include "src/ECS/Components/TileMap.h"
#include "src/ECS/Components/Transform.h"
// - Systems
#include "src/ECS/Systems/PathFinding.h"
#include "src/ECS/Systems/Renderer.h"

// Handlers
#include "src/Handlers/EventHandler.h"
#include "src/Handlers/InputHandler.h"

// Resource Factories
#include "src/ResourceFactories/ShaderFactory.h"

// Resources
#include "src/Resources/Framebuffer.h"
#include "src/Resources/Script.h"
#include "src/Resources/Shader.h"

// Scenes
#include "src/Scenes/Scene.h"
#include "src/Scenes/SceneImporter.h"
#include "src/Scenes/SceneExporter.h"
#include "src/Scenes/SceneManager.h"