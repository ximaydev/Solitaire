#include "SolitaireGamePCH.h"
#include "GameBoard/Card.h"
#include "GameBoard/WastePile.h"
#include "GameBoard/StockPile.h"
#include "Framework/World.h"
#include "Inputs/ConsoleInputHandler.h"
#include "Framework/ConsolePrompt.h"
#include "GameBoard/Tableau.h"

void GenerateFullDeck(SVector<SSharedPtr<SACard>>& StockPileCards)
{
    StockPileCards.clear();

    // Wszystkie kolory i rangi (przyk³ad)
    constexpr int NumRanks = 13;
    constexpr int NumSuits = 4;

    for (int suit = 1; suit <= NumSuits; ++suit)
    {
        for (int rank = 1; rank <= NumRanks; ++rank)
        {
            FCardInfo CardInfo(static_cast<ECardRank>(rank), static_cast<ECardSuit>(suit), false);
            auto Card = std::make_shared<SACard>(SGridPositionU32(0, 0), CardInfo, nullptr);

            StockPileCards.push_back(std::move(Card));
        }
    }
}

SUniquePtr<SAStockPile> StockPile = nullptr;

void LogSomething()
{
    StockPile->UseStockPile();
}

int main()
{
    SSharedPtr<SWorld> World = std::make_shared<SWorld>();

    SSharedPtr<SACard> Card = World->SpawnActor<SSharedPtr<SACard>, SACard>(SGridPositionU32(68, 24), FCardInfo(ECardRank::Jack, ECardSuit::Spades, false), nullptr);
    SSharedPtr<SAConsolePrompt> ConsolePromt = World->SpawnActor<SSharedPtr<SAConsolePrompt>, SAConsolePrompt>(SGridPositionU32(90, 45), FG_YELLOW, TEXT("Hello World. Type something :"), std::bind(LogSomething));

    SVector<SSharedPtr<SACard>> StockPileCards;
    GenerateFullDeck(StockPileCards);
    StockPile = World->SpawnActor<SUniquePtr<SAStockPile>, SAStockPile>(SGridPositionU32(20, 10), StockPileCards);
    StockPile->Initialize();

    SConsoleInputHandler::GetInstance()->SetGridPosition(SGridPositionU32(10, 10));
    GSolitaireEngine->SetUseConsoleInputHandler(true);

    GSolitaireEngine->Initialize(World);
    ConsolePromt->Initialize();
    GSolitaireEngine->Run();

    return 0;
}