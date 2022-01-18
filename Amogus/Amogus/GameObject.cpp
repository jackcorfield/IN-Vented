#include "GameObject.h"


GameObject::GameObject(const char* spriteSheet, int* inputRenderer, Vector2 pos) {

	renderer = inputRenderer;
	spriteTexture = FileHandler::ReadPNG(spriteSheet); // This causes no default constructor error - query with Alex

}
void GameObject::Update()
{

	// TO DO - Implement Update function

	pos.x,pos.y = 0;

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = 32;
	srcRect.h = 32;

	destRect.x = pos.x;
	destRect.y = pos.y;
	destRect.w = srcRect.w * 2;
	destRect.h = srcRect.h * 2;

}
void GameObject::Render() 
{

	// TO DO - Implement Render function
	// 
	// Something like this:
	// RenderObj(renderer, spriteTexture, &srcRect, &destRect)

}
