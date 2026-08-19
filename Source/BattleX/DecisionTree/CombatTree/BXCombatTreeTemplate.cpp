#include "BXCombatTreeTemplate.h"

#include "BXCombatTreeNode.h"
#include "BXCombatTreeEdge.h"



UBXCombatTreeType::UBXCombatTreeType()
{
	// 基类bAllowCycle/FirstNodeType/LoadAssetTypes均在WITH_EDITORONLY_DATA块内,cooked打包构建下访问会编译失败
#if WITH_EDITORONLY_DATA
	bAllowCycle = true;
	FirstNodeType = UBXCombatTreeRootNode::StaticClass();
	LoadAssetTypes.Add(UBXSkillAsset::StaticClass());

	NodeTypes.Add(UBXCombatTreeNode::StaticClass());
	EdgeType = UBXCombatTreeEdge::StaticClass();
#endif
}
