#include "InGameUIState.h"

#include <SpelSylt/UI/Label.h>
#include <SpelSylt/UI/Panel.h>
#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Utility/Time/TimeGetter.h>
#include <SpelSylt/Messaging/MessageQueue.h>

#include "GameJamGame/Core/GameMessages.h"
#include <SpelSylt/Math/CommonMath.h>

tree::CInGameUIState::CInGameUIState(unsigned int aWidth, unsigned int aHeight) :
	UI::UIState(aWidth, aHeight, "UI/Layouts/ingame_layout.xml")
{
	UIState::SetStateFlags(StateFlags::UPDATE_BELOW | CState::DRAW_BELOW);
}

void tree::CInGameUIState::OnInit(SGameContext & InGameContext)
{
	if (auto panel = myLayout->GetPanel("water_level")) {
		panel->SetBounds(-1.f, -1.f, 0.f, -1.f);
	}

	InGameContext.MessageQueue.Subscribe<ShowWaterPrompt>([this](const auto& msg) {
		if (auto prompt = myLayout->GetLabel("water_prompt")) {
			prompt->SetVisible(true);
		}
	}, mySubs);
	InGameContext.MessageQueue.Subscribe<HideWaterPrompt>([this](const auto& msg) {
		if (auto prompt = myLayout->GetLabel("water_prompt")) {
			prompt->SetVisible(false);
		}
	}, mySubs);
	InGameContext.MessageQueue.Subscribe<ShotgunAmmoMsg>([this](const auto& msg) {
		if (auto label = myLayout->GetLabel("shotgun_ammo")) {
			std::string ammoString = std::to_string(msg.Param);
			label->SetText(ammoString);
		}
	}, mySubs);
	InGameContext.MessageQueue.Subscribe<MinigunAmmoMsg>([this](const auto& msg) {
		if (auto label = myLayout->GetLabel("minigun_ammo")) {
			std::string ammoString = std::to_string(msg.Param);
			label->SetText(ammoString);
		}
	}, mySubs);
	InGameContext.MessageQueue.Subscribe<GrenadeAmmoMsg>([this](const auto& msg) {
		if (auto label = myLayout->GetLabel("grenade_ammo")) {
			std::string ammoString = std::to_string(msg.Param);
			label->SetText(ammoString);
		}
	}, mySubs);
	InGameContext.MessageQueue.Subscribe<WaterLevelMsg>([this](const auto& msg) {
		if (auto panel = myLayout->GetPanel("water_level")) {
			panel->SetBounds(-1.f, -1.f, Math::Clamp(msg.Param, 0.f, 1.f) * 405.f, -1.f);
		}
	}, mySubs);
	InGameContext.MessageQueue.Subscribe<TreeLevelMsg>([this](const auto& msg) {
		if (auto label = myLayout->GetLabel("tree_level")) {
			label->SetText(std::to_string(msg.Param));
		}
	}, mySubs);
	InGameContext.MessageQueue.Subscribe<SwitchWeaponMsg>([this](const auto& msg) {
		for (int i = 1; i <= 4; ++i)
		{
			if (auto panel = myLayout->GetPanel("weapon" + std::to_string(i))) {
				if (i == msg.Param + 1)
				{
					panel->SetScale(1.5f, 1.5f);
				}
				else
				{
					panel->SetScale(1.f, 1.f);
				}
			}
		}
	}, mySubs);
}

void tree::CInGameUIState::OnUpdate(SGameContext & InGameContext)
{
	if (auto waterPrompt = myLayout->GetLabel("water_prompt"))
	{
		auto c = waterPrompt->GetTextColor();
			c.a = 255.f * (0.3f + 0.2f * sinf(5.f * InGameContext.Time.GetTotalTime()));
			waterPrompt->SetTextColor(c);
	}
}

void tree::CInGameUIState::OnRender(sf::RenderTarget & InTarget)
{
}
