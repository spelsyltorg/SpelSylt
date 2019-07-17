#include "Game/HookGame.h"
#include "SFML/Graphics/RenderWindow.hpp"

#include "SpelSylt/Math/CommonMath.h"
#include "SpelSylt/Utility/Input/InputManager.h"

#include "SpelSylt/Rendering/RenderQueue.h"

#include <iostream>

#include "SpelSylt/FileHandling/Asset/Assets.h"

#include "SpelSylt/Contexts/GameContext.h"
#include "SpelSylt/Contexts/RenderingContext.h"
#include "SpelSylt/Messaging/MessageQueue.h"
#include "SpelSylt/FileHandling/Loading/AsyncLoaderInterface.h"
#include "SpelSylt/Utility/Time/TimeGetter.h"

#include "SpelSylt/Messaging/Messages/AudioMessages.h"

HookGame::HookGame()
	: myIsGrounded(true)
{
}

HookGame::~HookGame()
{
}

void HookGame::OnInit(SGameContext& InGameContext)
{
	myPlayerPos = sf::Vector2f(800.f, 900.f);

	for (int i = 0; i < 100; ++i)
	{
		myHookPoints.emplace_back(sf::Vector2f(200.f + (float)(rand() % 1200), 500.f - i * 75.f));
	}

	InGameContext.MessageQueue.DispatchEvent<SMusicMessage>("HookGameOst", false);

	myPlayerTexture = &InGameContext.TextureProvider.GetTexture("Graphics/Sprites/Player.png");
	myPlayerHookedTexture = &InGameContext.TextureProvider.GetTexture("Graphics/Sprites/Player_hooked.png");
	myRopeTexture = &InGameContext.TextureProvider.GetTexture("Graphics/Sprites/Rope.png");
	//myRopeTexture.setRepeated(true);
	myHookPointTexture = &InGameContext.TextureProvider.GetTexture("Graphics/Sprites/Hookpoint.png");
	myFloorTexture = &InGameContext.TextureProvider.GetTexture("Graphics/Sprites/Floor.png");
	//myFloorTexture.setRepeated(true);

	myRope.SetTextureAsset(*myRopeTexture);
	//myRope.setOrigin(myRopeTexture.getSize().x * 0.5f, 0.f);

	//myPlayer.setOrigin(myPlayerTexture.getSize().x * 0.5f, myPlayerTexture.getSize().y * 1.f);
}

void HookGame::OnUpdate(SGameContext& InGameContext)
{

	const float dt = InGameContext.Time.GetDeltaTime();

	Anchor = myPlayerPos;
	Anchor.y -= 100.f;

	if (!myIsHooked)
	{
		for (auto point : myHookPoints)
		{
			if (point.y <= Anchor.y)
			{
				if (Math::Length(myHookPoint - Anchor) > Math::Length(point - Anchor))
					myHookPoint = point;
			}
		}
	}

	auto& im = InGameContext.Input;
	if (im.IsKeyPressed(EKeyCode::Up))
	{
		if (!myIsHooked)
		{
			InGameContext.MessageQueue.DispatchEvent<SSoundMessage>("HookNoise");
			myRopeLength = Math::Length(myHookPoint - Anchor);
			myRotation = myTargetRotation;
		}
		myIsHooked = !myIsHooked;
	}

	myVelocity.y += 2500.f * dt;

	if ((myIsGrounded || myIsHooked) && im.IsKeyPressed(EKeyCode::Space))
	{
		myVelocity.y = -1000.f;
		myIsHooked = false;
	}

	if (!myIsHooked)
	{
		if (im.IsKeyDown(EKeyCode::Left))
			myVelocity.x = -200.f;
		if (im.IsKeyDown(EKeyCode::Right))
			myVelocity.x = 200.f;
		if (myIsGrounded && !im.IsKeyDown(EKeyCode::Left) && !im.IsKeyDown(EKeyCode::Right))
			myVelocity.x = 0.f;

		myPlayerPos += myVelocity * dt;
		if (!myIsGrounded)
		{
			myRotation += (myVelocity.x < 0.f ? 1.f : -1.f) * 720.f * InGameContext.Time.GetDeltaTime();
		}
		else
		{
			myRotation = 0.f;
			myTargetRotation = 0.f;
		}
	}
	else
	{
		sf::Vector2f p = Anchor + myVelocity * dt;
		if (Math::Length(p - myHookPoint) > myRopeLength)
		{
			sf::Vector2f restrainedP = (myHookPoint + myRopeLength * Math::GetNormalized(p - myHookPoint));
			myVelocity = (restrainedP - Anchor) / dt;
			myPlayerPos = restrainedP;
			myPlayerPos.y += 100.f;
		}
		else
		{
			myPlayerPos += myVelocity * dt;
		}
	}

	myPlayerPos.y = Math::Min(myPlayerPos.y, 900.f);
	myIsGrounded = myPlayerPos.y == 900.f;

	myPlayer.setPosition(myPlayerPos);

	myRotation = myRotation + InGameContext.Time.GetDeltaTime() * (myTargetRotation - myRotation);

	GetCamera().setCenter(myPlayerPos.x, myPlayerPos.y);
}

void HookGame::OnRender(CRenderQueue& InRenderQueue)
{
	sf::Vector2f pToAnchor = Anchor - myHookPoint;
	myRope.setPosition(myHookPoint);
	float rot = Math::ToDegrees(atan2f(-pToAnchor.x, pToAnchor.y));
	if (myIsHooked) myTargetRotation = -rot;
	myRope.setRotation(rot);
	//myRope.setTextureRect(sf::IntRect(0, 0, myRopeTexture->GetSize().x, 10 + (Math::Length(pToAnchor) / myRopeTexture->GetSize().y) * (myRopeTexture->GetSize().y)));

	const STextureAsset* texture = myIsHooked ? myPlayerHookedTexture : myPlayerTexture;
	myPlayer.SetTextureAsset(*texture);
	myPlayer.setOrigin(texture->GetSize().x * 0.5f, myPlayer.getOrigin().y);
	myPlayer.setRotation(myRotation);

	SS::CSprite floor;
	floor.setPosition(-1000.f, 900.f);
	floor.SetTextureAsset(*myFloorTexture);
	//floor.setTextureRect({ 0, 0, 5000, 1000 });
	InRenderQueue.Enqueue(ERenderLayer::Game, SSpriteRenderCommand(floor));
	if (myIsHooked) InRenderQueue.Enqueue(ERenderLayer::Game, SSpriteRenderCommand(myRope));
	for (auto hookPoint : myHookPoints)
	{
		SS::CSprite s;
		s.setOrigin(myHookPointTexture->GetSize().x * 0.5f, myHookPointTexture->getSize().y * 0.5f);
		s.SetTextureAsset(*myHookPointTexture);
		s.setPosition(hookPoint);
		InRenderQueue.Enqueue(ERenderLayer::Game, SSpriteRenderCommand(s));
	}
	InRenderQueue.Enqueue(ERenderLayer::Game, SSpriteRenderCommand(myPlayer));
}
