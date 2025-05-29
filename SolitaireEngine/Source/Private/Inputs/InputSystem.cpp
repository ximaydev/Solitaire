#include "SolitaireEnginePCH.h"
#include "Inputs/InputSystem.h"

FKeyBinding::FKeyBinding(EKeyAction NewKeyAction, SUInt32 NewVirtualKey) : KeyAction(NewKeyAction), VirtualKey(NewVirtualKey) {}

FKeyBinding::FKeyBinding(EKeyAction NewKeyAction, SUInt32 NewVirtualKey, const FCallbackRecord& NewCallBack) :
	KeyAction(NewKeyAction), VirtualKey(NewVirtualKey), CallBack(NewCallBack) {}

SInputSystem* SInputSystem::GetInstance()
{
    // Returns the singleton instance of the SInputSystem.
    static SUniquePtr<SInputSystem> InputSystem = std::make_unique<SInputSystem>();
    return InputSystem.get();
}

SBool SInputSystem::Initialize()
{
    // Bind default key actions to corresponding virtual key codes
    Bindings[0] = { EKeyAction::MoveUp, VK_UP };
    Bindings[1] = { EKeyAction::MoveDown, VK_DOWN };
    Bindings[2] = { EKeyAction::Select, VK_RETURN };
    Bindings[3] = { EKeyAction::Back, VK_ESCAPE };
    Bindings[4] = { EKeyAction::StartTyping, VK_HOME };

    // Log successful initialization
    S_LOG(LogInputSystem, TEXT("Input System initialized."));

    return true;
}

SBool SInputSystem::IsPressed(EKeyAction KeyAction)
{
    // Return the current pressed state of the specified key action.
    return KeyStates[(SUInt8)KeyAction];
}

SUInt64 SInputSystem::AddCallback(EKeyAction KeyAction, const SCallback& CallBack)
{
    // Set ID
    SUInt64 AssignedID = NextID++;

    // Iterate through all key bindings
    for (FKeyBinding& KeyBinding : Bindings)
    {
        // Check if the current binding matches the specified key action
        if (KeyBinding.KeyAction == KeyAction)
        {
            // Add the new callback to the list of callbacks for this key action
            KeyBinding.CallBack = FCallbackRecord(AssignedID, CallBack);
            return AssignedID;
        }
    }

    // Return invalid callback ID
    return INVALID_CALLBACK_ID;
}

void SInputSystem::RemoveCallback(EKeyAction KeyAction, const SUInt64 CallbackID)
{
    // Check if the CallbackID isn't invalid
    if (CallbackID != INVALID_CALLBACK_ID)
    {
        // Iterate through all key bindings
        for (FKeyBinding& KeyBinding : Bindings)
        {
            // Check if this binding matches the requested key action
            if (KeyBinding.KeyAction == KeyAction)
            {
                // Remove callback
                KeyBinding.CallBack = FCallbackRecord();
            }
        }
    }
}

void SInputSystem::UpdateKeyStates()
{
    // Iterate through all the key bindings
    for (SInt32 Index = 0; Index < MaxBindings; Index++)
    {
        //Get Binding
        const FKeyBinding& Binding = Bindings[Index];

        // If no key is bound (VirtualKey = 0), skip this iteration
        if (Binding.VirtualKey == 0) continue;

        // Get the current state of the key (0x8000 means the key is pressed)
        SInt16 KeyStateResult = GetAsyncKeyState(Binding.VirtualKey);
        SBool IsCurrentlyPressed = (KeyStateResult & 0x8000) != 0;

        // If the key is pressed and was not pressed previously, trigger the callback
        if (IsCurrentlyPressed && !IsPressed(Binding.KeyAction))
        {
            if (Binding.CallBack.Callback)
            {
                // Execute the valid callback
                Binding.CallBack.Callback();
            }
            else
            {
                // Log a warning if the callback is invalid (nullptr)
                S_LOG_WARNING(LogInputSystem, TEXT("Attempted to invoke an invalid callback function."));
            }
        }

        // Update the key state (pressed or not) in the KeyStates array
        KeyStates[(SUInt8)Binding.KeyAction] = IsCurrentlyPressed;
    }
}