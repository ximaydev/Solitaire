#include "SolitaireEnginePCH.h"
#include "Framework/World.h"
#include "Framework/Actor.h"

SWorld::SWorld(const SWorld& Other)
{
	// Call CopyFrom and perform a deep copy
	CopyFrom(Other);
}

void SWorld::CopyFrom(const SWorld& Other)
{
	// Clear the current list of actors
	Actors.clear();

	// Pre-allocate memory to avoid reallocations
	Actors.reserve(Other.Actors.size());

	// Deep copy each actor by creating a new shared instance with copied data
	for (const auto& ActorPtr : Other.Actors)
	{
		if (ActorPtr)
		{
			// Use the copy constructor to clone the actor's contents
			SSharedPtr<SAActor> NewActor = std::make_shared<SAActor>(*ActorPtr);
			Actors.push_back(NewActor);
		}
	}
}

void SWorld::Write()
{
	// Loop through each actor in the world
	for (SSharedPtr<SAActor> Actor : Actors)
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