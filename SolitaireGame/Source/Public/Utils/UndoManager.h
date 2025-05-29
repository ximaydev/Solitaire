#pragma once
#include "Globals.h"
#include "Config/IniFileManager.h"
#include "Config/IniFile.h"

/** Forward Declarations */
class SAConsolePrompt;
class SAConsoleSelector;

/**
 * SUndoManager is a templated class designed to manage the undo history of a given object type.
 * It stores up to a fixed number of snapshots (of type SnapshotType), allowing the object to revert
 * to a previous state on demand. The class inherits from SAActor and maintains deep copy semantics
 * through its CopyFrom override.
 */
template<typename SnapshotType>
class SUndoManager
{
public:
    static SUndoManager* GetInstance();

    /** Constructor */
    SUndoManager(const SUInt32 NewMaxSnapshots) : MaxSnapshots(NewMaxSnapshots) {}

    /** Saves a snapshot of the given object. */
    void SaveSnapshot(const SnapshotType& SnapshotObject);

    /** Removes last taken snapshot */
    void RemoveLastSnapshot();

    /** Removes the every snapshot */
    void RemoveSnapshots();

    /** Returns true if undo is possible */
    inline SBool CanUndo() const { return !Snapshots.empty(); }

    /** Get MaxSnapshots */
    inline SUInt32 GetMaxSnapshots() const { return MaxSnapshots; }

    /** Restores the previous world state if available. Returns nullptr if none. */
    SSharedPtr<SnapshotType> Undo();

private:
    /** Maximum number of snapshots stored in the deque */
    SUInt32 MaxSnapshots = 0;

    /** Stores up to 3 previous snapshots */
    SDeque<SSharedPtr<SnapshotType>> Snapshots;
};

template<typename SnapshotType>
inline SUndoManager<SnapshotType>* SUndoManager<SnapshotType>::GetInstance()
{
    // Default max snapshots count if no specialization matches
    SUInt32 TempMaxSnapshots = 3;

    // If SnapshotType is derived from SGameBoardWorld, override max snapshots from config
    if constexpr (std::is_base_of<SGameBoardWorld, SnapshotType>::value)
    {
        TempMaxSnapshots = SIniFileManager::GetInstance()->GetConfigFile(DefaultGameConfig)->GetValueFromKey<SUInt32>(TEXT("[GameBoardWorld]"), TEXT("MaxSnapshots"));
    }

    static SUniquePtr<SUndoManager> UndoManager = std::make_unique<SUndoManager>(TempMaxSnapshots);
    return UndoManager.get();
}

template<typename SnapshotType>
inline void SUndoManager<SnapshotType>::SaveSnapshot(const SnapshotType& SnapshotObject)
{
    // If we've reached the maximum allowed snapshots, remove the oldest one
    if (MaxSnapshots == Snapshots.size())
    {
        Snapshots.pop_front();
        S_LOG(LogUndoManager, TEXT("Maximum snapshot count reached. Oldest snapshot removed."));
    }

    // Add a copy of the current snapshot to the back of the deque
    Snapshots.push_back(SnapshotObject.CloneWorld());

    // Log the successful snapshot save
    S_LOG(LogUndoManager, TEXT("Snapshot saved. Total snapshots: %d"), Snapshots.size());
}

template<typename SnapshotType>
inline void SUndoManager<SnapshotType>::RemoveLastSnapshot()
{	
    // Check if there are any snapshots stored before attempting to remove the last one
    if (Snapshots.size() != 0)
    {
        // Remove the most recent snapshot from the deque
        Snapshots.pop_back();

        S_LOG(LogUndoManager, TEXT("Snapshot removed. Remaining snapshots: %d"), Snapshots.size());
    }
}

template<typename SnapshotType>
inline void SUndoManager<SnapshotType>::RemoveSnapshots()
{
    Snapshots.clear();
}

template<typename SnapshotType>
inline SSharedPtr<SnapshotType> SUndoManager<SnapshotType>::Undo()
{
    // Check if undo is possible
    if (CanUndo())
    {
        // Move the latest snapshot out and remove it from the deque
        SSharedPtr<SnapshotType> LastSnapshot = Snapshots.back();
        Snapshots.pop_back();

        // Refresh callbacks for all actors in the snapshot
        for (auto& Actor : LastSnapshot->GetActors())
        {
            if (SAConsolePrompt* ConsolePrompt = dynamic_cast<SAConsolePrompt*>(Actor.get()))
            {
                // If the actor is a console prompt, bind the Enter key to the MoveManager's handler
                ConsolePrompt->SetConsolePromptCallback(std::bind(&SSolitaireMoveManager::OnEnterClicked, LastSnapshot->GetMoveManager(), std::placeholders::_1));
            }
            else if (SAConsoleSelector* ConsoleSelector = dynamic_cast<SAConsoleSelector*>(Actor.get()))
            {
                // Reinitialize to clear outdated input callbacks (e.g., from a previously removed ConsoleSelector)
                ConsoleSelector->Initialize();

                // If the actor is a console selector, bind the "Undo Move" option to the world's undo handler
                ConsoleSelector->SetNewCallback(TEXT("Undo Move"), std::bind(&SGameBoardWorld::HandleUndoMove, LastSnapshot));
                ConsoleSelector->SetNewCallback(TEXT("New game"),  std::bind(&SGameBoardWorld::HandleCreateNewMatch, LastSnapshot));
                ConsoleSelector->SetNewCallback(TEXT("Back to Main Menu"), std::bind(&SGameBoardWorld::HandleReturnToMainMenu, LastSnapshot));
            }
        }

        // Log the undo operation
        S_LOG(LogUndoManager, TEXT("Undo performed. Remaining snapshots: %d"), Snapshots.size());
        return LastSnapshot;
    }

    // Log that undo was not possible
    S_LOG_WARNING(LogUndoManager, TEXT("Undo requested, but no snapshots available."));
    return nullptr;
}