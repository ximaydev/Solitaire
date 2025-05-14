#include "SolitaireEnginePCH.h"
#include "Framework/World.h"
#include "Framework/Actor.h"

void SWorld::Write()
{
	// Loop through each actor in the world
	for (const SAActor* Actor : Actors)
	{
		// Write the actors to the console at the given position
		WriteAt(Actor->GetGridPosition());
	}
}

void SWorld::ClearBuffer()
{
	/** Get the console renderer instance */
	SConsoleRenderer* ConsoleRenderer = SConsoleRenderer::GetInstance();

	// Clear the console buffer
	ConsoleRenderer->ClearBuffer();

	// Draw the console border after clearing the buffer
	ConsoleRenderer->DrawBorder();
}