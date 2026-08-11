#include "BXBuffComponent.h"

#include "BXBuffAsset.h"
#include "BXBuffManager.h"



#pragma region API
int64 UBXBuffComponent::AddBuff(UBXBuffAsset* InAsset, AActor* InInstigator, int32 InLayer, int32 InLevel)
{
	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	if (!Mgr)
	{
		return INDEX_NONE;
	}

	FBXBuffPlayContext Context;
	Context.Instigator = InInstigator ? InInstigator : GetOwner();
	Context.Triggerer = GetOwner();
	Context.InitLayer = InLayer;
	Context.InitLevel = InLevel;

	int64 BuffID = Mgr->AddBuff(InAsset, GetOwner(), Context);
	if (BuffID != INDEX_NONE)
	{
		OwnedBuffIDs.Add(BuffID);
	}

	return BuffID;
}

void UBXBuffComponent::RemoveBuff(int64 InBuffID, int32 InLayerDelta)
{
	UBXBuffManager* Mgr = UBXBuffManager::Get(this);
	if (!Mgr)
	{
		return;
	}

	Mgr->RemoveBuff(InBuffID, InLayerDelta);

	if (InLayerDelta == 0)
	{
		OwnedBuffIDs.Remove(InBuffID);
	}
}

void UBXBuffComponent::ChangeBuffLayer(int64 InBuffID, int32 InLayerDelta)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(this))
	{
		Mgr->ChangeBuffLayer(InBuffID, InLayerDelta);
	}
}

void UBXBuffComponent::ChangeBuffLevel(int64 InBuffID, int32 InLevelDelta)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(this))
	{
		Mgr->ChangeBuffLevel(InBuffID, InLevelDelta);
	}
}

void UBXBuffComponent::RefreshBuffLifetime(int64 InBuffID)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(this))
	{
		Mgr->RefreshBuffLifetime(InBuffID);
	}
}

bool UBXBuffComponent::HasBuff(UBXBuffAsset* InAsset) const
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(const_cast<UBXBuffComponent*>(this)))
	{
		return Mgr->HasBuff(GetOwner(), InAsset);
	}

	return false;
}

bool UBXBuffComponent::HasBuffByTag(FGameplayTag InTag) const
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(const_cast<UBXBuffComponent*>(this)))
	{
		return Mgr->HasBuffByTag(GetOwner(), InTag);
	}

	return false;
}

#pragma endregion API



#pragma region Lifecycle
void UBXBuffComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UBXBuffManager* Mgr = UBXBuffManager::Get(this))
	{
		for (int64 BuffID : OwnedBuffIDs)
		{
			Mgr->RemoveBuff(BuffID);
		}
	}

	OwnedBuffIDs.Empty();

	Super::EndPlay(EndPlayReason);
}

#pragma endregion Lifecycle
