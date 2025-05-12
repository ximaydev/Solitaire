#include "SolitaireEnginePCH.h"

SKeyBinding::SKeyBinding(EKeyAction NewKeyAction, SUInt32 NewVirtualKey) : KeyAction(NewKeyAction), VirtualKey(NewVirtualKey) {}

SKeyBinding::SKeyBinding(EKeyAction NewKeyAction, SUInt32 NewVirtualKey, const SCallBack& NewCallBack) :
	KeyAction(NewKeyAction), VirtualKey(NewVirtualKey), CallBack(NewCallBack) {}

SBool SInputSystem::Initialize()
{
    //Bind 
	Bindings[0] = { EKeyAction::MoveUp, VK_UP };
	Bindings[1] = { EKeyAction::MoveDown, VK_DOWN };
	Bindings[2] = { EKeyAction::Select, VK_RETURN };
	Bindings[3] = { EKeyAction::Back, VK_ESCAPE };

    // Log
    S_LOG(LogInputSystem, TEXT("Input System initialized."));

    return true;
}

SBool SInputSystem::IsPressed(EKeyAction KeyAction)
{
    return KeyStates[(SUInt8)KeyAction];
}

void SInputSystem::UpdateKeyStates()
{
    // Iterate through all the key bindings
    for (SInt32 Index = 0; Index < MaxBindings; Index++)
    {
        //Get Binding
        const SKeyBinding& Binding = Bindings[Index];

        // If no key is bound (VirtualKey = 0), skip this iteration
        if (Binding.VirtualKey == 0) continue;

        // Get the current state of the key (0x8000 means the key is pressed)
        SInt16 KeyStateResult = GetAsyncKeyState(Binding.VirtualKey);
        SBool IsCurrentlyPressed = (KeyStateResult & 0x8000) != 0;

        // Log when the key is pressed
        if (IsCurrentlyPressed)
        {
            S_LOG(LogInputSystem, TEXT("Key '%u' is pressed."), Binding.VirtualKey);
        }

        // If the key is pressed and was not pressed previously, trigger the callback
        if (IsCurrentlyPressed && !IsPressed(Binding.KeyAction))
        {
            if (Binding.CallBack)
            {
                // Invoke the assigned callback function
                Binding.CallBack(); 
            }
        }

        // Update the key state (pressed or not) in the KeyStates array
        KeyStates[(SUInt8)Binding.KeyAction] = IsCurrentlyPressed;
    }
}
