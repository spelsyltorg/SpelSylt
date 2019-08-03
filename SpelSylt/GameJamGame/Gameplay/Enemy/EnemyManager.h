#pragma once

#include "GameJamGame/Core/System.h"
#include "GameJamGame/Gameplay/Pawn.h"
#include "GameJamGame/Gameplay/Enemy/Enemy.h"

#include <SpelSylt/Rendering/Sprite/Texture.h>
#include <SpelSylt/Rendering/Animation/SpriteAnimation.h>

#include <SpelSylt/Messaging/Subscribing/Subscriptions.h>
#include "GameJamGame/Core/GameMessages.h"

#include <vector>

#define MAX_SIMPLE_ENEMY_TYPE 32

namespace SpelSylt
{
	struct SGameContext;
}

namespace tree
{
	class CControllerContainer;

	class CEnemyManager
		: public ISystem
	{
	public:	
		CEnemyManager(CControllerContainer& InControllerContainer, SpelSylt::SGameContext& InGameContext);

		void SetTexture(SS::CTexture& InTexture);

		void Update(float InDT) override;
		void Render(SpelSylt::CRenderQueue& aRenderQueue) override;
	private:
		void SpawnEnemy();
		void KillEnemies(std::vector<int>& InEnemiesMarkedForKill);

		void OnHitscanMsg(const HitscanShotMsg& InMsg);

		SS::CSubscriptions Subscriptions;

		SpelSylt::SGameContext& GameContext;

		SS::CTexture SimpleEnemyTexture;

		float TimeBetweenSpawns;
		float TimeUntilNextSpawn;
		size_t NextSimpleEnemy;
		std::vector<CEnemy*> ActiveEnemies;
		std::vector<CEnemy> SimpleEnemyList;
	};
}