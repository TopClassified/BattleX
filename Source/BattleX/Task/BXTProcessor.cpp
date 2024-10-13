#include "BXTProcessor.h"

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

void UBXTProcessor::StartTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, UBXTask* InTask)
{
	if ((ExecuteFunctions & 1) > 0)
	{
		Start(InOutRTData, InOutRTSData, InOutRTTData, InTask);
	}

	if ((ExecuteFunctions & 2) > 0)
	{
		ScriptStart(InOutRTData, InOutRTSData, InOutRTTData, InTask);
	}
}

void UBXTProcessor::UpdateTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, UBXTask* InTask, float InDeltaTime)
{
	if (InOutRTTData.NextTick < 0.0f)
	{
		if (InOutRTData.StaticData)
		{
			FString OutInfo = FString::FromInt(InOutRTData.StaticData->ID) + TEXT("  Task FullIndex:") + FString::FromInt(InOutRTSData.Index * 1000 + InOutRTTData.Index);
			UE_LOG(BX_TP, Log, TEXT("This Task(%s) Will Never Execute The Tick Logic."), *OutInfo);
		}

		return;
	}
	
	InOutRTTData.NextTick = FMath::Max(0.0f, InOutRTTData.NextTick - InDeltaTime);
	if (InOutRTTData.NextTick > 0.0f)
	{
		return;
	}

	if ((ExecuteFunctions & 4) > 0)
	{
		Update(InOutRTData, InOutRTSData, InOutRTTData, InTask, InDeltaTime);
	}

	if ((ExecuteFunctions & 8) > 0)
	{
		ScriptUpdate(InOutRTData, InOutRTSData, InOutRTTData, InTask, InDeltaTime);
	}
}

void UBXTProcessor::EndTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, UBXTask* InTask, EBXTLFinishReason InReason)
{
	if ((ExecuteFunctions & 16) > 0)
	{
		End(InOutRTData, InOutRTSData, InOutRTTData, InTask, InReason);
	}

	if ((ExecuteFunctions & 32) > 0)
	{
		ScriptEnd(InOutRTData, InOutRTSData, InOutRTTData, InTask, InReason);
	}
}

void UBXTProcessor::ChangeTaskTickRate(UPARAM(ref) FBXTLTaskRTData& InOutRTTData, UBXTask* InTask, float InRate)
{
	if ((ExecuteFunctions & 64) > 0)
	{
		ChangeTickRate(InOutRTTData, InTask, InRate);
	}

	if ((ExecuteFunctions & 128) > 0)
	{
		ScriptChangeTickRate(InOutRTTData, InTask, InRate);
	}
}

#pragma endregion Important



#pragma region GlobalAPI
bool UBXTProcessor::IsTaskCompleted(UBXTask* InTask, const FBXTLTaskRTData& InTaskData, EBXTLFinishReason& OutReason)
{
	OutReason = EBXTLFinishReason::FR_Interrupt;
	if (!InTask)
	{
		return true;
	}

	if (InTaskData.bEarlyFinish)
	{
		return true;
	}

	OutReason = EBXTLFinishReason::FR_EndOfLife;
	switch (InTask->LifeType)
	{
	case EBXTLifeType::L_Instant:
		return true;
	case EBXTLifeType::L_Duration:
		return InTaskData.RunTime >= InTask->Duration;
	case EBXTLifeType::L_Timeline:
		return false;
	case EBXTLifeType::L_DurationTimeline:
		return InTaskData.RunTime >= InTask->Duration;
	default:
		return true;
	}
}

