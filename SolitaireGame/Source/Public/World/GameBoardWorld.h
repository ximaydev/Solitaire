#pragma once
#include "Framework/World.h"
#include "GameBoard/SolitaireMoveManager.h"

/** Forward Declarations */
class SASolitaireRules;
class SAStockPile;
class SATableau;
class SAFoundationList;

/**
 * Represents the main game world for Solitaire,
 * managing core components like rules, stock pile,
 * tableau (board), and foundation lists.
 */
class SGameBoardWorld : public SWorld
{
public:
    /** Initializes the GameBoard world, preparing all necessary components. */
    virtual SBool Initialize() override;

    /** Get the Tableau. */
    inline SATableau* GetTableau() const { return Tableau.get(); }

    /** Get the Foundation Lists. */
    inline SAFoundationList* GetFoundationList() const { return FoundationList.get(); }

    /** Get the Stock Pile. */
    inline SAStockPile* GetStockPile() const { return StockPile.get(); }

    /** Get the Move Manager */
    inline SSolitaireMoveManager* GetMoveManager() const { return MoveManager.get(); }

    /** Get the Game Rules */
    inline SASolitaireRules* GetGameRules() const { return GameRules.get(); }

protected:
    /** Unique pointer to Tableau */
    SUniquePtr<SATableau> Tableau = nullptr;

    /** Unique pointer to Foundation Lists */
    SUniquePtr<SAFoundationList> FoundationList = nullptr;

    /** Unique pointer to Stock Pile */
    SUniquePtr<SAStockPile> StockPile = nullptr;

    /** Unique pointer to Move Manager */
    SUniquePtr<SSolitaireMoveManager> MoveManager = nullptr;

    /** Unique pointer to game rules */
    SUniquePtr<SASolitaireRules> GameRules = nullptr;
};