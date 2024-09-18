#include "BXTLGraphNode.h"
#include "UObject/ObjectSaveContext.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "Kismet2/BlueprintEditorUtils.h"

#include "BXTask.h"
#include "BXTLAsset.h"

#include "SBXTLGraphNode.h"



#define LOCTEXT_NAMESPACE "BXTLGraphNode"

#pragma region Important
UBXTLGraphNode::UBXTLGraphNode()
{
	bCanRenameNode = false;
}

UBXTLGraphNode::~UBXTLGraphNode()
{

}

#pragma endregion Important



#pragma region Pin
void UBXTLGraphNode::AllocateDefaultPins()
{
	if (CachedTask)
	{
		RefreshPinInformationList(PinInformations);

		for (int32 i = 0; i < PinInformations.Num(); ++i)
		{
			CreatePinByInformation(PinInformations[i]);
		}
	}
}

void UBXTLGraphNode::UpdatePins()
{
	if (CachedTask)
	{
		TArray<FBXTLGNodePin> NewPinInformationList;
		RefreshPinInformationList(NewPinInformationList);

		// 第一次循环，将目前没有，之前已有的删除
		for (int32 i = 0; i < PinInformations.Num(); ++i)
		{
			int32 Index = NewPinInformationList.Find(PinInformations[i]);
			if (Index >= 0)
			{
				NewPinInformationList[Index].PinGUIDName = PinInformations[i].PinGUIDName;
			}
			else
			{
				FGuid CurGuid;
				FGuid::Parse(PinInformations[i].PinGUIDName.ToString(), CurGuid);

				UEdGraphPin* CurPin = GetPinByGUID(CurGuid);
				if (CurPin)
				{
					RemovePin(CurPin);
				}
			}
		}

		// 第二次循环，将目前已有，之前没有的添加
		for (int32 i = 0; i < NewPinInformationList.Num(); ++i)
		{
			int32 Index = PinInformations.Find(NewPinInformationList[i]);
			if (Index >= 0)
			{
				NewPinInformationList[i].PinGUIDName = PinInformations[Index].PinGUIDName;
			}
			else
			{
				CreatePinByInformation(NewPinInformationList[i]);
			}
		}

		PinInformations.Empty();
		PinInformations.Append(NewPinInformationList);
	}
}

FLinearColor UBXTLGraphNode::GetPinColor(const UEdGraphPin* InPin)
{
	if (FBXTLGNodePin* PinInfo = GetPinInformation(InPin))
	{
		if (PinInfo->PinType == 0 || PinInfo->PinType == 1)
		{
			return FLinearColor::White;
		}
		else
		{
			return FLinearColor::Yellow;
		}
	}

	return FLinearColor::Red;
}

FBXTLGNodePin* UBXTLGraphNode::GetPinInformation(const UEdGraphPin* InPin)
{
	if (Pins.Contains(InPin))
	{
		FName GUIDName = FName(InPin->PinId.ToString());

		for (int32 i = 0; i < PinInformations.Num(); ++i)
		{
			if (PinInformations[i].PinGUIDName.IsEqual(GUIDName))
			{
				return &PinInformations[i];
			}
		}
	}

	return nullptr;
}

void UBXTLGraphNode::GetPinDataType(const UEdGraphPin* InPin, EBXDataType& OutType, UScriptStruct*& OutStructType)
{
	if (!CachedTask)
	{
		return;
	}

	if (FBXTLGNodePin* PinInfo = GetPinInformation(InPin))
	{
		if (PinInfo->PinType == 2)
		{
			const TArray<FBXTInputInfo>& CollisionInfos = CachedTask->CollisionInputDatas;
			for (int32 i = 0; i < CollisionInfos.Num(); ++i)
			{
				const FBXTInputInfo& CurInfo = CollisionInfos[i];
				if (CurInfo.GetUniqueID() == PinInfo->UniqueID)
				{
					OutType = CurInfo.DataType;
					OutStructType = CurInfo.StructType;

					return;
				}
			}
		}
		else if (PinInfo->PinType == 3)
		{
			for (int32 i = 0; i < CachedTask->InputDatas.Num(); ++i)
			{
				const FBXTInputInfo& CurInfo = CachedTask->InputDatas[i];
				if (CurInfo.GetUniqueID() == PinInfo->UniqueID)
				{
					OutType = CurInfo.DataType;
					OutStructType = CurInfo.StructType;

					return;
				}
			}
		}
		else if (PinInfo->PinType == 4)
		{
			for (int32 i = 0; i < CachedTask->OutputDatas.Num(); ++i)
			{
				const FBXTOutputInfo& CurInfo = CachedTask->OutputDatas[i];
				if (CurInfo.GetUniqueID() == PinInfo->UniqueID)
				{
					OutType = CurInfo.DataType;
					OutStructType = CurInfo.StructType;

					return;
				}
			}
		}
	}
}

