#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

#include "BXEnums.h"
#include "BXGameplayTags.h"

#include "BXStructs.generated.h" 



// Object列表
USTRUCT(BlueprintType)
struct FBXObjectList
{
	GENERATED_USTRUCT_BODY()

public:
	// 对象列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UObject*> List;

};



// Actor列表
USTRUCT(BlueprintType)
struct FBXActorList
{
	GENERATED_USTRUCT_BODY()

public:
	// 角色列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> List;

};



// 骨骼选择器
USTRUCT(BlueprintType)
struct FBXBoneSelector
{
	GENERATED_USTRUCT_BODY()

public:
	// 骨骼名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneName = NAME_None;

#if WITH_EDITORONLY_DATA
	// 骨骼资源
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class USkeletalMesh> SkeletalMesh = nullptr;
#endif

};



// 模型创建器
USTRUCT(BlueprintType)
struct FBXMeshCreater
{
	GENERATED_USTRUCT_BODY()

public:
	// 模型名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MeshName = TEXT("DynamicMesh");

	// 使用静态模型 还是 骨骼模型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseStaticMesh = true;

	// 静态模型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "bUseStaticMesh"))
	TSoftObjectPtr<class UStaticMesh> StaticMesh = nullptr;

	// 动态模型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "!bUseStaticMesh"))
	TSoftObjectPtr<class USkeletalMesh> SkeletalMesh = nullptr;

	// 动态模型动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "!bUseStaticMesh"))
	TSoftObjectPtr<class UAnimationAsset> AnimSequence = nullptr;

	// 循环播放动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "!bUseStaticMesh"))
	bool bNeedLoop = false;

	// 是否进行挂接
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedAttach = true;

	// 挂接骨骼名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "bNeedAttach"))
	FBXBoneSelector AttachSocket;

	// 相对位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform RelativeTransform = FTransform::Identity;

	// 是否接受贴花
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
	bool bReceivesDecals = false;

	// bRenderCustomDepth & bRenderInDepthPass，为true时会进行这系列的Render参数设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render")
	bool bUseCustomDepth = false;

	// bRenderInMainPass
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render", Meta = (EditCondition = "bUseCustomDepth"))
	bool bRenderInMainPass = true;

	// 自定义深度的Mask
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render", Meta = (EditCondition = "bUseCustomDepth"))
	ERendererStencilMask CustomDepthStencilWriteMask = ERendererStencilMask::ERSM_Default;

	// 自定义深度的值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render", Meta = (EditCondition = "bUseCustomDepth"))
	int32 CustomDepthStencilValue = 0;
	
};



// 碰撞通道策略
USTRUCT(BlueprintType, Blueprintable)
struct FBXCollisionChannelInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FBXCollisionChannelInfo() {}
	FBXCollisionChannelInfo(ECollisionChannel Channel, ECollisionResponse Response) : NewCollisionChannel(Channel), NewCollisionResponse(Response) {}

public:
	// 碰撞通道
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> NewCollisionChannel = ECollisionChannel::ECC_WorldStatic;

	// 碰撞通道对应的策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ECollisionResponse> NewCollisionResponse = ECollisionResponse::ECR_Ignore;

};



// 属性比较器
USTRUCT(BlueprintType)
struct FBXAttributeComparator
{
	GENERATED_USTRUCT_BODY()

public:
	// 分子属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Molecular;

	// 分母属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Denominator;

	// 运算符号
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBXCompareSign Sign = EBXCompareSign::CS_Less;

	// 要比较的值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.0f;

};



USTRUCT(BlueprintType)
struct FBXChangeBuff
{
	GENERATED_USTRUCT_BODY()

public:
	FBXChangeBuff() {}
	FBXChangeBuff(int32 InBuffID, int32 LayerNum) : BuffID(InBuffID), ChangeLayer(LayerNum) {}

public:
	// BUFF资源ID
	UPROPERTY(BlueprintReadWrite)
	int32 BuffID = 0;

