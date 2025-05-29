#pragma once
#include "Globals.h"
#include "Inputs/KeyAction.h"

/** Macro representing an invalid or uninitialized callback ID. */
#define INVALID_CALLBACK_ID (SUInt64)-1

/** Represents a single callback record that stores a unique identifier */
struct SOLITAIRE_ENGINE_API FCallbackRecord
{
public:
	/** Unique identifier for this callback (used for removal or tracking) */
	SUInt64 ID = {};

	/** The actual callback function to be invoked */
	SCallback Callback = {};
};

/**
 * Structure that defines a mapping between a key action and a virtual key code.
 * Used to bind engine-defined actions (e.g., MoveUp) to specific keyboard inputs (e.g., VK_UP).
 */
struct SOLITAIRE_ENGINE_API FKeyBinding
{
public:
	/** Constructors */
	FKeyBinding() = default;
	FKeyBinding(EKeyAction NewKeyAction, SUInt32 NewVirtualKey);
	FKeyBinding(EKeyAction NewKeyAction, SUInt32 NewVirtualKey, const FCallbackRecord& NewCallBack);

	/** Input action defined by the engine (e.g., MoveUp, Select) */
	EKeyAction KeyAction = EKeyAction::Max;

	/** Windows virtual-key code (e.g., VK_UP, VK_RETURN) assigned to the action */
	SUInt32 VirtualKey = {};

	/** Callback function that is triggered when the key action is activated */
	FCallbackRecord CallBack;
};

/** Class responsible for handling input and mapping virtual key codes to game actions. */
class SOLITAIRE_ENGINE_API SInputSystem
{
	friend class SSolitaireEngine;

public:
	/** Get instance of this class */
	static SInputSystem* GetInstance();

	/** Maximum number of key bindings allowed */
	static constexpr SUInt8 MaxBindings = 16;

	/** Initializes the input system and sets up default key bindings */
	SBool Initialize();

	/** Checks if the key assigned to the given binding is currently pressed */
	SBool IsPressed(EKeyAction KeyAction);

	/** Registers a callback to be triggered when the specified key action occurs */
	SUInt64 AddCallback(EKeyAction KeyAction, const SCallback& Callback);

	/** Remove the callback from the given key action */
	void RemoveCallback(EKeyAction KeyAction, const SUInt64 CallbackID);

protected:
	/** Updates the input states by polling the keyboard */
	void UpdateKeyStates();

	/** Array of key bindings linking engine actions to virtual key codes */
	FKeyBinding Bindings[MaxBindings];

	/** Key state map: index is EKeyAction as integer, value is true if pressed */
	SBool KeyStates[(SUInt8)EKeyAction::Max] = { false };

	/** The next available unique ID that will be assigned to the callback record. */
	SUInt64 NextID = 1;
};