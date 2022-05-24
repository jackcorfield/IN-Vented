#pragma once

#include <ECS/Components/ScriptComponent.h>

class ScriptSystem
{
public:
	void OnAttach();
	void OnUpdate(float dt);
	void OnRender(float dt);
	void OnUnattach();
private:
};