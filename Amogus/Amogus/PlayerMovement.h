#pragma once
 
#include "InputHandler.h"

class PlayerMovement
{
public:
	PlayerMovement();
	PlayerMovement(float speed) : m_speed(speed) {}

	float m_speed;

	void AddMovement(KeyInputEvent* e);
};
