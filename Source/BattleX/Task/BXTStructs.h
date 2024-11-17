#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "UObject/NoExportTypes.h"

#include "GameplayTagContainer.h"
#include "InstancedStruct.h"

#include "BXEnums.h"
#include "BXTEnums.h"
#include "BXStructs.h"
#include "BXFunctionLibrary.h"

#include "BXTStructs.generated.h" 



// 触发事件
USTRUCT(BlueprintType)
struct FBXTEvent
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTEvent() {}
	
	FBXTEvent(const FBXTEvent& InOther)
	{
		bMulticast = InOther.bMulticast;
		Event.Append(InOther.Event);
	}

	FBXTEvent(FBXTEvent&& InOther)
	{
		bMulticast = InOther.bMulticast;
		Event.Reset();
		Event.Append(InOther.Event);
		InOther.Event.Reset();
	}

	FBXTEvent& operator=(const FBXTEvent& InOther)
	{
		if (this != &InOther)
		{
			bMulticast = InOther.bMulticast;
			Event.Append(InOther.Event);
		}

		return *this;
	}

	FBXTEvent& operator=(FBXTEvent&& InOther)
	{
		if (this != &InOther)
		{
			bMulticast = InOther.bMulticast;
			Event.Reset();
			Event.Append(InOther.Event);
			InOther.Event.Reset();
		}

		return *this;
	}

public:
	// 需要进行网络广播
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bMulticast = false;
	
	// 触发事件，延迟触发时间
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	TMap<TSoftObjectPtr<class UBXTask>, float> Event;

	
#if WITH_EDITORONLY_DATA
public:
	int64 GetUniqueID()
	{
		if (UniqueID <= 0)
		{
			UniqueID = UBXFunctionLibrary::GetUniqueID();
		}
		
		return UniqueID;
	}
	
private:
	UPROPERTY()
	int64 UniqueID = 0;
#endif

};



// 读取数据的信息
USTRUCT(BlueprintType)
struct BATTLEX_API FBXTInputInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTInputInfo() {}
	
	FBXTInputInfo(const FBXTInputInfo& InOther)
	{
	#if WITH_EDITORONLY_DATA
		DisplayName = InOther.DisplayName;
		StructType = InOther.StructType;
	#endif
		
		DataTask = InOther.DataTask;
		DataTag = InOther.DataTag;
	}

	FBXTInputInfo(FBXTInputInfo&& InOther)
	{
	#if WITH_EDITORONLY_DATA
		DisplayName = InOther.DisplayName;
		StructType = InOther.StructType;
	#endif
		
		DataTask = InOther.DataTask;
		DataTag = InOther.DataTag;
	}

	FBXTInputInfo& operator=(const FBXTInputInfo& InOther)
	{
		if (this != &InOther)
		{
		#if WITH_EDITORONLY_DATA
			DisplayName = InOther.DisplayName;
			StructType = InOther.StructType;
		#endif
			
			DataTask = InOther.DataTask;
			DataTag = InOther.DataTag;
		}

		return *this;
	}

	FBXTInputInfo& operator=(FBXTInputInfo&& InOther)
	{
		if (this != &InOther)
		{
		#if WITH_EDITORONLY_DATA
			DisplayName = InOther.DisplayName;
			StructType = InOther.StructType;
		#endif
			
			DataTask = InOther.DataTask;
			DataTag = InOther.DataTag;
		}

		return *this;
	}

public:
	// 获取哪个任务生产的数据
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class UBXTask> DataTask = nullptr;

	// 获取名称为"xxx"的数据
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DataTag;

	
#if WITH_EDITORONLY_DATA
public:
	void SetDisplayName(const FName& InName)
	{
		DisplayName = InName;
	}
	
	FName GetDisplayName()
	{
		return DisplayName;
	}
	
	void SetStructType(UScriptStruct* InStructType)
	{
		StructType = InStructType;
	}
	
	UScriptStruct* GetStructType()
	{
		return StructType;
	}
	
	void SetUniqueID(int64 InID)
	{
		if (InID > 0)
		{
			UniqueID = InID;
		}
	}
	
	int64 GetUniqueID()
	{
		if (UniqueID <= 0)
		{
			UniqueID = UBXFunctionLibrary::GetUniqueID();
		}
		
		return UniqueID;
	}

private:
	// 输入的针脚名称
	UPROPERTY(EditDefaultsOnly)
	FName DisplayName = NAME_None;

	// 输入的结构体类型
	UPROPERTY(EditDefaultsOnly)
	UScriptStruct* StructType = nullptr;
	
	UPROPERTY()
	int64 UniqueID = 0;
#endif

};



// 输出数据的信息
USTRUCT(BlueprintType)
struct BATTLEX_API FBXTOutputInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTOutputInfo() {}

	FBXTOutputInfo(FGameplayTag InTag, UScriptStruct* InStructType) : DataTag(InTag), StructType(InStructType) {}

	FBXTOutputInfo(const FBXTOutputInfo& InOther)
	{
		DataTag = InOther.DataTag;
		StructType = InOther.StructType;
	}

	FBXTOutputInfo(FBXTOutputInfo&& InOther)
	{
		DataTag = InOther.DataTag;
		StructType = InOther.StructType;
	}

	FBXTOutputInfo& operator=(const FBXTOutputInfo& InOther)
	{
		if (this != &InOther)
		{
			DataTag = InOther.DataTag;
			StructType = InOther.StructType;
		}

		return *this;
	}

	FBXTOutputInfo& operator=(FBXTOutputInfo&& InOther)
	{
		if (this != &InOther)
		{
			DataTag = InOther.DataTag;
			StructType = InOther.StructType;
		}

		return *this;
	}

