#include "BattleX.h"

#include "GameplayTagsManager.h"



#define LOCTEXT_NAMESPACE "FBattleXModule"

void FBattleXModule::StartupModule()
{
	UGameplayTagsManager::Get().AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("BattleX/Config/Tags"));
}

void FBattleXModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBattleXModule, BattleX)