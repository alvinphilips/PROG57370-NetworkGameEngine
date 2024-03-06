#include "EngineCore.h"
#include "InputSystem.h"

#define NDEBUG_INPUT

InputSystem* InputSystem::instance = nullptr;

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::Initialize()
{
	// Initialize the state of the input system
	// Initialize keyStates and mouseButtonStates
	// Initialize keyEventHandlers and mouseEventHandlers
	for (int i = 0; i < 512; i++) 
	{
		keyStates[i] = false;
	}
	for (int i = 0; i < 5; i++) 
	{
		mouseButtonStates[i] = false;
	}
}

void InputSystem::Update()
{

	// Update the state of the input system every frame
	   // Handle events
	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type) {
		case SDL_KEYDOWN:
		{
			SDL_Scancode scancode = SDL_GetScancodeFromKey(event.key.keysym.sym);
#ifdef DEBUG_INPUT
			LOG((int)scancode);
#endif
			if (!keyStates[scancode]) 
			{
				keyStates[scancode] = true;
				triggerKeyEvent(event.key.keysym.sym, true); // Assuming triggerKeyEvent still uses SDL_Keycode
			}
		}
		break;
		case SDL_KEYUP:
		{
			SDL_Scancode scancode = SDL_GetScancodeFromKey(event.key.keysym.sym);
			if (keyStates[scancode]) 
			{
				keyStates[scancode] = false;
				triggerKeyEvent(event.key.keysym.sym, false); // Assuming triggerKeyEvent still uses SDL_Keycode
			}
		}
		break;
		// Handle other events as before...

		case SDL_MOUSEBUTTONDOWN:
			// Only if the button was previously up
			if (!mouseButtonStates[event.button.button])
			{
				mouseButtonStates[event.button.button] = true;
				triggerMouseEvent(event.button.button, true);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			// Only if the button was previously down
			if (mouseButtonStates[event.button.button])
			{
				mouseButtonStates[event.button.button] = false;
				triggerMouseEvent(event.button.button, false);
			}
			break;
		case SDL_CONTROLLERDEVICEADDED:
			gamepadId = event.cdevice.which;
			SDL_GameControllerOpen(gamepadId);
			break;

		case SDL_CONTROLLERBUTTONDOWN:
			handleGamepadButton(event.cbutton.which, static_cast<SDL_GameControllerButton>(event.cbutton.button), true);
			break;
		case SDL_CONTROLLERBUTTONUP:
			handleGamepadButton(event.cbutton.which, static_cast<SDL_GameControllerButton>(event.cbutton.button), false);
			break;
		case SDL_CONTROLLERAXISMOTION:
			handleGamepadAxis(event.caxis.which, static_cast<SDL_GameControllerAxis>(event.caxis.axis), event.caxis.value);
			break;
		case SDL_QUIT:
			if (quitEventHandler) 
			{
				quitEventHandler();
			}
			break;
		}
	}
}

void InputSystem::triggerKeyEvent(SDL_Keycode key, bool pressed) 
{
	auto it = pressed ? keyPressHandlers.find(key) : keyReleaseHandlers.find(key);
	auto end = pressed ? keyPressHandlers.end() : keyReleaseHandlers.end();

	if (it != end) {
		it->second();
	}
}

void InputSystem::triggerMouseEvent(Uint8 button, bool pressed) 
{
	auto it = pressed ? mousePressHandlers.find(button) : mouseReleaseHandlers.find(button);
	auto end = pressed ? mousePressHandlers.end() : mouseReleaseHandlers.end();

	if (it != end) 
	{
		it->second();
	}
}

bool InputSystem::areKeysPressed(const std::vector<SDL_Keycode>& keys) const
{
	for (auto key : keys) {
		if (!isKeyPressed(key))
			return false;
	}
	return true;
}

bool InputSystem::isKeyPressed(SDL_Keycode keycode) const 
{
	const SDL_Scancode key = SDL_GetScancodeFromKey(keycode);
	if (key >= 512)
	{
#ifdef DEBUG_INPUT
		LOG("Unhandled Input (key out of range): " << key);
#endif
		return false;
	}
	return keyStates[key];
}

bool InputSystem::isMouseButtonPressed(Uint8 button) const 
{
	return mouseButtonStates[button];
}

