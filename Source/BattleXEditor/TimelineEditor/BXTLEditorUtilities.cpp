#include "BXTLEditorUtilities.h"

#include "ClassViewerFilter.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "BXTLEditorSettings.h"
#include "BXTLAsset.h"
#include "BXTask.h"



#define LOCTEXT_NAMESPACE "BXTLEditorUtilities"

void FBXTLEditorUtilities::MakeNewTaskPicker(FMenuBuilder& MenuBuilder, const FOnClassPicked& OnTaskClassPicked)
{
	class FNotifyStateClassFilter : public IClassViewerFilter
	{
	public:
		FNotifyStateClassFilter() {}

		bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			const bool bChildOfObjectClass = InClass->IsChildOf(UBXTask::StaticClass());
			const bool bMatchesFlags = !InClass->HasAnyClassFlags(CLASS_Hidden | CLASS_HideDropDown | CLASS_Deprecated | CLASS_Abstract);

			return bChildOfObjectClass && bMatchesFlags;
		}

		virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			const bool bChildOfObjectClass = InUnloadedClassData->IsChildOf(UBXTask::StaticClass());
			const bool bMatchesFlags = !InUnloadedClassData->HasAnyClassFlags(CLASS_Hidden | CLASS_HideDropDown | CLASS_Deprecated | CLASS_Abstract);

			return bChildOfObjectClass && bMatchesFlags;
		}
	};

	if (MenuBuilder.GetMultiBox()->GetBlocks().Num() > 1)
	{
		MenuBuilder.AddMenuSeparator();
	}

	FClassViewerInitializationOptions InitOptions;
	InitOptions.Mode = EClassViewerMode::ClassPicker;
	InitOptions.bShowObjectRootClass = false;
	InitOptions.bShowUnloadedBlueprints = true;
	InitOptions.bShowNoneOption = false;
	InitOptions.bEnableClassDynamicLoading = true;
	InitOptions.bExpandRootNodes = true;
	InitOptions.NameTypeToDisplay = EClassViewerNameTypeToDisplay::DisplayName;
	InitOptions.ClassFilters.Add(MakeShared<FNotifyStateClassFilter>());
	InitOptions.bShowBackgroundBorder = false;

	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	MenuBuilder.AddWidget
	(
		SNew(SBox)
		.MinDesiredWidth(300.0f)
		.MaxDesiredHeight(400.0f)
		[
			ClassViewerModule.CreateClassViewer(InitOptions, OnTaskClassPicked)
		],
		FText(), true, false
	);
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
				NewInfo.DataDesc = Template.DataDesc;
				NewInfo.DataTask = DestTasks[FindIndex];
			}
			else
			{
				NewInfo.DataDesc = NAME_None;
				NewInfo.DataTask = nullptr;
			}
		}

		// 修复InputDatas
		DestTask->InputDatas.Empty();
		for (int32 j = 0; j < SrcTask->InputDatas.Num(); j++)
		{
			const FBXTInputInfo& Template = SrcTask->InputDatas[j];

			// 无脑拷贝所有数据
			FBXTInputInfo& NewInfo = DestTask->InputDatas.Add_GetRef(Template);

			int32 FindIndex = SrcTasks.Find(Cast<UBXTask>(Template.DataTask.Get()));
			if (FindIndex >= 0)
			{
				NewInfo.DataDesc = Template.DataDesc;
				NewInfo.DataTask = DestTasks[FindIndex];
			}
			else
			{
				NewInfo.DataDesc = NAME_None;
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
		for (TMap<FName, FBXTEvent>::TIterator It(SrcTask->Events); It; ++It)
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
