#pragma once
#include "State.h"

#include "DebugDrawer.h"

#include "SFML/System/Vector2.hpp"

class HookGame : public CState 
{
public:
	HookGame();
	~HookGame();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Render(sf::RenderWindow* aRenderWindow);

private:

	sf::Vector2f myPlayerPos;
	sf::Vector2f myVelocity;
	bool myIsGrounded;
	bool myIsHooked = false;
	float myRopeLength;

	DebugDrawer myDrawer;
};