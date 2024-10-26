#include "BXTask.h"
#include "UObject/ObjectSaveContext.h"

#include "BXFunctionLibrary.h" 



#pragma region Important
UBXTask::UBXTask()
{
	
}
#pragma endregion Important



#pragma region Editor
#if WITH_EDITOR
void UBXTask::PreSave(FObjectPreSaveContext SaveContext)
{
	RefreshTransformCreaters();
	
	Super::PreSave(SaveContext);
}

void UBXTask::AlignTimeProperty(float InAlign)
{
	if ((TriggerTypes & 1) <= 0)
	{
		StartTime = 0.0f;
	}
	StartTime = UBXFunctionLibrary::AlignTime(StartTime, InAlign);

	if (LifeType == EBXTLifeType::L_Instant)
	{
		Duration = 0.1f;
	}
	Duration = UBXFunctionLibrary::AlignTime(Duration, InAlign);

	for (TMap<FGameplayTag, FBXTEvent>::TIterator It(Events); It; ++It)
	{
		for (TMap<TSoftObjectPtr<class UBXTask>, float>::TIterator It2(It->Value.Event); It2; ++It2)
		{
			It2->Value = UBXFunctionLibrary::AlignTime(It2->Value, InAlign);
		}
	}
}

FText UBXTask::GetDisplayName() const
{
	FString Name = this->GetName();
	Name = Name.Replace(TEXT("BP_BXT_"), TEXT(""));
	Name = Name.Replace(TEXT("_C"), TEXT(""));

	if (!DisplayName.IsEmpty())
	{
		Name = Name + TEXT("  ") + DisplayName.ToString();
	}

	return FText::FromString(Name);
}

FText UBXTask::GetAnnotation() const
{
	return Annotation;
}

void UBXTask::CopyDataFromOther(UBXTask* Other)
{
	UBXFunctionLibrary::CopyObject(this, Other);

	Events.Reset();

	InputDatas.Reset();
	OutputDatas.Reset();
	CollisionInputDatas.Reset();
}

bool UBXTask::RefreshProperty()
{
	return false;
}

void UBXTask::RefreshDataByPreviewObject_Implementation(UObject* InObject, const FBXTLPreviewObjectData& InData)
{

}

