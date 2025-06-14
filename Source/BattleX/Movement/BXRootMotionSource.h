#pragma once

#include "CoreMinimal.h"
#include "GameFramework/RootMotionSource.h"

#include "BXRootMotionSource.generated.h"



USTRUCT()
struct BATTLEX_API FBXRootMotionSource_Uppercut : public FRootMotionSource
{
	GENERATED_USTRUCT_BODY()

public:
	FBXRootMotionSource_Uppercut();

	virtual ~FBXRootMotionSource_Uppercut() {}
	
	virtual FRootMotionSource* Clone() const override;

	virtual bool Matches(const FRootMotionSource* Other) const override;

	virtual bool MatchesAndHasSameState(const FRootMotionSource* Other) const override;

	virtual bool UpdateStateFrom(const FRootMotionSource* SourceToTakeStateFrom, bool bMarkForSimulatedCatchup = false) override;

	virtual void PrepareRootMotion(float SimulationTime, float MovementTickTime, const ACharacter& Character, const UCharacterMovementComponent& MoveComponent) override;

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FString ToSimpleString() const override;

	virtual void AddReferencedObjects(class FReferenceCollector& Collector) override;

protected:
	FVector GetPathOffset(float MoveFraction) const;

	FVector GetRelativeLocation(float MoveFraction) const;
	
public:
	// 移动方向
	UPROPERTY()
	FRotator Direction = FRotator::ZeroRotator;

	// 水平距离
	UPROPERTY()
	float Distance = 0.0f;

	// 最大高度
	UPROPERTY()
	float Height = 0.0f;

	// 移动时间映射曲线
	UPROPERTY()
	TObjectPtr<UCurveFloat> MoveTimeMappingCurve = nullptr;

	// 需要改变朝向
	UPROPERTY()
	bool bChangeOrientation = false;

	// 开始朝向
	UPROPERTY()
	FRotator StartOrientation = FRotator::ZeroRotator;
	
	// 结束朝向
	UPROPERTY()
	FRotator EndOrientation = FRotator::ZeroRotator;

	// 旋转时间映射曲线
	UPROPERTY()
	TObjectPtr<UCurveFloat> RotateTimeMappingCurve = nullptr;
	
};

template<>
struct TStructOpsTypeTraits<FBXRootMotionSource_Uppercut> : public TStructOpsTypeTraitsBase2<FBXRootMotionSource_Uppercut>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
