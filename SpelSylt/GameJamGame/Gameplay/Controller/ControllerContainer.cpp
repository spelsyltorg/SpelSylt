#include "GameJamGame/Gameplay/Controller/ControllerContainer.h"

#include <SpelSylt/Utility/Input/InputEventGetter.h>

#ifdef _DEBUG
#include <cassert>
#include <SpelSylt/Debugging/Logging/DebugLogger.h>
#endif

//------------------------------------------------------------------

using namespace tree;

//------------------------------------------------------------------

CControllerContainer::CControllerContainer()
	: InputControllers()
{
	InputControllers.reserve(sizeof(CInputController) * MAX_INPUT_CONTROLLERS);
}

//------------------------------------------------------------------

CInputController& CControllerContainer::CreateInputController(const SS::IInputEventGetter& InInputHandler, SS::CMessageQueue& InMessageQueue)
{
#ifdef _DEBUG
	if (!InputControllers.size() == MAX_INPUT_CONTROLLERS)
	{
		LOG_ERROR(CInputController, "Input controller buffer full. Consider incrementing MAX_INPUT_CONTROLLERS macro");
		assert(false);
	}
#endif

	InputControllers.emplace_back(CInputController{ InInputHandler, InMessageQueue });
	AllControllers.push_back(&InputControllers.back());
	return InputControllers.back();
}

//------------------------------------------------------------------

void tree::CControllerContainer::Update()
{
	for (IController* Controller : AllControllers)
	{
		Controller->Update();
	}
}

//------------------------------------------------------------------