public:
	// 输出的数据名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DataTag;

	// 输出的结构体类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UScriptStruct* StructType = nullptr;

	
#if WITH_EDITORONLY_DATA
public:
	int64 GetUniqueID()
	{
		if (UniqueID <= 0)
		{
			UniqueID = UBXFunctionLibrary::GetUniqueID();
		}
		
		return UniqueID;
	}

private:
	UPROPERTY()
	int64 UniqueID = 0;
#endif
	
};



// 32位整形封装
USTRUCT(BlueprintType)
struct FBXTInt32
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTInt32() {}
	FBXTInt32(int32 InValue) { Value = InValue; }
	
public:
	// 数值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Value = 0;

};



// 32位浮点封装
USTRUCT(BlueprintType)
struct FBXTFloat
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTFloat() {}
	FBXTFloat(float InValue) { Value = InValue; }
	
public:
	// 数值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Value = 0.0f;

};



// 指针封装
USTRUCT(BlueprintType)
struct FBXTObject
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTObject() {}
	FBXTObject(UObject* InObject) { Object = InObject; }
	FBXTObject(FSoftObjectPath InPath) { Object = InPath; }

public:
	// 对象指针
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UObject> Object = nullptr;

};



// 碰撞信息
USTRUCT(BlueprintType)
struct FBXTHitResults
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTHitResults()
	{
		Results.Empty(10);
	}

public:
	// 碰撞信息列表
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FHitResult> Results;

};



// 坐标系创建器
USTRUCT(BlueprintType)
struct BATTLEX_API FBXTTransformCreater
{
	GENERATED_USTRUCT_BODY()

public:
	FBXTTransformCreater() {}
	
	FBXTTransformCreater(EBXTCoordinateType InOrigin) : OriginType(InOrigin) {}

public:
	// 原点选取规则
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBXTCoordinateType OriginType = EBXTCoordinateType::C_Owner;
	
	// 特定原点输入数据
	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<class UBXTask> OriginInputTask = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (EditCondition = "OriginType == EBXTCoordinateType::C_Special || OriginType == EBXTCoordinateType::C_World", EditConditionHides))
	FGameplayTag OriginInputTag = FGameplayTag::EmptyTag;
	
	// 原点选取的骨骼名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "OriginType != EBXTCoordinateType::C_LockPart && OriginType != EBXTCoordinateType::C_World", EditConditionHides))
	FBXBoneSelector OriginBoneName;

	// 原点偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OriginDelta = FVector::ZeroVector;
	

	// X轴选取规则(不填，则默认使用原点的坐标系轴向)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBXTCoordinateType XAxisType = EBXTCoordinateType::C_TMax;
	
	// 特定X轴输入数据ID
	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<class UBXTask> XAxisInputTask = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Meta = (EditCondition = "XAxisType == EBXTCoordinateType::C_Special || XAxisType == EBXTCoordinateType::C_World", EditConditionHides))
	FGameplayTag XAxisInputTag = FGameplayTag::EmptyTag;

	// X轴选取的骨骼名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "XAxisType != EBXTCoordinateType::C_LockPart && XAxisType != EBXTCoordinateType::C_World && XAxisType != EBXTCoordinateType::C_TMax", EditConditionHides))
	FBXBoneSelector XAxisBoneName;

	// 使用连线作为X轴的轴向
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "XAxisType != EBXTCoordinateType::C_TMax", EditConditionHides))
	EBXTConnectionType ConnectionType = EBXTConnectionType::C_TMax;


	// 创建完坐标系后的总体偏移
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTransform PostOffset;


#if WITH_EDITORONLY_DATA
public:
	int64 GetOriginInputUniqueID()
	{
		if (OriginInputUniqueID <= 0)
		{
			OriginInputUniqueID = UBXFunctionLibrary::GetUniqueID();
		}

		return OriginInputUniqueID;
	}

	int64 GetXAxisInputUniqueID()
	{
		if (XAxisInputUniqueID <= 0)
		{
			XAxisInputUniqueID = UBXFunctionLibrary::GetUniqueID();
		}

		return XAxisInputUniqueID;
	}
	
private:
	UPROPERTY()
	int64 OriginInputUniqueID = 0;

	UPROPERTY()
	int64 XAxisInputUniqueID = 0;
#endif
	
};



// 坐标系创建器解析结果
USTRUCT(BlueprintType)
struct BATTLEX_API FBXTTransformCreaterResult
{
	GENERATED_USTRUCT_BODY()

public:
	// 结果
	UPROPERTY(Transient, BlueprintReadWrite)
	FTransform Result;

	// 无偏移修正结果
	UPROPERTY(Transient, BlueprintReadWrite)
	FTransform NoOffsetResult;

	// 坐标原点组件
	UPROPERTY(Transient, BlueprintReadWrite)
	USceneComponent* OriginComponent = nullptr;

	// 坐标X轴组件
	UPROPERTY(Transient, BlueprintReadWrite)
	USceneComponent* XAxisComponent = nullptr;
	
};
