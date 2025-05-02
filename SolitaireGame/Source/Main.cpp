#include "SolitaireGamePCH.h"
#include "Main.h"
#include "Globals.h"
#include "Logger/LoggerMacros.h"
#include <thread>
#include <chrono>
int main()
{
    Core::Paths::IntializeProjectRootPath();
    S_LOG_WARNING(TEXT("debug wearning MESSAAGE"));
    S_LOG_ERROR(TEXT("debug ERROR mesfdsafdsfsage!##!"));
    S_LOG(TEXT("debug LOG MESSAGE??!!"));
    S_LOG_ERROR(TEXT("debug ERROR mesasdfdsfafssage!##!"));


    FileLogger::GetInstance()->WaitForLoggingToFinish();
    return 0;
}