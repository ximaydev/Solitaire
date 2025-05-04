#include "SolitaireGamePCH.h"
#include "Main.h"
#include "Globals.h"
#include "SolitaireGamePCH.h"
#include "Main.h"
#include "Core/Logger/LoggerMacros.h"
#include "Core/Logger/LogCategories.h"

int main()
{
    Core::Paths::CreateDirectories();
    S_LOG_WARNING(LogTemp, TEXT("Debug : %s"), TEXT("something"));
    S_LOG_ERROR(LogTemp, TEXT("Debug : %f"), 20.5);
    S_LOG_ERROR(LogTemp, TEXT("Debug : %d"), 50000);
    S_LOG_ERROR(LogTemp, TEXT("Debug"));

    FileLogger::GetInstance()->WaitForLoggingToFinish();
    return 0;
}