void InputSystem::registerKeyEventHandler(SDL_Keycode key, bool onPress, std::function<void()> handler) 
{
	if (onPress) 
	{
		keyPressHandlers[key] = handler;
	}
	else 
	{
		keyReleaseHandlers[key] = handler;
	}
}

void InputSystem::handleGamepadConnection(int joystickIndex) 
{
	SDL_GameController* gamepad = SDL_GameControllerOpen(joystickIndex);
	if (gamepad) {
		InputSystem::gamepadMap[joystickIndex] = gamepad;
	}
}

void InputSystem::registerMouseEventHandler(Uint8 button, bool onPress, std::function<void()> handler) 
{
	if (onPress) 
	{
		mousePressHandlers[button] = handler;
	}
	else 
	{
		mouseReleaseHandlers[button] = handler;
	}
}

void InputSystem::initializeGamepads() 
{
	int numJoysticks = SDL_NumJoysticks();
	for (int i = 0; i < numJoysticks; ++i) 
	{
		if (SDL_IsGameController(i)) 
		{
			SDL_GameController* controller = SDL_GameControllerOpen(i);
			if (controller) 
			{
				// Store the controller in a container if needed
			}
			else 
			{
				SDL_Log("Could not open gamecontroller %i: %s", i, SDL_GetError());
			}
		}
	}
}

void InputSystem::handleGamepadButton(SDL_JoystickID joystickID, SDL_GameControllerButton button, bool pressed) 
{
	// You might want to keep track of the state of each button for each gamepad
	gamepadButtonStates[joystickID][button] = pressed;

	// Trigger gamepad button event callbacks if any
	if (gamepadButtonEventHandlers.find(joystickID) != gamepadButtonEventHandlers.end() &&
		gamepadButtonEventHandlers[joystickID].find(button) != gamepadButtonEventHandlers[joystickID].end()) 
	{
		gamepadButtonEventHandlers[joystickID][button](pressed);
	}

	// Other game logic related to gamepad button press/release...
}

void InputSystem::registerGamepadButtonEventHandler(SDL_JoystickID joystickID, SDL_GameControllerButton button, std::function<void(bool)> handler)
{
	gamepadButtonEventHandlers[joystickID][button] = handler;
}

bool InputSystem::isGamepadButtonPressed(SDL_JoystickID joystickID, SDL_GameControllerButton button) const 
{
	auto joystickIt = gamepadButtonStates.find(joystickID);
	if (joystickIt != gamepadButtonStates.end()) 
	{
		auto buttonIt = joystickIt->second.find(button);
		if (buttonIt != joystickIt->second.end()) 
		{
			return buttonIt->second;
		}
	}
	return false; // Default to not pressed if not found
}

void InputSystem::handleGamepadAxis(SDL_JoystickID joystickID, SDL_GameControllerAxis axis, Sint16 value) 
{
	// Update the state of the axis
	gamepadAxisStates[joystickID][axis] = value;

	if (axis == SDL_CONTROLLER_AXIS_LEFTX || axis == SDL_CONTROLLER_AXIS_LEFTY) 
	{
		// Handle left analog stick movement
	}
	else if (axis == SDL_CONTROLLER_AXIS_RIGHTX || axis == SDL_CONTROLLER_AXIS_RIGHTY) 
	{
	}
}

float InputSystem::getGamepadAxisState(SDL_JoystickID joystickID, SDL_GameControllerAxis axis) const 
{
	auto joystickIt = gamepadAxisStates.find(joystickID);
	if (joystickIt != gamepadAxisStates.end()) 
	{
		auto axisIt = joystickIt->second.find(axis);
		if (axisIt != joystickIt->second.end()) 
		{
			// Normalize the value to be between -1.0 and 1.0
			return axisIt->second / 32767.0f;
		}
	}

	// Return 0.0 or an appropriate default value if the axis state is not found
	return 0.0f;
}

void InputSystem::registerQuitEventHandler(std::function<void()> handler) 
{
	quitEventHandler = handler;
}

void InputSystem::handleQuitEvent() 
{
	if (quitEventHandler) 
	{
		quitEventHandler();
	}
	else 
	{
		// Default quit behavior if no handler is set
		// For example, you might set a flag to indicate that the game should exit
	}
}
