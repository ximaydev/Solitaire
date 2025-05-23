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
    /** Get Solitaire game rules */
    inline SASolitaireRules* GetGameRules() const { return GameRules.get(); }

    /** Get the Stock Pile */
    inline SAStockPile* GetStockPile() const { return StockPile.get(); }

    /** Get the Tableau (board columns) */
    inline SATableau* GetTableau() const { return Tableau.get(); }

    /** Get the Foundation Lists */
    inline SAFoundationList* GetFoundationList() const { return FoundationList.get(); }

    /** Initializes the GameBoard world, preparing all necessary components. */
    virtual SBool Initialize() override;

protected:
    /** Unique pointer to game rules */
    SUniquePtr<SASolitaireRules> GameRules = nullptr;

    /** Unique pointer to Stock Pile */
    SUniquePtr<SAStockPile> StockPile = nullptr;

    /** Unique pointer to Tableau */
    SUniquePtr<SATableau> Tableau = nullptr;

    /** Unique pointer to Foundation Lists */
    SUniquePtr<SAFoundationList> FoundationList = nullptr;

    /** Unique pointer to Move Manager */
    SUniquePtr<SSolitaireMoveManager> MoveManager = nullptr;
};