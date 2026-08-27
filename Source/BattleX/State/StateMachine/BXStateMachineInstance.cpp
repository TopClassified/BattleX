#include "BXStateMachineInstance.h"



void UBXStateMachineInstance::Initialize(UBXStateMachineAsset* InAsset)
{
	Asset = InAsset;
	CurrentNode = nullptr;
	DefaultNode = InAsset ? InAsset->FindDefaultStateNode() : nullptr;
	bActivatedOnce = false;
}

void UBXStateMachineInstance::Deinitialize()
{
	Asset = nullptr;
	CurrentNode = nullptr;
	DefaultNode = nullptr;
	bActivatedOnce = false;
}
