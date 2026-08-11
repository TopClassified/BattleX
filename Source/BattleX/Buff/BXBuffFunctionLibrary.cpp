#include "BXBuffFunctionLibrary.h"

#include "BXBuffAsset.h"
#include "BXBuffManager.h"



#pragma region API
int64 UBXBuffFunctionLibrary::AddBuff(UObject* WorldContextObject, UBXBuffAsset* InAsset, AActor* InOwner, AActor* InInstigator, int32 InLayer, int32 InLevel)
{
	UBXBuffManager* Mgr = UBXBuffManager::Get(WorldContextObject);
	if (!Mgr)
	{
		return INDEX_NONE;
	}

	FBXBuffPlayContext Context;
	Context.Instigator = InInstigator;
	Context.Triggerer = InInstigator;
	Context.InitLayer = InLayer;
	Context.InitLevel = InLevel;

	return Mgr->AddBuff(InAsset, InOwner, Context);
}

void UBXBuffFunctionLibrary::RemoveBuff(UObject* WorldContextObject, int64 InBuffID, int32 InLayerDelta)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(WorldContextObject))
	{
		Mgr->RemoveBuff(InBuffID, InLayerDelta);
	}
}

void UBXBuffFunctionLibrary::ChangeBuffLayer(UObject* WorldContextObject, int64 InBuffID, int32 InLayerDelta)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(WorldContextObject))
	{
		Mgr->ChangeBuffLayer(InBuffID, InLayerDelta);
	}
}

void UBXBuffFunctionLibrary::ChangeBuffLevel(UObject* WorldContextObject, int64 InBuffID, int32 InLevelDelta)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(WorldContextObject))
	{
		Mgr->ChangeBuffLevel(InBuffID, InLevelDelta);
	}
}

void UBXBuffFunctionLibrary::RefreshBuffLifetime(UObject* WorldContextObject, int64 InBuffID)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(WorldContextObject))
	{
		Mgr->RefreshBuffLifetime(InBuffID);
	}
}

bool UBXBuffFunctionLibrary::HasBuff(UObject* WorldContextObject, AActor* InOwner, UBXBuffAsset* InAsset)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(WorldContextObject))
	{
		return Mgr->HasBuff(InOwner, InAsset);
	}

	return false;
}

bool UBXBuffFunctionLibrary::HasBuffByTag(UObject* WorldContextObject, AActor* InOwner, FGameplayTag InTag)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(WorldContextObject))
	{
		return Mgr->HasBuffByTag(InOwner, InTag);
	}

	return false;
}

#pragma endregion API
