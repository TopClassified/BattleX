#include "BXTLEditorUtilities.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Blueprint.h"
#include "UObject/UnrealType.h"

#include "BXTLEditorSettings.h"
#include "BXTLAsset.h"
#include "BXTask.h"



#define LOCTEXT_NAMESPACE "BXTLEditorUtilities"

TArray<UClass*> FBXTLEditorUtilities::CollectBPTaskClasses()
{
	TArray<UClass*> TaskClasses;

	auto CheckClass = [&TaskClasses](const FAssetData& AssetMsg)
	{
		// 仅处理BP_BXT_前缀的蓝图
		FString AssetName = AssetMsg.AssetName.ToString();
		if (!AssetName.StartsWith(TEXT("BP_BXT_")))
		{
			return;
		}

		// 优先从GeneratedClass标签直接拿类路径,避免依赖UBlueprint中间对象
		FString GeneratedClassPath;
		AssetMsg.GetTagValue(FName("GeneratedClassPath"), GeneratedClassPath);

		UClass* CurClass = nullptr;
		if (!GeneratedClassPath.IsEmpty())
		{
			CurClass = LoadClass<UBXTask>(nullptr, *GeneratedClassPath);
		}
		else
		{
			// 回退到加载Blueprint对象取GeneratedClass
			UBlueprint* CurBP = LoadObject<UBlueprint>(nullptr, *AssetMsg.GetSoftObjectPath().ToString());
			if (CurBP)
			{
				CurClass = CurBP->GeneratedClass;
			}
		}

		if (!CurClass || CurClass->HasAnyClassFlags(CLASS_Abstract))
		{
			return;
		}

		if (CurClass->IsChildOf(UBXTask::StaticClass()))
		{
			TaskClasses.AddUnique(CurClass);
		}
	};

	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.bRecursiveClasses = true;
	Filter.ClassPaths.AddUnique(UBlueprint::StaticClass()->GetClassPathName());

	TArray<FAssetData> BlueprintList;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	AssetRegistryModule.Get().GetAssets(Filter, BlueprintList);
	for (const FAssetData& AssetMsg : BlueprintList)
	{
		CheckClass(AssetMsg);
	}

	return TaskClasses;
}

void FBXTLEditorUtilities::MakeNewTaskPicker(FMenuBuilder& MenuBuilder, const FOnClassPicked& OnTaskClassPicked)
{
	TArray<UClass*> TaskClasses = CollectBPTaskClasses();
	if (TaskClasses.Num() == 0)
	{
		return;
	}

	if (MenuBuilder.GetMultiBox()->GetBlocks().Num() > 1)
	{
		MenuBuilder.AddMenuSeparator();
	}

	// 按BlueprintNamespace分组
	TMap<FString, TArray<UClass*>> NamespaceToClasses;
	for (UClass* TaskClass : TaskClasses)
	{
		UBXTask* TaskCDO = Cast<UBXTask>(TaskClass->GetDefaultObject(true));
		FString Namespace = TaskCDO ? TaskCDO->GetBlueprintNamespace().ToString() : FString();
		if (Namespace.IsEmpty())
		{
			Namespace = TEXT("Tasks");
		}

		NamespaceToClasses.FindOrAdd(Namespace).Add(TaskClass);
	}

	TArray<FString> Namespaces;
	NamespaceToClasses.GetKeys(Namespaces);
	Namespaces.Sort();

	for (const FString& Namespace : Namespaces)
	{
		if (TArray<UClass*>* ClassesPtr = NamespaceToClasses.Find(Namespace))
		{
			TArray<UClass*> Classes = *ClassesPtr;

			// 每个命名空间作为可折叠展开的子菜单
			MenuBuilder.AddSubMenu
			(
				FText::FromString(Namespace),
				FText::FromString(Namespace),
				FNewMenuDelegate::CreateLambda([Classes, OnTaskClassPicked](FMenuBuilder& SubMenuBuilder)
				{
					for (UClass* TaskClass : Classes)
					{
						UBXTask* TaskCDO = Cast<UBXTask>(TaskClass->GetDefaultObject(true));
						FText DisplayName = TaskCDO ? TaskCDO->GetBlueprintDisplayName() : FText();
						if (DisplayName.IsEmpty())
						{
							DisplayName = TaskClass->GetDisplayNameText();
						}

						FText ToolTip = FText::FromString(TaskClass->GetDescription());

						SubMenuBuilder.AddMenuEntry
						(
							DisplayName,
							ToolTip,
							FSlateIcon(),
							FUIAction(FExecuteAction::CreateLambda([OnTaskClassPicked, TaskClass]() { OnTaskClassPicked.ExecuteIfBound(TaskClass); }))
						);
					}
				})
			);
		}
	}
}

