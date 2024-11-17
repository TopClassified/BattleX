#pragma once

#include "CoreMinimal.h"
#include "CollisionQueryParams.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BXCollision.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(BXCOLLISION, Log, All);



UENUM(BlueprintType)
enum class EBXCDirection : uint8
{
	D_RightToLeft               = 0               UMETA(DisplayName = "从右向左"),
	D_LeftToRight                                 UMETA(DisplayName = "从左向右"),
	D_InToOut                                     UMETA(DisplayName = "从里向外"),
};



// 碰撞筛选器
USTRUCT(BlueprintType)
struct BATTLEX_API FBXCFilter
{
	GENERATED_USTRUCT_BODY()

public:
	// 是否忽略自己
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIgnoreSelf = true;

	// 可以通过筛选的父类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UObject>> ClassTypes;

	// 不可以通过筛选的父类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UObject>> IgnoreClassTypes;

	// 可以通过筛选的Actor标签
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ActorTags;

	// 不可以通过筛选的Actor标签
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> IgnoreActorTags;

	// 可以通过筛选的Component标签
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ComponentTags;

	// 不可以通过筛选的Component标签
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> IgnoreComponentTags;

public:
	void operator=(const FBXCFilter& Other)
	{
		bIgnoreSelf = Other.bIgnoreSelf;

		ClassTypes.Empty();
		ClassTypes.Append(Other.ClassTypes);

		IgnoreClassTypes.Empty();
		IgnoreClassTypes.Append(Other.IgnoreClassTypes);

		ActorTags.Empty();
		ActorTags.Append(Other.ActorTags);

		IgnoreActorTags.Empty();
		IgnoreActorTags.Append(Other.IgnoreActorTags);

		ComponentTags.Empty();
		ComponentTags.Append(Other.ComponentTags);

		IgnoreComponentTags.Empty();
		IgnoreComponentTags.Append(Other.IgnoreComponentTags);
	}

	void Reset()
	{
		bIgnoreSelf = true;
		ClassTypes.Empty();
		IgnoreClassTypes.Empty();
		ActorTags.Empty();
		IgnoreActorTags.Empty();
		ComponentTags.Empty();
		IgnoreComponentTags.Empty();
	}
};



// 碰撞参数
USTRUCT(BlueprintType)
struct BATTLEX_API FBXCParameter
{
	GENERATED_USTRUCT_BODY()

public:
	FBXCParameter() {}

	FBXCParameter(AActor* InRequest, FVector CheckLocation, FRotator CheckRotation) : Requester(InRequest), StartLocation(CheckLocation), StartRotation(CheckRotation) {}

	FBXCParameter(AActor* InRequest, FVector StartLoc, FRotator StartRot, FVector EndLoc, FRotator EndRot) : Requester(InRequest), StartLocation(StartLoc), StartRotation(StartRot), EndLocation(EndLoc), EndRotation(EndRot) {}

public:
	// 请求碰撞检查的对象
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	AActor* Requester = nullptr;

	// 起始位置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector StartLocation = FVector::ZeroVector;

	// 起始朝向
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRotator StartRotation = FRotator::ZeroRotator;

	// 终止位置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector EndLocation = FVector::ZeroVector;

	// 终止朝向
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRotator EndRotation = FRotator::ZeroRotator;

	// 缩放大小
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Scale = FVector::OneVector;

};



// 碰撞策略
USTRUCT(BlueprintType)
struct BATTLEX_API FBXCStrategy
{
	GENERATED_USTRUCT_BODY()

public:
	// Object的碰撞类型
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	// 碰撞筛选器
	UPROPERTY(EditDefaultsOnly)
	FBXCFilter Filter;

};

// 球体碰撞
USTRUCT(BlueprintType)
struct BATTLEX_API FBXCSSphere : public FBXCStrategy
{
	GENERATED_USTRUCT_BODY()

public:
	// 球体大小
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 0.001f))
	float SpereSize = 50.0f;

	// 启用扫描
	UPROPERTY(EditDefaultsOnly)
	bool bUseSweep = false;

};

// 胶囊体碰撞
USTRUCT(BlueprintType)
struct BATTLEX_API FBXCSCapsule : public FBXCStrategy
{
	GENERATED_USTRUCT_BODY()

public:
	// 胶囊体大小(半径、半高)
	UPROPERTY(EditDefaultsOnly)
	FVector2D CapsuleSize = FVector2D(20.0f, 50.0f);

	// 启用扫描
	UPROPERTY(EditDefaultsOnly)
	bool bUseSweep = false;

