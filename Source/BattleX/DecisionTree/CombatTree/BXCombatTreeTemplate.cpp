#include "BXCombatTreeTemplate.h"

#include "BXCombatTreeNode.h"
#include "BXCombatTreeEdge.h"



UBXCombatTreeType::UBXCombatTreeType()
{
	bAllowCycle = true;
	FirstNodeType = UBXCombatTreeRootNode::StaticClass();
	LoadAssetTypes.Add(UBXSkillAsset::StaticClass());

#if WITH_EDITORONLY_DATA
	NodeTypes.Add(UBXCombatTreeNode::StaticClass());
	EdgeType = UBXCombatTreeEdge::StaticClass();
#endif
}
