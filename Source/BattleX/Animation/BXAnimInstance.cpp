#include "BXAnimInstance.h"

#include "BXFunctionLibrary.h"



int64 UBXAnimInstance::GetPlayAnimationPermission(const FGameplayTag& InAnimationTag, int32 InPriority)
{
	int64 Result = -1;
	
	if (FBXPlayAnimationPermission* FindResult = PlayAnimationPermissionMap.Find(InAnimationTag))
	{
		if (FindResult->Priority <= InPriority)
		{
			Result = UBXFunctionLibrary::GetUniqueID();
			FindResult->Priority = InPriority;
			FindResult->Permission = Result;
		}
	}
	else
	{
		Result = UBXFunctionLibrary::GetUniqueID();
		PlayAnimationPermissionMap.Add(InAnimationTag, FBXPlayAnimationPermission(InPriority, Result));
	}

	return Result;
}

void UBXAnimInstance::RevokePlayAnimationPermission(const FGameplayTag& InAnimationTag, int64 InPermission)
{
	if (FBXPlayAnimationPermission* FindResult = PlayAnimationPermissionMap.Find(InAnimationTag))
	{
		if (FindResult->Permission == InPermission)
		{
			PlayAnimationPermissionMap.Remove(InAnimationTag);
		}
	}
}