FName UBXTLGraphNode::GetPinNameFromTagName(const FName& InTag)
{
	FString TagString = InTag.ToString();
	int32 LastPointPos = 0;
	TagString.FindLastChar('.', LastPointPos);
	TagString = TagString.Right(TagString.Len() - LastPointPos - 1);

	return FName(TagString);
}

void UBXTLGraphNode::RefreshPinInformationList(TArray<FBXTLGNodePin>& InPinInformationList)
{
	InPinInformationList.Empty();

	FBXTLGNodePin PinInfo;
	PinInfo.PinType = 0;
	PinInfo.ExtraName = TEXT("Exe");
	InPinInformationList.Add(PinInfo);

	for (TMap<FName, FBXTEvent>::TIterator It(CachedTask->Events); It; ++It)
	{
		PinInfo.PinType = 1;
		PinInfo.ExtraName = It->Key;
		InPinInformationList.Add(PinInfo);
	}

	const TArray<FBXTInputInfo>& CollisionInfos = CachedTask->CollisionInputDatas;
	for (TArray<FBXTInputInfo>::TConstIterator It(CollisionInfos); It; ++It)
	{
		PinInfo.PinType = 2;
		PinInfo.UniqueID = It->GetUniqueID();
		PinInfo.ExtraName = It->DisplayName;
		InPinInformationList.Add(PinInfo);
	}

	for (TArray<FBXTInputInfo>::TIterator It(CachedTask->InputDatas); It; ++It)
	{
		PinInfo.PinType = 3;
		PinInfo.UniqueID = It->GetUniqueID();
		PinInfo.ExtraName = It->DisplayName;
		InPinInformationList.Add(PinInfo);
	}

	for (TArray<FBXTOutputInfo>::TIterator It(CachedTask->OutputDatas); It; ++It)
	{
		PinInfo.PinType = 4;
		PinInfo.UniqueID = It->GetUniqueID();
		PinInfo.ExtraName = It->DataDesc;
		InPinInformationList.Add(PinInfo);
	}
}

UEdGraphPin* UBXTLGraphNode::GetPinByGUID(FGuid InGuid)
{
	for (int32 i = 0; i < Pins.Num(); ++i)
	{
		if (Pins[i]->PinId == InGuid)
		{
			return Pins[i];
		}
	}

	return nullptr;
}

UEdGraphPin* UBXTLGraphNode::GetPinByName(FName InExtraName)
{
	for (int32 i = 0; i < Pins.Num(); ++i)
	{
		if (Pins[i]->PinName == InExtraName)
		{
			return Pins[i];
		}
	}

	return nullptr;
}

void UBXTLGraphNode::CreatePinByInformation(FBXTLGNodePin& InInformation)
{
	FName Pin1(TEXT("Graph")), Pin2(TEXT("Node"));

	EEdGraphPinDirection CurPinDir;
	if (InInformation.PinType == 0)
	{
		Pin1 = TEXT("real");
		Pin2 = TEXT("float");
		CurPinDir = EEdGraphPinDirection::EGPD_Input;
	}
	else if (InInformation.PinType == 1)
	{
		CurPinDir = EEdGraphPinDirection::EGPD_Output;
	}
	else if (InInformation.PinType == 4)
	{
		CurPinDir = EEdGraphPinDirection::EGPD_Output;
	}
	else
	{
		CurPinDir = EEdGraphPinDirection::EGPD_Input;
	}

	FName PinName = InInformation.ExtraName;
	if (InInformation.PinType == 4)
	{
		PinName = GetPinNameFromTagName(PinName);
	}

	if (UEdGraphPin* NewPin = CreatePin(CurPinDir, Pin1, Pin2, PinName))
	{
		if (InInformation.PinType == 0)
		{
			NewPin->GetSchema()->TrySetDefaultValue(*NewPin, *LexToString(0.0f));
		}

		InInformation.PinGUIDName = FName(NewPin->PinId.ToString());
	}
}

