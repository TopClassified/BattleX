#include "BXBuffEditor.h"

#include "EdGraphUtilities.h"

#include "BXBuffEditorMode.h"
#include "Graph/BXBuffGraph.h"
#include "Graph/BXBuffGraphSchema.h"
#include "Graph/BXBuffGraphNode.h"
#include "Graph/SBXBuffGraphNode.h"



// BUFF节点工厂(为UBXBuffGraphNode创建SBXBuffGraphNode)
class FBXBuffGraphNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UBXBuffGraphNode* GraphNode = Cast<UBXBuffGraphNode>(Node))
		{
			TSharedRef<SBXBuffGraphNode> SNode = SNew(SBXBuffGraphNode, GraphNode);
			GraphNode->NodeWidget = SNode;

			return SNode;
		}

		return nullptr;
	}
};

static TSharedPtr<FGraphPanelNodeFactory> BXBuffGraphNodeFactory = nullptr;

// BUFF编辑器实例计数(节点工厂注册/注销管理)
static int32 BXBuffEditorNum = 0;



#pragma region Important
FBXBuffEditor::FBXBuffEditor()
{
	BXBuffEditorNum += 1;

	// 注册BUFF节点工厂(实例计数管理:首个实例注册,末个实例关闭时注销,原实现每个实例重复注册且永不注销)
	if (BXBuffEditorNum == 1)
	{
		BXBuffGraphNodeFactory = MakeShareable(new FBXBuffGraphNodeFactory());
		FEdGraphUtilities::RegisterVisualNodeFactory(BXBuffGraphNodeFactory);
	}
}

FBXBuffEditor::~FBXBuffEditor()
{

}

void FBXBuffEditor::OnClose()
{
	FBXTLEditor::OnClose();

	BXBuffEditorNum -= 1;

	// 末个实例关闭时注销BUFF节点工厂(原实现从不注销,工厂随编辑器开关无限累积)
	if (BXBuffEditorNum <= 0 && BXBuffGraphNodeFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(BXBuffGraphNodeFactory.ToSharedRef());
		BXBuffGraphNodeFactory.Reset();
	}
}

TSharedPtr<FApplicationMode> FBXBuffEditor::CreateEditorMode()
{
	return MakeShareable(new FBXBuffEditorMode(BXTLEditorModes::Timeline, SharedThis(this)));
}

UClass* FBXBuffEditor::GetGraphClass() const
{
	return UBXBuffGraph::StaticClass();
}

UClass* FBXBuffEditor::GetGraphSchemaClass() const
{
	return UBXBuffGraphSchema::StaticClass();
}

#pragma endregion Important