void FBXTLEditorUtilities::MakeNewTaskTemplatePicker(class FMenuBuilder& MenuBuilder, const FBXTLPickTaskGroup& OnTemplatePicked)
{
	TArray< TSharedPtr<FName>> TemplateNameList;
	if (const UBXTLEditorSettings* Setting = GetDefault<UBXTLEditorSettings>())
	{
		if (UBXTLTaskGroupTemplate* Template = Setting->Template.LoadSynchronous())
		{
			for (FBXTLTaskGroupInformation Group : Template->Groups)
			{
				TemplateNameList.Add(MakeShared<FName>(Group.Name));
			}
		}
	}

	MenuBuilder.AddWidget
	(
		SNew(SBox)
		.MinDesiredWidth(200.0f)
		.MaxDesiredHeight(400.0f)
		[
			SNew(SBXTLTaskGroupPicker)
			.AllTemplateNames(TemplateNameList)
			.PickTaskGroupEvent(OnTemplatePicked)
		],
		FText(), true, false
	);
}

TSharedRef<SWidget> FBXTLEditorUtilities::MakeTrackButton(FText HoverText, FOnGetContent MenuContent)
{
	FSlateFontInfo SmallLayoutFont = FCoreStyle::GetDefaultFontStyle("Regular", 8);

	TSharedRef<STextBlock> ComboButtonText = SNew(STextBlock)
		.Text(HoverText)
		.Font(SmallLayoutFont)
		.ColorAndOpacity(FSlateColor::UseForeground());

	TSharedRef<SComboButton> ComboButton = SNew(SComboButton)
		.HasDownArrow(false)
		.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
		.ForegroundColor(FSlateColor::UseForeground())
		.OnGetMenuContent(MenuContent)
		.ContentPadding(FMargin(5, 2))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.ButtonContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(FMargin(0, 0, 2, 0))
			[
				SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(FAppStyle::GetBrush("ComboButton.Arrow"))
			]
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				ComboButtonText
			]
		];

	return ComboButton;
}

// 构建同类两Task间坐标系创建器输入UID映射(源UID→目标UID):目标侧UID已被CopyDataFromOther重置,此处惰性分配全新值,供变换输入条目重新配对
static void BuildCreaterInputUIDMap(UBXTask* SrcTask, UBXTask* DestTask, TMap<int64, int64>& OutUIDMap)
{
	for (TFieldIterator<FProperty> PropIt(SrcTask->GetClass()); PropIt; ++PropIt)
	{
		if (FStructProperty* StructProp = CastField<FStructProperty>(*PropIt))
		{
			if (StructProp->Struct != FBXTTransformCreater::StaticStruct())
			{
				continue;
			}

			FBXTTransformCreater* SrcCreater = StructProp->ContainerPtrToValuePtr<FBXTTransformCreater>(SrcTask);
			FBXTTransformCreater* DestCreater = StructProp->ContainerPtrToValuePtr<FBXTTransformCreater>(DestTask);
			OutUIDMap.Add(SrcCreater->GetOriginInputUniqueID(), DestCreater->GetOriginInputUniqueID());
			OutUIDMap.Add(SrcCreater->GetXAxisInputUniqueID(), DestCreater->GetXAxisInputUniqueID());
		}
		else if (FArrayProperty* ArrayProp = CastField<FArrayProperty>(*PropIt))
		{
			FStructProperty* ArrayInnerProp = CastField<FStructProperty>(ArrayProp->Inner);
			if (!ArrayInnerProp || ArrayInnerProp->Struct != FBXTTransformCreater::StaticStruct())
			{
				continue;
			}

			FScriptArrayHelper SrcArrayHelper(ArrayProp, SrcTask);
			FScriptArrayHelper DestArrayHelper(ArrayProp, DestTask);
			for (int32 j = 0; j < SrcArrayHelper.Num() && j < DestArrayHelper.Num(); ++j)
			{
				FBXTTransformCreater* SrcCreater = ArrayInnerProp->ContainerPtrToValuePtr<FBXTTransformCreater>(SrcArrayHelper.GetRawPtr(j));
				FBXTTransformCreater* DestCreater = ArrayInnerProp->ContainerPtrToValuePtr<FBXTTransformCreater>(DestArrayHelper.GetRawPtr(j));
				OutUIDMap.Add(SrcCreater->GetOriginInputUniqueID(), DestCreater->GetOriginInputUniqueID());
				OutUIDMap.Add(SrcCreater->GetXAxisInputUniqueID(), DestCreater->GetXAxisInputUniqueID());
			}
		}
	}
}

