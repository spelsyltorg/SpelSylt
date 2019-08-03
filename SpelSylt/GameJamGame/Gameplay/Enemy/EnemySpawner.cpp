#include "GameJamGame/Gameplay/Enemy/EnemySpawner.h"

#include <SpelSylt/Rendering/Sprite/Sprite.h>
#include <SpelSylt/Rendering/RenderCommand.h>
#include <SpelSylt/Rendering/RenderQueue.h>

#include "GameJamGame/Gameplay/Controller/ControllerContainer.h"

#include "GameJamGame/Core/WindowDefines.h"

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CEnemySpawner::CEnemySpawner(CControllerContainer& InControllerContainer)
{
	TimeBetweenSpawns = 5.f;
	TimeUntilNextSpawn = 0.f;
	NextSimpleEnemy = 0;

	SimpleEnemyList.reserve(MAX_SIMPLE_ENEMY_TYPE);

	for (int i = 0; i < MAX_SIMPLE_ENEMY_TYPE; ++i)
	{
		SimpleEnemyList.emplace_back();
		SimpleEnemyList.back().AttachController(InControllerContainer.CreateAIController());
		SimpleEnemyList.back().SetSpeed(64.f);
	}
}

//------------------------------------------------------------------

void CEnemySpawner::SetTexture(SS::CTexture& InTexture)
{
	SimpleEnemyTexture = InTexture;
}

//------------------------------------------------------------------

void CEnemySpawner::Update(float InDT)
{
	TimeUntilNextSpawn -= InDT;

	if (TimeUntilNextSpawn <= 0.f)
	{
		SpawnEnemy();
	}

	for (CPawn* ActiveEnemyPawn : ActiveEnemies)
	{
		ActiveEnemyPawn->Tick(InDT);
	}
}

//------------------------------------------------------------------

void CEnemySpawner::Render(SpelSylt::CRenderQueue& aRenderQueue)
{
	SS::CSprite Sprite;
	Sprite.SetTextureAsset(SimpleEnemyTexture.Get());

	for (CPawn* ActiveEnemyPawn : ActiveEnemies)
	{
		Sprite.setPosition(ActiveEnemyPawn->GetPosition());
		aRenderQueue.Enqueue(ERenderLayer::Game, SS::SSpriteRenderCommand(Sprite));
	}
}

//------------------------------------------------------------------

void CEnemySpawner::SpawnEnemy()
{
	TimeUntilNextSpawn = TimeBetweenSpawns;

	CPawn& NextEnemy = SimpleEnemyList[NextSimpleEnemy];
	NextSimpleEnemy++;

	int RandomVal = rand() % 8;

	sf::Vector2f PositionToSpawn = { 0.f, 0.f };

	switch (RandomVal)
	{
	case 0:
		PositionToSpawn = POS_TOP_LEFT;
		break;
	case 1:
		PositionToSpawn = POS_TOP_CENTER;
		break;
	case 2:
		PositionToSpawn = POS_TOP_RIGHT;
		break;
	case 3:
		PositionToSpawn = POS_CENTER_RIGHT;
		break;
	case 4:
		PositionToSpawn = POS_BOT_RIGHT;
		break;
	case 5:
		PositionToSpawn = POS_BOT_CENTER;
		break;
	case 6:
		PositionToSpawn = POS_BOT_LEFT;
		break;
	case 7:
		PositionToSpawn = POS_CENTER_LEFT;
	default:
		break;
	}

	NextEnemy.SetPositon(PositionToSpawn); //Todo: Should be random

	ActiveEnemies.push_back(&NextEnemy);
}

//------------------------------------------------------------------