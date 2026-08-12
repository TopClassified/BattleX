#include "BXBuffGraphSchema.h"

#include "ScopedTransaction.h"

#include "BXBuffGraph.h"
#include "BXBuffGraphNode.h"
#include "BXBuffAsset.h"
#include "BXTask.h"
#include "BXTLEditorUtilities.h"



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

	// 收集所有BP派生的非抽象Task子类(只显示BP_BXT_前缀的蓝图,避免列出未注册到TaskProcessorMap/TaskCustomDataMap的native C++类)
	TArray<UClass*> TaskClasses = FBXTLEditorUtilities::CollectBPTaskClasses();

	for (UClass* TaskClass : TaskClasses)
	{
		if (!TaskClass)
		{
			continue;
		}

		UBXTask* TaskCDO = Cast<UBXTask>(TaskClass->GetDefaultObject(true));
		if (!TaskCDO)
		{
			continue;
		}

		// 以BlueprintNamespace作为分类,未设置时回退到"Tasks"
		FText Namespace = TaskCDO->GetBlueprintNamespace();
		FText CategoryName = Namespace.IsEmpty() ? LOCTEXT("TaskCategory", "Tasks") : Namespace;

		FText TaskName = TaskCDO->GetBlueprintDisplayName();
		FText TaskToolTip = FText::FromString(TaskClass->GetDescription());

		TSharedPtr<FBXBuffCreateTaskAction> NewAction = MakeShareable(new FBXBuffCreateTaskAction(CategoryName, TaskName, TaskToolTip, 0, TaskClass));
		ContextMenuBuilder.AddAction(NewAction);
	}
}

#undef LOCTEXT_NAMESPACE
