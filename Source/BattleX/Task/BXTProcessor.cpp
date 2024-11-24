#include "BXTProcessor.h"

#include "BXCharacterMovementComponent.h"
#include "BXSettings.h"
#include "BXTLAsset.h"
#include "BXTLStructs.h"
#include "BXTask.h"
#include "BXHitReactionComponent.h"
#include "ShaderPrintParameters.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Kismet/KismetMathLibrary.h"



DEFINE_LOG_CATEGORY(BX_TP);



#pragma region Important
UWorld* UBXTProcessor::GetWorld() const
{
	if (!GetOuter())
	{
		return nullptr;
	}

	return GetOuter()->GetWorld();
}

void UBXTProcessor::StartTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData)
{
	if ((ExecuteFunctions & 1) > 0)
	{
		Start(InOutRTData, InOutRTSData, InOutRTTData);
	}

	if ((ExecuteFunctions & 2) > 0)
	{
		ScriptStart(InOutRTData, InOutRTSData, InOutRTTData);
	}

	UBXTProcessor::AddPendingTask(InOutRTData, InOutRTSData, InOutRTTData, UBXTProcessor::GenerateContextScope(InOutRTData, InOutRTTData), BXGameplayTags::BXTEvent_Start);
}

void UBXTProcessor::UpdateTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, float InDeltaTime)
{
	// 更新运行时间
	InOutRTTData.RunTime += InDeltaTime;
	
	if (InOutRTTData.NextTick < 0.0f)
	{
		return;
	}
	
	InOutRTTData.NextTick = FMath::Max(0.0f, InOutRTTData.NextTick - InDeltaTime);
	if (InOutRTTData.NextTick > 0.0f)
	{
		return;
	}

	if ((ExecuteFunctions & 4) > 0)
	{
		Update(InOutRTData, InOutRTSData, InOutRTTData, InDeltaTime);
	}

	if ((ExecuteFunctions & 8) > 0)
	{
		ScriptUpdate(InOutRTData, InOutRTSData, InOutRTTData, InDeltaTime);
	}
}

void UBXTProcessor::EndTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, EBXTLFinishReason InReason)
{
	if ((ExecuteFunctions & 16) > 0)
	{
		End(InOutRTData, InOutRTSData, InOutRTTData, InReason);
	}

	if ((ExecuteFunctions & 32) > 0)
	{
		ScriptEnd(InOutRTData, InOutRTSData, InOutRTTData, InReason);
	}

	UBXTProcessor::AddPendingTask(InOutRTData, InOutRTSData, InOutRTTData, UBXTProcessor::GenerateContextScope(InOutRTData, InOutRTTData), BXGameplayTags::BXTEvent_End);

	for (TArray<FBXTLTaskRTData>::TIterator It(InOutRTSData.RunningTasks); It; ++It)
	{
		// 判断地址是否相同
		if ((&(*It)) == (&InOutRTTData))
		{
			It.RemoveCurrent();
			break;
		}
	}
}

void UBXTProcessor::ChangeTaskTickRate(UPARAM(ref) FBXTLTaskRTData& InOutRTTData, float InRate)
{
	if ((ExecuteFunctions & 64) > 0)
	{
		ChangeTickRate(InOutRTTData, InRate);
	}

	if ((ExecuteFunctions & 128) > 0)
	{
		ScriptChangeTickRate(InOutRTTData, InRate);
	}
}

#pragma endregion Important



#pragma region GlobalAPI
bool UBXTProcessor::IsTaskCompleted(const FBXTLTaskRTData& InTaskData, EBXTLFinishReason& OutReason)
{
	OutReason = EBXTLFinishReason::FR_Interrupt;
	if (!InTaskData.Task)
	{
		return true;
	}

	if (InTaskData.bEarlyFinish)
	{
		return true;
	}

	OutReason = EBXTLFinishReason::FR_EndOfLife;
	switch (InTaskData.Task->LifeType)
	{
	case EBXTLifeType::L_Instant:
		return true;
	case EBXTLifeType::L_Duration:
		return InTaskData.RunTime >= InTaskData.Task->Duration;
	case EBXTLifeType::L_Timeline:
		return false;
	case EBXTLifeType::L_DurationTimeline:
		return InTaskData.RunTime >= InTaskData.Task->Duration;
	default:
		return true;
	}
}

