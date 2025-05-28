#pragma once
#include "Framework/World.h"
#include "GameBoard/SolitaireMoveManager.h"

/** Forward Declarations */
class SASolitaireRules;
class SAStockPile;
class SATableau;
class SAFoundationList;
class SAWastePile;

/** Conditionally copies a new actor pointer to a member if the current actor matches. */
#define COPY_ACTOR(ToType, MemberPtr)                                    \
    if (ActorPtr.get() == OtherGameBoardWorld->MemberPtr.get())          \
    {                                                                    \
        MemberPtr = std::static_pointer_cast<ToType>(NewActor);          \
    }

/**
 * Represents the main game world for Solitaire,
 * managing core components like rules, stock pile,
 * tableau (board), and foundation lists.
 */
class SGameBoardWorld : public SWorld
{
    template<typename SnapshotType>
    friend class SUndoManager;
public:
    /** Constructor */
    SGameBoardWorld() = default;
    SGameBoardWorld(const SGameBoardWorld& Other);

    /** Operators */
    SGameBoardWorld operator=(const SGameBoardWorld& Other)
    {
        if (this != &Other)
        {
            // Call CopyFrom and perform a deep copy
            CopyFrom(Other);
        }

        return *this;
    }

    /** Initializes the GameBoard world, preparing all necessary components. */
    virtual SBool Initialize() override;

    /** Renders the World. */
    virtual void Write() override;

    /** Performs a deep copy of all owned data from 'other' into this object. */
    virtual void CopyFrom(const SWorld& Other) override;

    /** Performs a deep copy of the current object using the copy constructor. */
    virtual SSharedPtr<SWorld> Clone() const override { return SSharedPtr<SWorld>(new SGameBoardWorld(*this)); }
    virtual SSharedPtr<SGameBoardWorld> CloneWorld() const { return SSharedPtr<SGameBoardWorld>(new SGameBoardWorld(*this)); }

    /** Get the Tableau. */
    inline SSharedPtr<SATableau> GetTableau() const { return Tableau; }

    /** Get the Foundation Lists. */
    inline SSharedPtr<SAFoundationList> GetFoundationList() const { return FoundationList; }

    /** Get the Stock Pile. */
    inline SSharedPtr<SAStockPile> GetStockPile() const { return StockPile; }

    /** Get the Waste Pile. */
    inline SSharedPtr<SAWastePile> GetWastePile() const { return WastePile; }

    /** Get the Move Manager */
    inline SSharedPtr<SSolitaireMoveManager> GetMoveManager() const { return MoveManager; }

    /** Get the Game Rules */
    inline SSharedPtr<SASolitaireRules> GetGameRules() const { return GameRules; }

    /** Increments the move count. */
    inline void IncrementMoveCount() { MoveCount++; }

protected:
    /** Weak pointer to Tableau */
    SSharedPtr<SATableau> Tableau = {};

    /** Weak pointer to Foundation Lists */
    SSharedPtr<SAFoundationList> FoundationList = {};

    /** Weak pointer to Stock Pile */
    SSharedPtr<SAStockPile> StockPile = {};

    /** Weak pointer to waste pile */
    SSharedPtr<SAWastePile> WastePile = {};

    /** Weak pointer to Move Manager */
    SSharedPtr<SSolitaireMoveManager> MoveManager = {};

    /** Weak pointer to game rules */
    SSharedPtr<SASolitaireRules> GameRules = {};

    /** Number of moves the player has made. */
    SUInt64 MoveCount = 0;

private:
    /** Handler function triggered when the player requests to undo a move */
    void HandleUndoMove();
};