	// 启用扫描 且 起始朝向和终止朝向不一致时，该变量起效，值越小，碰撞精度越高，性能消耗越大！
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 5.0f, ClampMax = 90.0f, EditCondition = "bUseSweep", EditConditionHides))
	float AngleStep = 15.0f;

};

// 圆柱体碰撞
USTRUCT(BlueprintType)
struct BATTLEX_API FBXCSCylinder : public FBXCStrategy
{
	GENERATED_USTRUCT_BODY()

public:
	// 圆柱体大小(半径、半高)
	UPROPERTY(EditDefaultsOnly)
	FVector2D CylinderSize = FVector2D(50.0f, 100.0f);

};

// 空心圆柱体碰撞
USTRUCT(BlueprintType)
struct BATTLEX_API FBXCSHollowCylinder : public FBXCStrategy
{
	GENERATED_USTRUCT_BODY()

public:
	// 空心圆柱体大小(内半径、外半径、半高)
	UPROPERTY(EditDefaultsOnly)
	FVector HollowCylinderSize = FVector(100.0f, 200.0, 100.0f);

	// 角度步进越小，碰撞精度越高，性能消耗越大！
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 10.0f, ClampMax = 60.0f))
	float AngleStep = 30.0f;

};

// 长方体碰撞
USTRUCT(BlueprintType)
struct BATTLEX_API FBXCSBox : public FBXCStrategy
{
	GENERATED_USTRUCT_BODY()

public:
	// 长方体大小(半长、半宽、半高)
	UPROPERTY(EditDefaultsOnly)
	FVector BoxSize = FVector(30.0f, 30.0f, 50.0f);

	// 启用扫描
	UPROPERTY(EditDefaultsOnly)
	bool bUseSweep = false;

	// 启用扫描 且 起始朝向和终止朝向不一致时，该变量起效，值越小，碰撞精度越高，性能消耗越大！
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 5.0f, ClampMax = 90.0f, EditCondition = "bUseSweep", EditConditionHides))
	float AngleStep = 15.0f;

};

// 扇柱体碰撞
USTRUCT(BlueprintType)
struct BATTLEX_API FBXCSSector : public FBXCStrategy
{
	GENERATED_USTRUCT_BODY()

public:
	// 扇柱体碰撞信息(最小半径、最大半径、半高、半角)
	UPROPERTY(EditDefaultsOnly)
	FVector4 SectorInfo = FVector4(50.0f, 200.0f, 30.0f, 45.0f);

	// 扇柱体碰撞的精细检测方向
	UPROPERTY(EditDefaultsOnly)
	EBXCDirection Direction = EBXCDirection::D_RightToLeft;

	// 角度步进越小，碰撞精度越高，性能消耗越大！
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 5.0f, ClampMax = 60.0f))
	float AngleStep = 15.0f;

	// 扫描步进次数
	UPROPERTY(EditDefaultsOnly)
	int32 SweepStep = 10;

};






UCLASS(Blueprintable)
class BATTLEX_API UBXCollisionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 检查碰撞结果
	UFUNCTION(BlueprintCallable)
	static bool CheckCollisionResult(AActor* InRequester, const FHitResult& InResult, const FBXCFilter& InFilter);

	// 碰撞结果去重合并
	UFUNCTION(BlueprintCallable)
	static void CombineCollisionResults(const TArray<FHitResult>& InSourceList, TArray<FHitResult>& InOutList);
	
	// 球体碰撞检查
	UFUNCTION(BlueprintCallable)
	static TArray<FHitResult> SphereCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, float SpereSize, const FBXCFilter& Filter);

	// 胶囊体碰撞检查
	UFUNCTION(BlueprintCallable)
	static TArray<FHitResult> CapsuleCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector2D CapsuleSize, const FBXCFilter& Filter, float AngleStep = 30.0f);

	// 圆柱体碰撞检查
	UFUNCTION(BlueprintCallable)
	static TArray<FHitResult> CylinderCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector2D CylinderSize, const FBXCFilter& Filter);

	// 空心圆柱体碰撞检查
	UFUNCTION(BlueprintCallable)
	static TArray<FHitResult> HollowCylinderCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector HollowCylinderSize, const FBXCFilter& Filter, float AngleStep = 30.0f);

	// 长方体碰撞检查
	UFUNCTION(BlueprintCallable)
	static TArray<FHitResult> BoxCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector BoxSize, const FBXCFilter& Filter, float AngleStep = 30.0f);

	// 扇柱体碰撞检查
	UFUNCTION(BlueprintCallable)
	static TArray<FHitResult> SectorCheck(const FBXCParameter& Parameter, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, FVector4 SectorSize, EBXCDirection Direction, const FBXCFilter& Filter, float AngleStep = 30.0f, int32 SweepStep = 10);

};