bool UBXTProcessor::AddPendingTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, int64 InScope, const FGameplayTag& InEventTag)
{
	const UBXSettings* Settings = GetDefault<UBXSettings>();
	if (!Settings)
	{
		return false;
	}

	UBXTLAsset* Asset = InOutRTData.Timeline;
	if (!Asset)
	{
		return false;
	}

	if (!Asset->Sections.IsValidIndex(InOutRTSData.Index))
	{
		return false;
	}

	const FBXTLSection& Section = Asset->Sections[InOutRTSData.Index];
	if (!Section.TaskList.IsValidIndex(InOutRTTData.Index))
	{
		return false;
	}

	UBXTask* Task = Section.TaskList[InOutRTTData.Index];
	if (!Task)
	{
		return false;
	}

	FBXTEvent* EventPointer = Task->Events.Find(InEventTag);
	if (!EventPointer)
	{
		return false;
	}

	// 需要广播，则添加到广播列表
	if (EventPointer->bMulticast)
	{
		InOutRTSData.BroadcastTasks.Add(FBXTLBroadcastTaskInfo(InOutRTSData.Index * 1000 + InOutRTTData.Index, InEventTag));
	}

	for (TMap<TSoftObjectPtr<UBXTask>, float>::TIterator It(EventPointer->Event); It; ++It)
	{
		// 任务全量索引
		int32 FullIndex = UBXFunctionLibrary::GetSoftTaskFullIndex(Asset, It->Key);
		if (FullIndex < 0)
		{
			continue;
		}

		// 任务局部索引
		int32 LocalIndex = FullIndex % 1000;

		// 延迟触发的任务，添加到待处理列表
		if (It->Value > 0.0f)
		{
			InOutRTSData.PendingTasks.Add(FBXTLPendingTaskInfo(LocalIndex, InScope, InOutRTSData.RunTime + It->Value));

			continue;
		}

		// 如果开启了帧内待处理列表，则添加到该列表
		if (InOutRTSData.TaskStackInFrame.Num() > 0)
		{
			bool Flag = true;
			for (TArray<FInt64Vector2>::TIterator TS(InOutRTSData.TaskStackInFrame); TS; ++TS)
			{
				if (TS->X == LocalIndex)
				{
					Flag = false;
					break;
				}
			}
			
			if (Flag)
			{
				InOutRTSData.TaskStackInFrame.Add(FInt64Vector2(LocalIndex, InScope));
			}
		}
		else
		{
			InOutRTSData.PendingTasks.Add(FBXTLPendingTaskInfo(LocalIndex, InScope, InOutRTSData.RunTime));
		}
	}

	return true;
}

int64 UBXTProcessor::GenerateContextScope(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData)
{
	if (!IsValid(InOutRTData.Timeline) || !IsValid(InOutRTTData.Task))
	{
		return 0;
	}
	
	int64 NewScope = UBXFunctionLibrary::GetUniqueID();
	InOutRTData.ScopeGraph.Add(NewScope, FInt64Vector2(InOutRTTData.ParentScope, UBXFunctionLibrary::GetTaskFullIndex(InOutRTData.Timeline, InOutRTTData.Task)));

	return NewScope;
}