void UBXTask::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName().IsEqual(TEXT("TargetTypes")))
	{
		if ((TargetTypes & 16) > 0)
		{
			CollisionInputDatas.AddDefaulted();
		}
		else
		{
			CollisionInputDatas.Empty();
		}

		for (int32 i = 0; i < CollisionInputDatas.Num(); ++i)
		{
			CollisionInputDatas[i].DisplayName = FName(TEXT("InCol") + FString::FromInt(i));
			CollisionInputDatas[i].StructType = FBXTHitResults::StaticStruct();
		}
		
		RefreshInputOutput.Broadcast();
	}
	if (PropertyChangedEvent.GetPropertyName().IsEqual(TEXT("CollisionInputDatas")))
	{
		for (int32 i = 0; i < CollisionInputDatas.Num(); ++i)
		{
			CollisionInputDatas[i].DisplayName = FName(TEXT("InCol") + FString::FromInt(i));
			CollisionInputDatas[i].StructType = FBXTHitResults::StaticStruct();
		}
		
		RefreshInputOutput.Broadcast();
	}
	
	if (PropertyChangedEvent.GetPropertyName().IsEqual(TEXT("InputDatas")))
	{
		RefreshInputOutput.Broadcast();
	}
	
	if (PropertyChangedEvent.GetPropertyName().IsEqual(TEXT("OutputDatas")))
	{
		RefreshInputOutput.Broadcast();
	}

	if (PropertyChangedEvent.GetPropertyName().IsEqual(TEXT("OriginType")))
	{
		RefreshTransformCreaters();
	}

	if (PropertyChangedEvent.GetPropertyName().IsEqual(TEXT("XAxisType")))
	{
		RefreshTransformCreaters();
	}

	bNeedCollisionInput = (TargetTypes & (1 << (int32)EBXTTargetType::T_CollisionResults)) > 0;
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UBXTask::RefreshTransformCreaters()
{
	bool bBroadCast = false;
	FString PrefixS = TEXT("TCS_"), PrefixW = TEXT("TCW_"), SuffixO = TEXT("_O"), SuffixX = TEXT("_X");
	TArray<FBXTTransformCreater*> Creaters;
	TMap<FBXTTransformCreater*, FString> CreaterNames;
	
	for (TFieldIterator<FProperty> PropIt(GetClass()); PropIt; ++PropIt)
	{
		if (FStructProperty* StructProp = CastField<FStructProperty>(*PropIt))
		{
			if (StructProp->Struct != FBXTTransformCreater::StaticStruct())
			{
				continue;
			}
			
			FBXTTransformCreater* CreaterPtr = StructProp->ContainerPtrToValuePtr<FBXTTransformCreater>(this);
			if (CreaterPtr->OriginType == EBXTCoordinateType::C_Special || CreaterPtr->XAxisType == EBXTCoordinateType::C_Special)
			{
				Creaters.Add(CreaterPtr);
				CreaterNames.Add(CreaterPtr, PrefixS + StructProp->GetName());
			}
			if (CreaterPtr->OriginType == EBXTCoordinateType::C_World || CreaterPtr->XAxisType == EBXTCoordinateType::C_World)
			{
				Creaters.Add(CreaterPtr);
				CreaterNames.Add(CreaterPtr, PrefixW + StructProp->GetName());
			}
		}
		else if (FArrayProperty* ArrayProp = CastField<FArrayProperty>(*PropIt))
		{
			FStructProperty* ArrayInnerProp = CastField<FStructProperty>(ArrayProp->Inner);
			if (!ArrayInnerProp || ArrayInnerProp->Struct != FBXTTransformCreater::StaticStruct())
			{
				continue;
			}

			FScriptArrayHelper ArrayHelper(ArrayProp, this);
			for (int32 i = 0; i < ArrayHelper.Num(); ++i)
			{
				FBXTTransformCreater* CreaterPtr = ArrayInnerProp->ContainerPtrToValuePtr<FBXTTransformCreater>(ArrayHelper.GetRawPtr(i));
				if (CreaterPtr->OriginType == EBXTCoordinateType::C_Special || CreaterPtr->XAxisType == EBXTCoordinateType::C_Special)
				{
					Creaters.Add(CreaterPtr);
					CreaterNames.Add(CreaterPtr, PrefixS + StructProp->GetName() + FString::FromInt(i));
				}

				if (CreaterPtr->OriginType == EBXTCoordinateType::C_World || CreaterPtr->XAxisType == EBXTCoordinateType::C_World)
				{
					Creaters.Add(CreaterPtr);
					CreaterNames.Add(CreaterPtr, PrefixW + StructProp->GetName() + FString::FromInt(i));
				}
			}
		}		
	}

	for (TArray<FBXTInputInfo>::TIterator It(InputDatas); It; ++It)
	{
		FString InfoDisplayName = It->DisplayName.ToString();
		if (!InfoDisplayName.Contains(PrefixS) && !InfoDisplayName.Contains(PrefixW))
		{
			continue;
		}	
		
		bool Flag = false;
		for (int32 i = 0; i < Creaters.Num(); ++i)
		{
			FBXTTransformCreater* Creater = Creaters[i];
			if (!Creater)
			{
				continue;
			}

			if (InfoDisplayName.Contains(SuffixO) && It->GetUniqueID() == Creater->OriginInputUniqueID)
			{
				if (Creater->OriginType == EBXTCoordinateType::C_Special && InfoDisplayName.Contains(PrefixS))
				{
					Flag = true;
					break;
				}
				else if (Creater->OriginType == EBXTCoordinateType::C_World && InfoDisplayName.Contains(PrefixW))
				{
					
					Flag = true;
					break;	
				}
			}
			
			if (It->DisplayName.ToString().Contains(SuffixX) && It->GetUniqueID() == Creater->XAxisInputUniqueID)
			{
				if (Creater->XAxisType == EBXTCoordinateType::C_Special && InfoDisplayName.Contains(PrefixS))
				{
					Flag = true;
					break;
				}
				else if (Creater->XAxisType == EBXTCoordinateType::C_World && InfoDisplayName.Contains(PrefixW))
				{
					
					Flag = true;
					break;	
				}
			}
		}

		if (!Flag)
		{
			bBroadCast = true;
			It.RemoveCurrent();
		}
	}

	for (int32 i = 0; i < Creaters.Num(); ++i)
	{
		FBXTTransformCreater* Creater = Creaters[i];
		if (!Creater)
		{
			continue;
		}

		FString CreaterName;
		if (FString* FindResult = CreaterNames.Find(Creater))
		{
			CreaterName = *FindResult;
		}
		else
		{
			continue;
		}
		
		if (Creater->OriginType == EBXTCoordinateType::C_Special || Creater->OriginType == EBXTCoordinateType::C_World)
		{
			bool Flag = false;
			for (TArray<FBXTInputInfo>::TIterator It(InputDatas); It; ++It)
			{
				if (It->GetUniqueID() == Creater->OriginInputUniqueID)
				{
					It->DisplayName = FName(CreaterName + SuffixO);
					Creater->OriginInputTask = It->DataTask;
					Creater->OriginInputTag = It->DataTag;
					Flag = true;
					break;
				}
			}

			if (!Flag)
			{
				bBroadCast = true;
				FBXTInputInfo& NewInfo = InputDatas.AddDefaulted_GetRef();
				NewInfo.SetUniqueID(Creater->OriginInputUniqueID);
				NewInfo.StructType = (Creater->OriginType == EBXTCoordinateType::C_Special) ? FBXTHitResults::StaticStruct() : TBaseStructure<FTransform>::Get();
				NewInfo.DisplayName = FName(CreaterName + SuffixO);
				NewInfo.DataTask = nullptr;
				NewInfo.DataTag = FGameplayTag::EmptyTag;
			}
		}

		if (Creater->XAxisType == EBXTCoordinateType::C_Special || Creater->XAxisType == EBXTCoordinateType::C_World)
		{
			bool Flag = false;
			for (TArray<FBXTInputInfo>::TIterator It(InputDatas); It; ++It)
			{
				if (It->GetUniqueID() == Creater->XAxisInputUniqueID)
				{
					It->DisplayName = FName(CreaterName + SuffixX);
					Creater->XAxisInputTask = It->DataTask;
					Creater->XAxisInputTag = It->DataTag;
					Flag = true;
					break;
				}
			}

			if (!Flag)
			{
				bBroadCast = true;
				FBXTInputInfo& NewInfo = InputDatas.AddDefaulted_GetRef();
				NewInfo.SetUniqueID(Creater->XAxisInputUniqueID);
				NewInfo.StructType = (Creater->XAxisType == EBXTCoordinateType::C_Special) ? FBXTHitResults::StaticStruct() : TBaseStructure<FTransform>::Get();
				NewInfo.DisplayName = FName(CreaterName + SuffixX);
				NewInfo.DataTask = nullptr;
				NewInfo.DataTag = FGameplayTag::EmptyTag;
			}
		}
	}

	if (bBroadCast)
	{
		RefreshInputOutput.Broadcast();
	}
}
#endif
#pragma endregion Editor
