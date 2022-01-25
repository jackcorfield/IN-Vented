#pragma once
 
#include "InputHandler.h"

class PlayerMovement
{
public:
	PlayerMovement();

	float m_speed;

	void AddMovement(KeyInputEvent* e);
};
