#pragma once

#include "CoreMinimal.h"

#include "BXManager.generated.h" 



// 管理器基类
UCLASS(Blueprintable)
class BATTLEX_API UBXManager : public UObject
{
	GENERATED_BODY()

#pragma region Important
public:
	virtual void Initialize();

	virtual void Deinitialize();

#pragma endregion Important
	
};