void UBXTProcessor::GetTargetComponentList(const FBXTLRunTimeData& InRTData, const FBXTLTaskRTData& InTaskData, TArray<USceneComponent*>& OutComponents)
{
	OutComponents.Reset();
	if (!IsValid(InTaskData.Task) || !IsValid(InRTData.Timeline))
	{
		return;
	}
	
	for (int32 i = 0; i < (int32)EBXTTargetType::T_TMax; ++i)
	{
		if ((InTaskData.Task->TargetTypes & (1 << i)) <= 0)
		{
			continue;
		}

		switch ((EBXTTargetType)i)
		{
		case EBXTTargetType::T_Owner:
			if (IsValid(InRTData.Owner))
			{
				OutComponents.AddUnique(InRTData.Owner->GetRootComponent());
			}
			break;
		case EBXTTargetType::T_Instigator:
			if (IsValid(InRTData.Instigator))
			{
				OutComponents.AddUnique(InRTData.Instigator->GetRootComponent());
			}
			break;
		case EBXTTargetType::T_Trigger:
			if (IsValid(InRTData.Triggerer))
			{
				OutComponents.AddUnique(InRTData.Triggerer->GetRootComponent());
			}
			break;
		case EBXTTargetType::T_LockTargets:
			for (TArray<FBXBodyPartSelection>::TConstIterator It(InRTData.LockParts); It; ++It)
			{
				if (!IsValid(It->Owner))
				{
					continue;
				}

				FBXBodyPartRTInformation PartInformation;
				if (!It->Owner->GetBodyPartByType(It->BodyPart, PartInformation))
				{
					continue;
				}
				
				OutComponents.AddUnique(PartInformation.Component);
			}
			break;
		case EBXTTargetType::T_CollisionResults:
			for (TArray<FBXTInputInfo>::TIterator It(InTaskData.Task->CollisionInputDatas); It; ++It)
			{
				FBXTHitResults* HitResults = UBXTProcessor::ReadContextData<FBXTHitResults>(InRTData, UBXFunctionLibrary::GetSoftTaskFullIndex(InRTData.Timeline, It->DataTask), It->DataTag, InTaskData.ParentScope);
				if (!HitResults)
				{
					continue;
				}

				for (TArray<FHitResult>::TIterator HR(HitResults->Results); HR; ++HR)
				{
					OutComponents.AddUnique(HR->GetComponent());
				}
			}
			break;
		default:
			break;
		}
	}
}

void UBXTProcessor::GetTargetActorList(const FBXTLRunTimeData& InRTData, const FBXTLTaskRTData& InTaskData, TArray<AActor*>& OutActors)
{
	TArray<USceneComponent*> Components;
	UBXTProcessor::GetTargetComponentList(InRTData, InTaskData, Components);

	OutActors.Reset();
	for (TArray<USceneComponent*>::TIterator It(Components); It; ++It)
	{
		OutActors.AddUnique((*It)->GetOwner());
	}
}

