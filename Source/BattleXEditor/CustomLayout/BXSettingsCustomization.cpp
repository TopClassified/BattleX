#include "BXSettingsCustomization.h"

#include "CustomLayout/SBXBehaviorRelationMatrix.h"
#include "CustomLayout/SBXStateRelationMatrix.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"

#define LOCTEXT_NAMESPACE "BXSettingsCustomization"



TSharedRef<IDetailCustomization> FBXSettingsCustomization::MakeInstance()
{
	return MakeShareable(new FBXSettingsCustomization());
}

void FBXSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& InDetailBuilder)
{
	// 关系矩阵收拢到BattleX单页(2026-09-16:两个关系设置类关闭自动注册,矩阵UI在此注入);
	// 两个独立分类便于分别折叠,Important优先级排在UBXSettings自己的Important属性之后、Collision/Task/Sync等默认分类之前
	IDetailCategoryBuilder& BehaviorCategory = InDetailBuilder.EditCategory(TEXT("BehaviorRelations"), LOCTEXT("BehaviorRelationsCategory", "行为关系矩阵"), ECategoryPriority::Important);
	BehaviorCategory.AddCustomRow(LOCTEXT("BehaviorMatrixRow", "行为关系矩阵"))
		.WholeRowContent()
		[
			SNew(SBXBehaviorRelationMatrix)
		];

	IDetailCategoryBuilder& StateCategory = InDetailBuilder.EditCategory(TEXT("StateRelations"), LOCTEXT("StateRelationsCategory", "状态关系矩阵"), ECategoryPriority::Important);
	StateCategory.AddCustomRow(LOCTEXT("StateMatrixRow", "状态关系矩阵"))
		.WholeRowContent()
		[
			SNew(SBXStateRelationMatrix)
		];
}

#undef LOCTEXT_NAMESPACE