bool UBXTProcessor::AddPendingTask(UPARAM(ref) FBXTLRunTimeData& InOutRTData, UPARAM(ref) FBXTLSectionRTData& InOutRTSData, UPARAM(ref) FBXTLTaskRTData& InOutRTTData, const FName& InEventName)
{
	const UBXSettings* Settings = GetDefault<UBXSettings>();
	if (!Settings)
	{
		return false;
	}

	UBXTLAsset* Asset = InOutRTData.StaticData;
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

	FBXTEvent* EventPointer = Task->Events.Find(InEventName);
	if (!EventPointer)
	{
		return false;
	}

	// 需要广播，则添加到广播列表
	if (EventPointer->bMulticast)
	{
		InOutRTSData.BroadcastTasks.Add(FBXTLBroadcastTaskInfo(InOutRTSData.Index * 1000 + InOutRTTData.Index, InEventName));
	}

	TArray<int32>& List = InOutRTSData.FramePendingTasks;
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
			InOutRTSData.PendingTasks.Add(FBXTLPendingTaskInfo(LocalIndex, InOutRTSData.RunTime + It->Value));

			continue;
		}

		// 如果开启了帧内待处理列表，则添加到该列表
		if (List.Num() > 0)
		{
			if (!List.Contains(LocalIndex) && Settings->FramePendingTaskLimit > List.Num())
			{
				List.Add(LocalIndex);
			}
		}
		else
		{
			InOutRTSData.PendingTasks.Add(FBXTLPendingTaskInfo(LocalIndex, InOutRTSData.RunTime));
		}
	}

	return true;
}

void UBXTProcessor::GetTargetComponentList(const FBXTLRunTimeData& InRTData, UBXTask* InTask, TArray<USceneComponent*>& OutComponents)
{
	OutComponents.Reset();
	if (!IsValid(InTask) || !IsValid(InRTData.StaticData))
	{
		return;
	}
	
	for (int32 i = 0; i < (int32)EBXTTargetType::T_TMax; ++i)
	{
		if ((InTask->TargetTypes & (1 << i)) <= 0)
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
			for (TArray<FBXTInputInfo>::TIterator It(InTask->CollisionInputDatas); It; ++It)
			{
				FBXTHitResults* HitResults = UBXTProcessor::ReadContextData<FBXTHitResults>(InRTData, UBXFunctionLibrary::GetSoftTaskFullIndex(InRTData.StaticData, It->DataTask), It->DataDesc);
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

void UBXTProcessor::GetTargetActorList(const FBXTLRunTimeData& InRTData, UBXTask* InTask, TArray<AActor*>& OutActors)
{
	TArray<USceneComponent*> Components;
	UBXTProcessor::GetTargetComponentList(InRTData, InTask, Components);

	OutActors.Reset();
	for (TArray<USceneComponent*>::TIterator It(Components); It; ++It)
	{
		OutActors.AddUnique((*It)->GetOwner());
	}
}

bool UBXTProcessor::AnalyzeTransformCreater(AActor* InTarget, const FBXTLRunTimeData& InRTData, const FBXTTransformCreater& InCreater, FBXTTransformCreaterResult& OutResult)
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
		OriginComponent = UBXTProcessor::AnalyzeTransformCreaterCoordinateType(InCreater, true, InRTData, OriginTransform);
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
			XAxisComponent = UBXTProcessor::AnalyzeTransformCreaterCoordinateType(InCreater, false, InRTData, XAxisTransform);
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

bool UBXTProcessor::AnalyzeTransformCreaterList(AActor* InTarget, const FBXTLRunTimeData& InRTData, const TArray<FBXTTransformCreater>& InCreaterList, FBXTTransformCreaterResult& OutResult)
{
	for (TArray<FBXTTransformCreater>::TConstIterator It(InCreaterList); It; ++It)
	{
		if (UBXTProcessor::AnalyzeTransformCreater(InTarget, InRTData, *It, OutResult))
		{
			return true;
		}
	}

	return false;
}

USceneComponent* UBXTProcessor::AnalyzeTransformCreaterCoordinateType(const FBXTTransformCreater& InCreater, bool bUseOrigin, const FBXTLRunTimeData& InRTData, FTransform& OutTransform)
{
	EBXTCoordinateType CoordinateType = bUseOrigin ? InCreater.OriginType : InCreater.XAxisType;
	int32 FullIndex = bUseOrigin ? UBXFunctionLibrary::GetSoftTaskFullIndex(InRTData.StaticData, InCreater.OriginInputTask) : UBXFunctionLibrary::GetSoftTaskFullIndex(InRTData.StaticData, InCreater.XAxisInputTask);
	FName DataName = bUseOrigin ? InCreater.OriginInputDesc : InCreater.XAxisInputDesc;
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
		if (FBXTHitResults* HitResults = UBXTProcessor::ReadContextData<FBXTHitResults>(InRTData, FullIndex, DataName))
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
		if (FTransform* TransformResults = UBXTProcessor::ReadContextData<FTransform>(InRTData, FullIndex, DataName))
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

#pragma endregion GlobalAPI