bool UBXTProcessor::AnalyzeTransformCreater(const FBXTLRunTimeData& InRTData, const FBXTLTaskRTData& InTaskData, AActor* InTarget, const FBXTTransformCreater& InCreater, FBXTTransformCreaterResult& OutResult)
{
	USceneComponent* OriginComponent = nullptr;
	FTransform OriginTransform = FTransform::Identity;
	if (InCreater.OriginType == EBXTCoordinateType::C_Target)
	{
		if (!IsValid(InTarget))
		{
			return false;
		}
		
		OriginComponent = InTarget->GetRootComponent();
		OriginTransform = OriginComponent->GetComponentTransform();
	}
	else
	{
		OriginComponent = UBXTProcessor::AnalyzeTransformCreaterCoordinateType(InRTData, InTaskData, InCreater, true, OriginTransform);
		if (!IsValid(OriginComponent) && InCreater.OriginType != EBXTCoordinateType::C_World)
		{
			return false;
		}
	}
	OriginTransform.SetLocation(OriginTransform.TransformPosition(InCreater.OriginDelta));

	USceneComponent* XAxisComponent = OriginComponent;
	FTransform XAxisTransform = OriginTransform;
	if (InCreater.XAxisType != EBXTCoordinateType::C_TMax)
	{
		if (InCreater.XAxisType == EBXTCoordinateType::C_Target)
		{
			if (IsValid(InTarget))
			{
				return false;
			}
		
			XAxisComponent = InTarget->GetRootComponent();
			XAxisTransform = XAxisComponent->GetComponentTransform();
		}
		else
		{
			XAxisComponent = UBXTProcessor::AnalyzeTransformCreaterCoordinateType(InRTData, InTaskData, InCreater, false, XAxisTransform);
			if (!IsValid(XAxisComponent) && InCreater.XAxisType != EBXTCoordinateType::C_World)
			{
				return false;
			}
		}
	}

	if (InCreater.ConnectionType != EBXTConnectionType::C_TMax)
	{
		FVector Up = OriginTransform.GetUnitAxis(EAxis::Z);
		FVector Delta = XAxisTransform.GetLocation() - OriginTransform.GetLocation();
		switch (InCreater.ConnectionType)
		{
		case EBXTConnectionType::C_FromOriginIn2D:
			Delta = Delta.GetSafeNormal2D();
			break;
		case EBXTConnectionType::C_FromOriginIn3D:
			Delta = Delta.GetSafeNormal();
			break;
		case EBXTConnectionType::C_FromXAxisIn2D:
			Up = XAxisTransform.GetUnitAxis(EAxis::Z);
			Delta = Delta.GetSafeNormal2D() * -1.0f;
			break;
		case EBXTConnectionType::C_FromXAxisIn3D:
			Up = XAxisTransform.GetUnitAxis(EAxis::Z);
			Delta = Delta.GetSafeNormal() * -1.0f;
			break;
		default:
			return false;
		}

		XAxisTransform.SetRotation(UKismetMathLibrary::MakeRotFromXZ(Delta, Up).Quaternion());
	}

	OutResult.OriginComponent = OriginComponent;
	OutResult.XAxisComponent = XAxisComponent;
	OutResult.NoOffsetResult.SetLocation(OriginTransform.GetLocation());
	OutResult.NoOffsetResult.SetRotation(XAxisTransform.GetRotation());
	OutResult.NoOffsetResult.SetScale3D(FVector::OneVector);
	OutResult.Result = InCreater.PostOffset * OutResult.NoOffsetResult;
	
	return true;
}

bool UBXTProcessor::AnalyzeTransformCreaterList(const FBXTLRunTimeData& InRTData, const FBXTLTaskRTData& InTaskData, AActor* InTarget, const TArray<FBXTTransformCreater>& InCreaterList, FBXTTransformCreaterResult& OutResult)
{
	for (TArray<FBXTTransformCreater>::TConstIterator It(InCreaterList); It; ++It)
	{
		if (UBXTProcessor::AnalyzeTransformCreater(InRTData, InTaskData, InTarget, *It, OutResult))
		{
			return true;
		}
	}

	return false;
}

