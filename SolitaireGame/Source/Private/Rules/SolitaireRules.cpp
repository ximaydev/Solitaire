#include "SolitaireGamePCH.h"
#include "Rules/SolitaireRules.h"
#include "Config/IniFileManager.h"
#include "Config/IniFile.h"

SSolitaireRules::SSolitaireRules() 
	: CardsToRevealPerStockUse(SIniFileManager::GetInstance()->GetConfigFile(DefaultGameConfig)->GetValueFromKey<SUInt8>(TEXT("[GameRules]"), TEXT("CardsToRevealPerStockUse")))
{}