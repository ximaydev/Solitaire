#pragma once
#include "Globals.h"
#include "Core/Inputs/KeyAction.h"
#include <functional>

/**
 * Structure that defines a mapping between a key action and a virtual key code.
 * Used to bind engine-defined actions (e.g., MoveUp) to specific keyboard inputs (e.g., VK_UP).
 */
struct SOLITAIRE_ENGINE_API SKeyBinding
{
	using SCallBack = std::function<void()>;

public:
	/** Constructors */
	SKeyBinding() = default;
	SKeyBinding(EKeyAction NewKeyAction, SUInt32 NewVirtualKey);
	SKeyBinding(EKeyAction NewKeyAction, SUInt32 NewVirtualKey, const SCallBack& NewCallBack);

	/** Input action defined by the engine (e.g., MoveUp, Select) */
	EKeyAction KeyAction = EKeyAction::Max;

	/** Windows virtual-key code (e.g., VK_UP, VK_RETURN) assigned to the action */
	SUInt32 VirtualKey = {};

	/** Callback function that is triggered when the key action is activated */
	SCallBack CallBack = nullptr;
};

/** Class responsible for handling input and mapping virtual key codes to game actions. */
class SOLITAIRE_ENGINE_API SInputSystem
{
	friend class SSolitaireEngine;

public:
	/** Maximum number of key bindings allowed */
	static constexpr SUInt8 MaxBindings = 16;

	/** Initializes the input system and sets up default key bindings */
	SBool Initialize();

	/** Checks if the key assigned to the given binding is currently pressed */
	SBool IsPressed(EKeyAction KeyAction);

protected:
	/** Updates the input states by polling the keyboard */
	void UpdateKeyStates();

	/** Array of key bindings linking engine actions to virtual key codes */
	SKeyBinding Bindings[MaxBindings];

	/** Key state map: index is EKeyAction as integer, value is true if pressed */
	SBool KeyStates[(SUInt8)EKeyAction::Max] = { false };
};