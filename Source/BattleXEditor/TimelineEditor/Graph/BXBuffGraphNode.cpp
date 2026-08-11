#include "BXBuffGraphNode.h"

#include "BXBuffAsset.h"



#pragma region Lifecycle
void UBXBuffGraphNode::DestroyNode()
{
	if (CachedTask)
	{
		if (UBXBuffAsset* BuffAsset = Cast<UBXBuffAsset>(CachedTask->GetOuter()))
		{
			BuffAsset->RemoveTask(CachedTask);
		}
	}

	Super::DestroyNode();
}

#pragma endregion Lifecycle
