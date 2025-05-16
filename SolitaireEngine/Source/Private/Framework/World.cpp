#include "SolitaireEnginePCH.h"
#include "Framework/World.h"
#include "Framework/Actor.h"

void SWorld::AddActor(SAActor* Actor)
{
	/** Set this world context to the actor */
	Actor->World = this;

	/** Add the actor to the world's actor list */
	Actors.push_back(Actor);
}

void SWorld::Write()
{
	// Loop through each actor in the world
	for (SAActor* Actor : Actors)
	{
		// Write the actors to the console at the given position
		Actor->Write();
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