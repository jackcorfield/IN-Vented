#pragma once
 
#include <Handlers/InputHandler.h>

class PlayerMovement
{
public:
	PlayerMovement();
	PlayerMovement(float speed);

	float m_speed;

	void AddMovement(InputEvent* e);
};
