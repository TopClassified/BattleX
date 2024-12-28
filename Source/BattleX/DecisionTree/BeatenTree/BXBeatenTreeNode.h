#pragma once

#include "CoreMinimal.h"

#include "BXCurves.h"
#include "BXGameplayTags.h"
#include "BXDecisionTreeNode.h"

#include "BXBeatenTreeNode.generated.h"



UCLASS()
class BATTLEX_API UBXBeatenTreeNode : public UBXDecisionTreeNode
{
	GENERATED_BODY()
	
public:
	UBXBeatenTreeNode();

public:
	// 新硬直状态
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag StunState = BXGameplayTags::BXStunState_NoStun;

	// 动画库资源标签
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AnimLibraryTag;
	
	// 受击位移信息(X:时长 Y:平面距离 Z:高度)
	UPROPERTY(EditDefaultsOnly)
	FVector MotionDistance = FVector::ZeroVector;
	
	// 受击位移曲线
	UPROPERTY(EditDefaultsOnly)
	FBXRemapFloatCurve MotionCurve;

#if WITH_EDITOR
public:
	void SetNodeTitle(const FText& NewTitle) override;

protected:
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

};
