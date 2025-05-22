#pragma once
#include "Framework/World.h"

/** Forward Declarations */
class SSolitaireRules;
class SAStockPile;
class SATableau;
class SAFoundationList;

/**
 * Represents the main game world for Solitaire,
 * managing core components like rules, stock pile,
 * tableau (board), and foundation lists.
 */
class SGameBoardWorld : SWorld
{
public:
    /** Get Solitaire game rules */
    inline SSolitaireRules* GetGameRules() const { return GameRules.get(); }

    /** Get the Stock Pile */
    inline SAStockPile* GetStockPile() const { return StockPile.get(); }

    /** Get the Tableau (board columns) */
    inline SATableau* GetTableau() const { return Tableau.get(); }

    /** Get the Foundation Lists */
    inline SAFoundationList* GetFoundationList() const { return FoundationList.get(); }

protected:
    /** Unique pointer to game rules */
    SUniquePtr<SSolitaireRules> GameRules;

    /** Unique pointer to Stock Pile */
    SUniquePtr<SAStockPile> StockPile;

    /** Unique pointer to Tableau */
    SUniquePtr<SATableau> Tableau;

    /** Unique pointer to Foundation Lists */
    SUniquePtr<SAFoundationList> FoundationList;
};