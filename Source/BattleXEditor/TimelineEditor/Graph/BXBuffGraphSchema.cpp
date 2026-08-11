#include "BXBuffGraphSchema.h"

#include "ScopedTransaction.h"
#include "UObject/UObjectIterator.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Blueprint.h"

#include "BXBuffGraph.h"
#include "BXBuffGraphNode.h"
#include "BXBuffAsset.h"
#include "BXTask.h"



#define LOCTEXT_NAMESPACE "BXBuffGraphSchema"

UEdGraphNode* FBXBuffCreateTaskAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (!TaskClass.IsValid())
	{
		return nullptr;
	}

	UBXBuffGraph* BuffGraph = Cast<UBXBuffGraph>(ParentGraph);
	if (!BuffGraph)
	{
		return nullptr;
	}

	UBXBuffAsset* BuffAsset = Cast<UBXBuffAsset>(BuffGraph->GetOuter());
	if (!BuffAsset)
	{
		return nullptr;
	}

	FScopedTransaction Transaction(LOCTEXT("CreateTask", "Create Buff Task"));

	BuffGraph->Modify();
	BuffAsset->Modify();

	// 创建Task并添加到Section[0].Group[0]
	UBXTask* NewTask = NewObject<UBXTask>(BuffAsset, TaskClass.Get(), NAME_None, RF_Transactional);
	if (NewTask)
	{
		BuffAsset->AddTask(0, 0, NewTask);

		// 创建对应的图表节点
		BuffGraph->GenerateGraphNodeByTask(NewTask, Location.X, Location.Y);

		return BuffGraph->GetGraphNodeByTask(NewTask);
	}

	return nullptr;
}



void UBXBuffGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	Super::GetGraphContextActions(ContextMenuBuilder);

	// 收集所有BP派生的非抽象Task子类(与技能编辑器保持一致,只显示BP_BXT_前缀的蓝图,避免列出未注册到TaskProcessorMap/TaskCustomDataMap的native C++类)
	TArray<UClass*> TaskClasses;

	auto CheckClass = [&](FAssetData& AssetMsg)
	{
		FString ClassPath = AssetMsg.GetSoftObjectPath().ToString();
		if (!ClassPath.Contains(TEXT("BP_BXT_")))
		{
			return;
		}

		UBlueprint* CurBP = LoadObject<UBlueprint>(nullptr, *ClassPath);
		if (!CurBP)
		{
			return;
		}

		UClass* CurClass = CurBP->GeneratedClass;
		if (!CurClass || CurClass->HasAnyClassFlags(CLASS_Abstract))
		{
			return;
		}

		if (UBXTask* Task = Cast<UBXTask>(CurClass->GetDefaultObject()))
		{
			TaskClasses.AddUnique(Task->GetClass());
		}
	};

	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.bRecursiveClasses = true;
	Filter.ClassPaths.AddUnique(UBlueprint::StaticClass()->GetClassPathName());
	Filter.ClassPaths.AddUnique(UBlueprintGeneratedClass::StaticClass()->GetClassPathName());

	TArray<FAssetData> BlueprintList;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	AssetRegistryModule.Get().GetAssets(Filter, BlueprintList);
	for (int32 i = 0; i < BlueprintList.Num(); ++i)
	{
		CheckClass(BlueprintList[i]);
	}

	FText CategoryName = LOCTEXT("TaskCategory", "Tasks");

	for (int32 i = 0; i < TaskClasses.Num(); ++i)
	{
		UClass* TaskClass = TaskClasses[i];
		if (!TaskClass)
		{
			continue;
		}

		FText TaskName = TaskClass->GetDisplayNameText();
		FText TaskToolTip = FText::FromString(TaskClass->GetDescription());

		TSharedPtr<FBXBuffCreateTaskAction> NewAction = MakeShareable(new FBXBuffCreateTaskAction(CategoryName, TaskName, TaskToolTip, 0, TaskClass));
		ContextMenuBuilder.AddAction(NewAction);
	}
}

#undef LOCTEXT_NAMESPACE