	// 层数信息(用于删除BUFF时，该值为零代表全部删除)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ChangeLayer = 1;

};



// 碰撞形状信息
USTRUCT(BlueprintType)
struct FBXShapeInformation
{
	GENERATED_USTRUCT_BODY()

public:
	// 碰撞盒类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBXShapeType ShapeType = EBXShapeType::ST_Box;

	// 碰撞盒大小(球体=X, 胶囊体=X,Y, 长方体=X,Y,Z)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ShapeSize = FVector(100.0f, 30.0f, 10.0f);

	// 挂接到哪个组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttachParent = NAME_None;

	// 挂接到哪个Socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBXBoneSelector Socket;

	// 相对方位
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Relation = FTransform::Identity;

};



// 时间轴资源信息
USTRUCT(BlueprintType)
struct FBXTimelineAssetInformation
{
	GENERATED_USTRUCT_BODY()

public:
	// 时间轴资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UBXTLAsset> AssetPath = nullptr;

	// 资源类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* AssetType = nullptr;

};



// 身体部位选取信息
USTRUCT(BlueprintType)
struct FBXBodyPartSelection
{
	GENERATED_USTRUCT_BODY()

public:
	// 部位拥有者
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBXHitReactionComponent* Owner = nullptr;

	// 部位类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BodyPart;

};



// 身体部位配置信息
USTRUCT(BlueprintType)
struct FBXBodyPartConfig
{
	GENERATED_USTRUCT_BODY()

public:
	// 组件名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ComponentName = NAME_None;

	// 骨骼列表(不填代表所有骨骼)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> BoneNames;

	// 是否允许碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanCollision = true;

	// 锁定的骨骼名称，不填则不能锁定该部位
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LockBoneName = NAME_None;

};



// 身体部位运行信息
USTRUCT(BlueprintType)
struct FBXBodyPartRTInformation
{
	GENERATED_USTRUCT_BODY()

public:
	FBXBodyPartRTInformation() {}

	FBXBodyPartRTInformation(const FBXBodyPartRTInformation& InOther)
	{
		PartType = InOther.PartType;
		Component = InOther.Component;
		BoneNames.Reset();
		BoneNames.Append(InOther.BoneNames);
		bCanCollision = InOther.bCanCollision;
		LockBoneName = InOther.LockBoneName;
	}

	FBXBodyPartRTInformation(FBXBodyPartRTInformation&& InOther)
	{
		PartType = InOther.PartType;
		Component = InOther.Component;
		BoneNames.Reset();
		BoneNames.Append(InOther.BoneNames);
		InOther.BoneNames.Reset();
		bCanCollision = InOther.bCanCollision;
		LockBoneName = InOther.LockBoneName;
	}

	FBXBodyPartRTInformation& operator=(const FBXBodyPartRTInformation& InOther)
	{
		if (this != &InOther)
		{
			PartType = InOther.PartType;
			Component = InOther.Component;
			BoneNames.Reset();
			BoneNames.Append(InOther.BoneNames);
			bCanCollision = InOther.bCanCollision;
			LockBoneName = InOther.LockBoneName;
		}

		return *this;
	}

	FBXBodyPartRTInformation& operator=(FBXBodyPartRTInformation&& InOther)
	{
		if (this != &InOther)
		{
			PartType = InOther.PartType;
			Component = InOther.Component;
			BoneNames.Reset();
			BoneNames.Append(InOther.BoneNames);
			InOther.BoneNames.Reset();
			bCanCollision = InOther.bCanCollision;
			LockBoneName = InOther.LockBoneName;
		}

		return *this;
	}

public:
	// 部位类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag PartType = BXGameplayTags::BXBodyPart_Default;

	// 组件对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Component = nullptr;

	// 骨骼列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> BoneNames;

	// 是否允许碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanCollision = true;

	// 锁定的骨骼名称，不填则不能锁定该部位
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LockBoneName = NAME_None;

};
