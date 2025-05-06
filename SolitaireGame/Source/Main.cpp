#include "SolitaireGamePCH.h"
#include "SolitaireEnginePCH.h"
#include "Main.h"

#include "Core/Assets/AssetRegistry.h"
#include "Core/Assets/AssetLoader.h"
#include "Core/Assets/AssetLoaderRegistry.h"
int main()
{
    Core::Paths::CreateDirectories();
   
    std::shared_ptr<SWAVLoader> wavLoader = std::make_shared<SWAVLoader>();
    SAssetLoaderRegistry::GetInstance()->RegisterLoader(TEXT(".wav"), wavLoader);
   //SAssetRegistry::GetInstance()->RegisterAsset(TEXT("D:\\Work\\C++ Projects\\Solitaire\\SolitaireGame\\Content\\mixkit-game-show-suspense-waiting-667.wav"));
    SAssetRegistry::GetInstance()->RegisterAsset(TEXT("D:\\Work\\C++ Projects\\Solitaire\\SolitaireGame\\Content\\Wobble_SP_186_01.wav"));

    SAudioEngine AudioEngine;
    //AudioEngine.PlaySound(TEXT("D:\\Work\\C++ Projects\\Solitaire\\SolitaireGame\\Content\\mixkit-game-show-suspense-waiting-667.wav"), TEXT("Music"));
    AudioEngine.PlaySound(TEXT("D:\\Work\\C++ Projects\\Solitaire\\SolitaireGame\\Content\\Wobble_SP_186_01.wav"), TEXT("SFXVolume"));
    
    AudioEngine.SetVolume(TEXT("Music"), 0.75f);
    AudioEngine.SetVolume(TEXT("SFVVolume"), 0.3f);

    SINIFileManager::GetInstance()->LoadConfigFilesFromDisk();
    system("pause");
    SFileLogger::GetInstance()->WaitForLoggingToFinish();
    return 0;
}