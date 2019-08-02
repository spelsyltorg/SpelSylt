#include "GameJamGame/Gameplay/Controller/InputController.h"

#include <SpelSylt/Math/CommonMath.h>
#include <SpelSylt/Utility/Input/InputEventGetter.h>

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CInputController::CInputController(const SS::IInputEventGetter& InInputGetter)
	: InputGetter(InInputGetter)
	, CurrentDirection(0.f, 0.f)
{
}

//------------------------------------------------------------------

void CInputController::Update()
{
	CurrentDirection.x = 0.f;
	CurrentDirection.y = 0.f;

	if (InputGetter.IsKeyDown(EKeyCode::Up))
	{
		CurrentDirection.y -= 1;
	}
	if (InputGetter.IsKeyDown(EKeyCode::Down))
	{
		CurrentDirection.y += 1;
	}
	if (InputGetter.IsKeyDown(EKeyCode::Right))
	{
		CurrentDirection.x += 1;
	}
	if(InputGetter.IsKeyDown(EKeyCode::Left))
	{
		CurrentDirection.x -= 1;
	}

	if (Math::Length2(CurrentDirection) > 1.f)
	{
		Math::Normalize(CurrentDirection);
	}
}

//------------------------------------------------------------------

const sf::Vector2f& tree::CInputController::GetDirection() const
{
	return CurrentDirection;
}

//------------------------------------------------------------------
