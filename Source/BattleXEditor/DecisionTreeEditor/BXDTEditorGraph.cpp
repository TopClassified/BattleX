#include "BXDTEditorGraph.h"



UBXDTEditorGraph::UBXDTEditorGraph()
{

}

UBXDTEditorGraph::~UBXDTEditorGraph()
{

}

bool UBXDTEditorGraph::Modify(bool bAlwaysMarkDirty)
{
	bool Rtn = Super::Modify(bAlwaysMarkDirty);

	GetOuter()->Modify();

	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		Nodes[i]->Modify();
	}

	return Rtn;
}

void UBXDTEditorGraph::Clear()
{
	Nodes.Empty();
}

void UBXDTEditorGraph::PostEditUndo()
{
	Super::PostEditUndo();

	NotifyGraphChanged();
}
