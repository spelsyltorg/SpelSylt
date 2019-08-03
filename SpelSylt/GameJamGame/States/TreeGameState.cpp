#include "GameJamGame/States/TreeGameState.h"

#include "GameJamGame/Gameplay/Controller/InputController.h"
#include "GameJamGame/Core/GameMessages.h"

#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Contexts/RenderingContext.h>

#include <SpelSylt/Messaging/MessageQueue.h>
#include <SpelSylt/Utility/Time/TimeGetter.h>
#include <SpelSylt/FileHandling/ConfigReader.h>
#include <SpelSylt/FileHandling/FileWatcher.h>
#include <SpelSylt/Math/CommonMath.h>

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

void CTreeGameState::OnInit(SS::SGameContext& InGameContext)
{
	Tree = std::make_unique<CTree>(InGameContext.MessageQueue, InGameContext.AssetManager, PlayerPawn);
	
	PlayerPawn.AttachController(Controllers.CreateInputController(InGameContext.Input, InGameContext.MessageQueue));

	ReadPlayerPawnSpeedFromConfig();

	SS::CFileWatcher::AddFile("player.cfg", [this] { ReadPlayerPawnSpeedFromConfig(); });

	GetCamera().setCenter({ 0,0 });
	GetCamera().setSize(1920.f, 1080.f);
}

//------------------------------------------------------------------

void CTreeGameState::OnUpdate(SS::SGameContext& InGameContext)
{
	Controllers.Update();
	PlayerPawn.Tick(InGameContext.Time.GetDeltaTime());

	if (Math::Length2(PlayerPawn.GetPosition()) < 100 * 100.f)
	{
		InGameContext.MessageQueue.DispatchEvent<ShowWaterPrompt>();
	}
	else
	{
		InGameContext.MessageQueue.DispatchEvent<HideWaterPrompt>();
	}

	myDebugDrawer.DrawCircle({}, 100.f, false, sf::Color::Blue);
}

//------------------------------------------------------------------

void CTreeGameState::OnRender(SS::CRenderQueue& InRenderQueue)
{
	if (Tree)
	{
		Tree->Render(InRenderQueue);
	}
	myDebugDrawer.DrawCircle(PlayerPawn.GetPosition(), 32.f, true, sf::Color::Green);
}

//------------------------------------------------------------------

void CTreeGameState::ReadPlayerPawnSpeedFromConfig()
{
	SS::CConfigReader ConfigReader;
	ConfigReader.ReadConfigFile("player.cfg");

	PlayerPawn.SetSpeed(ConfigReader.GetAsFloat("speed"));
}

//------------------------------------------------------------------