void UBXTLGraphNode::NodeConnectionListChanged()
{
	RefreshGraphNodeInformation();
}

#pragma endregion Pin



#pragma region Property
void UBXTLGraphNode::RefreshGraphNodeInformation()
{
	if (!CachedTask)
	{
		return;
	}

	UBXTLAsset* Asset = Cast<UBXTLAsset>(CachedTask->GetOuter());
	if (!Asset)
	{
		return;
	}

	for (int32 i = 0; i < Pins.Num(); ++i)
	{
		UEdGraphPin* CurPin = Pins[i];
		if (!CurPin)
		{
			continue;
		}

		FBXTLGNodePin* CurPinInfo = GetPinInformation(CurPin);
		if (!CurPinInfo)
		{
			continue;
		}

		// 更新Task事件触发列表
		if (CurPinInfo->PinType == 1)
		{
			if (FBXTEvent* Events = CachedTask->Events.Find(CurPinInfo->ExtraName))
			{
				Events->Event.Reset();

				for (int32 j = 0; j < CurPin->LinkedTo.Num(); ++j)
				{
					UEdGraphPin* TarPin = CurPin->LinkedTo[j];
					if (!TarPin)
					{
						continue;
					}

					if (UBXTLGraphNode* NextNode = Cast<UBXTLGraphNode>(TarPin->GetOwningNode()))
					{
						Events->Event.Add(NextNode->CachedTask, FCString::Atof(*TarPin->DefaultValue));
					}
				}
			}
		}
		// 更新碰撞信息列表
		else if (CurPinInfo->PinType == 2)
		{
			TArray<FBXTInputInfo>& CollisionInfos = CachedTask->CollisionInputDatas;
			for (int32 j = 0; j < CollisionInfos.Num(); ++j)
			{
				FBXTInputInfo& Info = CollisionInfos[j];
				if (Info.GetUniqueID() == CurPinInfo->UniqueID)
				{
					if (CurPin->LinkedTo.Num() > 0)
					{
						if (UBXTLGraphNode* TargetNode = Cast<UBXTLGraphNode>(CurPin->LinkedTo[0]->GetOwningNode()))
						{
							if (FBXTLGNodePin* TargetPin = TargetNode->GetPinInformation(CurPin->LinkedTo[0]))
							{
								Info.DataDesc = TargetPin->ExtraName;
								Info.DataTask = TargetNode->CachedTask;
							}
						}
					}
					else
					{
						Info.DataDesc = NAME_None;
						Info.DataTask = nullptr;
					}

					break;
				}
			}
		}
		// 更新输入信息列表
		else if (CurPinInfo->PinType == 3)
		{
			for (int32 j = 0; j < CachedTask->InputDatas.Num(); ++j)
			{
				FBXTInputInfo& Info = CachedTask->InputDatas[j];
				if (Info.GetUniqueID() == CurPinInfo->UniqueID)
				{
					if (CurPin->LinkedTo.Num() > 0)
					{
						if (UBXTLGraphNode* TargetNode = Cast<UBXTLGraphNode>(CurPin->LinkedTo[0]->GetOwningNode()))
						{
							if (FBXTLGNodePin* TargetPin = TargetNode->GetPinInformation(CurPin->LinkedTo[0]))
							{
								Info.DataDesc = TargetPin->ExtraName;
								Info.DataTask = TargetNode->CachedTask;
							}
						}
					}
					else
					{
						Info.DataDesc = NAME_None;
						Info.DataTask = nullptr;
					}

					break;
				}
			}
		}
	}
}

FText UBXTLGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (!CachedTask)
	{
		return Super::GetNodeTitle(TitleType);
	}
	else
	{
		return CachedTask->GetDisplayName();
	}
}

#pragma endregion Property



#pragma region Edit
void UBXTLGraphNode::PrepareForCopying()
{

}

void UBXTLGraphNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#pragma endregion Edit

#undef LOCTEXT_NAMESPACE