void FBXTLEditorUtilities::RestoreTasksRelation(TArray<UBXTask*>& DestTasks, const TArray<UBXTask*>& SrcTasks)
{
	// 两个数组的大小不一致，则直接返回
	if (DestTasks.Num() != SrcTasks.Num())
	{
		return;
	}

	// 对应元素的类型不一致，则直接返回
	for (int32 i = 0; i < SrcTasks.Num(); ++i)
	{
		if (!IsValid(SrcTasks[i]) || !IsValid(DestTasks[i]) || SrcTasks[i]->GetClass() != DestTasks[i]->GetClass())
		{
			return;
		}
	}

	// 根据模板列表，修复Task的依赖关系
	for (int32 i = 0; i < DestTasks.Num(); ++i)
	{
		UBXTask* SrcTask = SrcTasks[i];
		UBXTask* DestTask = DestTasks[i];

		// 变换输入UID映射(源→目标):拷贝构造不携带UID,须为变换输入条目配对目标Task创建器的新UID
		TMap<int64, int64> CreaterUIDMap;
		BuildCreaterInputUIDMap(SrcTask, DestTask, CreaterUIDMap);

		// 修复CollisionInputDatas
		DestTask->CollisionInputDatas.Empty();
		for (int32 j = 0; j < SrcTask->CollisionInputDatas.Num(); j++)
		{
			const FBXTInputInfo& Template = SrcTask->CollisionInputDatas[j];

			// 无脑拷贝所有数据
			FBXTInputInfo& NewInfo = DestTask->CollisionInputDatas.Add_GetRef(Template);

			// 调整选取的对象
			int32 FindIndex = SrcTasks.Find(Cast<UBXTask>(Template.DataTask.Get()));
			if (FindIndex >= 0)
			{
				NewInfo.DataTag = Template.DataTag;
				NewInfo.DataTask = DestTasks[FindIndex];
			}
			else
			{
				NewInfo.DataTag = FGameplayTag::EmptyTag;
				NewInfo.DataTask = nullptr;
			}
		}

		// 修复InputDatas
		DestTask->InputDatas.Empty();
		for (int32 j = 0; j < SrcTask->InputDatas.Num(); j++)
		{
			FBXTInputInfo& Template = SrcTask->InputDatas[j];

			// 无脑拷贝所有数据
			FBXTInputInfo& NewInfo = DestTask->InputDatas.Add_GetRef(Template);

			// 变换输入条目重配对目标创建器UID:条目UID与目标创建器断裂时,PreSave的RefreshTransformCreaters会删除重建该条目,丢失已接线的DataTask/DataTag
			if (int64* MappedUID = CreaterUIDMap.Find(Template.GetUniqueID()))
			{
				NewInfo.SetUniqueID(*MappedUID);
			}

			int32 FindIndex = SrcTasks.Find(Cast<UBXTask>(Template.DataTask.Get()));
			if (FindIndex >= 0)
			{
				NewInfo.DataTag = Template.DataTag;
				NewInfo.DataTask = DestTasks[FindIndex];
			}
			else
			{
				NewInfo.DataTag = FGameplayTag::EmptyTag;
				NewInfo.DataTask = nullptr;
			}
		}

		// 修复OutputDatas
		DestTask->OutputDatas.Empty();
		for (int32 j = 0; j < SrcTask->OutputDatas.Num(); ++j)
		{
			DestTask->OutputDatas.Add(SrcTask->OutputDatas[j]);
		}

		// 修复EventTaskMap
		DestTask->Events.Empty();
		for (TMap<FGameplayTag, FBXTEvent>::TIterator It(SrcTask->Events); It; ++It)
		{
			FBXTEvent NewEvent;
			NewEvent.bMulticast = It->Value.bMulticast;
			
			for (TMap<TSoftObjectPtr<UBXTask>, float>::TIterator It2(It->Value.Event); It2; ++It2)
			{
				int32 FindIndex = SrcTasks.Find(Cast<UBXTask>(It2->Key.Get()));
				if (FindIndex >= 0)
				{
					NewEvent.Event.Add(DestTasks[FindIndex], It2->Value);
				}
			}

			DestTask->Events.Add(It->Key, NewEvent);
		}
	}
}

#undef LOCTEXT_NAMESPACE
