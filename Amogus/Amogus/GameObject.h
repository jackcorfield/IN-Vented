#pragma once
#include "FileHandler.h"

// Replace for engine vector when available as this is unnecessary
struct Vector2 {

	float x;
	float y;
	float w;
	float h;

	Vector2(float x, float y);
	Vector2(float x, float y, float w, float h);
 };

class GameObject
{
public:
	GameObject(const char* spriteSheet, int* inputRenderer, int x, int y); // change int inputRenderer to the renderer once that is merged
	~GameObject();
	
	void Update();
	void Render();

private:
	int posX, posY;

	// Add following once available in engine / swap for our versions
	PNG_Data spriteTexture;
	Vector2 srcRect, destRect;
	int* renderer;
};