USceneComponent* UBXTProcessor::AnalyzeTransformCreaterCoordinateType(const FBXTLRunTimeData& InRTData, const FBXTLTaskRTData& InTaskData, const FBXTTransformCreater& InCreater, bool bUseOrigin, FTransform& OutTransform)
{
	EBXTCoordinateType CoordinateType = bUseOrigin ? InCreater.OriginType : InCreater.XAxisType;
	int32 FullIndex = bUseOrigin ? UBXFunctionLibrary::GetSoftTaskFullIndex(InRTData.Timeline, InCreater.OriginInputTask) : UBXFunctionLibrary::GetSoftTaskFullIndex(InRTData.Timeline, InCreater.XAxisInputTask);
	const FGameplayTag& DataTag = bUseOrigin ? InCreater.OriginInputTag : InCreater.XAxisInputTag;
	FName BoneName = bUseOrigin ? InCreater.OriginBoneName.BoneName : InCreater.XAxisBoneName.BoneName;

	// 根据类型找到坐标系和坐标系参照者
	USceneComponent* OutResult = nullptr;
	switch (CoordinateType)
	{
	case EBXTCoordinateType::C_Owner:
		OutResult = IsValid(InRTData.Owner) ? InRTData.Owner->GetRootComponent() : nullptr;
		if (OutResult)
		{
			OutTransform = OutResult->GetComponentTransform();
		}
		break;
	case EBXTCoordinateType::C_Instigator:
		OutResult = IsValid(InRTData.Instigator) ? InRTData.Instigator->GetRootComponent() : nullptr;
		if (OutResult)
		{
			OutTransform = OutResult->GetComponentTransform();
		}
		break;
	case EBXTCoordinateType::C_Trigger:
		OutResult = IsValid(InRTData.Triggerer) ? InRTData.Triggerer->GetRootComponent() : nullptr;
		if (OutResult)
		{
			OutTransform = OutResult->GetComponentTransform();
		}
		break;
	case EBXTCoordinateType::C_LockTarget:
		for (TArray<FBXBodyPartSelection>::TConstIterator It(InRTData.LockParts); It; ++It)
		{
			if (IsValid(It->Owner) && IsValid(It->Owner->GetOwner()))
			{
				OutResult = It->Owner->GetOwner()->GetRootComponent();
				if (OutResult)
				{
					OutTransform = OutResult->GetComponentTransform();
				}
				break;
			}
		}
		break;
	case EBXTCoordinateType::C_LockPart:
		for (TArray<FBXBodyPartSelection>::TConstIterator It(InRTData.LockParts); It; ++It)
		{
			FBXBodyPartRTInformation BPRTInfo;
			if (IsValid(It->Owner) && It->Owner->GetBodyPartByType(It->BodyPart, BPRTInfo))
			{
				OutResult = BPRTInfo.Component;
				if (OutResult)
				{
					OutTransform = OutResult->GetSocketTransform(BPRTInfo.LockBoneName);
				}
				break;
			}
		}
		break;
	case EBXTCoordinateType::C_Special:
		if (FBXTHitResults* HitResults = UBXTProcessor::ReadContextData<FBXTHitResults>(InRTData, FullIndex, DataTag, InTaskData.ParentScope))
		{
			if (HitResults->Results.Num() > 0)
			{
				OutResult = HitResults->Results[0].GetComponent();
				if (OutResult)
				{
					OutTransform = OutResult->GetComponentTransform();
				}
			}
		}
		break;
	case EBXTCoordinateType::C_World:
		if (FTransform* TransformResults = UBXTProcessor::ReadContextData<FTransform>(InRTData, FullIndex, DataTag, InTaskData.ParentScope))
		{
			OutResult = nullptr;
			OutTransform = *TransformResults;
		}
		break;
	default:
		OutResult = nullptr;
		OutTransform = FTransform::Identity;
		break;
	}

	// 通过骨骼名称进行二次查询
	if (CoordinateType != EBXTCoordinateType::C_LockPart && CoordinateType != EBXTCoordinateType::C_World)
	{
		if (OutResult && !BoneName.IsNone())
		{
			if (USceneComponent* SocketOwner = UBXFunctionLibrary::GetSceneComponentBySocketName(OutResult->GetOwner(), BoneName))
			{
				OutResult = SocketOwner;
				OutTransform = OutResult->GetSocketTransform(BoneName);
			}
		}
	}

	return OutResult;
}

bool UBXTProcessor::GetTargetTransformByWorldTime(AActor* InTarget, float InWorldTime, FTransform& OutTransform)
{
	if (!IsValid(InTarget))
	{
		return false;
	}

	UBXCharacterMovementComponent* CMC = InTarget->FindComponentByClass<UBXCharacterMovementComponent>();
	if (!IsValid(CMC))
	{
		return false;
	}

	OutTransform = CMC->GetHistoryTransformByTime(InWorldTime);

	return true;
}

#pragma endregion GlobalAPI
