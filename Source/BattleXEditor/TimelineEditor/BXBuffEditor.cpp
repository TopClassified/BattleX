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



#pragma region Important
FBXBuffEditor::FBXBuffEditor()
{
	// 注册BUFF节点工厂
	BXBuffGraphNodeFactory = MakeShareable(new FBXBuffGraphNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(BXBuffGraphNodeFactory);
}

FBXBuffEditor::~FBXBuffEditor()
{

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
