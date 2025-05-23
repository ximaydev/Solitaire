#include "SolitaireGamePCH.h"
#include "GameBoard/Card.h"
#include "GameBoard/WastePile.h"
#include "GameBoard/StockPile.h"
#include "Framework/World.h"
#include "Inputs/ConsoleInputHandler.h"
#include "Framework/ConsolePrompt.h"
#include "GameBoard/Tableau.h"
#include "World/GameBoardWorld.h"
#include "Rules/SolitaireRules.h"
#include "GameBoard/FoundationList.h"


SUniquePtr<SAStockPile> StockPile = nullptr;

void LogSomething()
{
    StockPile->UseStockPile();
}

int main()
{
    SSharedPtr<SWorld> World = std::make_shared<SGameBoardWorld>();

    //SConsoleInputHandler::GetInstance()->SetGridPosition(SGridPositionU32(10, 10)); 
    GSolitaireEngine->SetUseConsoleInputHandler(true);

    GSolitaireEngine->Initialize(World);
    //ConsolePromt->Initialize();
    GSolitaireEngine->Run();

    return 0;
}