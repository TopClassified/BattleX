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
	// 关系矩阵收拢到BattleX单页(2026-09-16:两个关系设置类关闭自动注册,矩阵UI在此注入;
	// Important优先级+Relations分类排在UBXSettings自己的Important属性之后、Collision/Task/Sync等默认分类之前)
	IDetailCategoryBuilder& RelationsCategory = InDetailBuilder.EditCategory(TEXT("Relations"), LOCTEXT("RelationsCategory", "关系矩阵"), ECategoryPriority::Important);

	RelationsCategory.AddCustomRow(LOCTEXT("BehaviorMatrixRow", "行为关系矩阵"))
		.WholeRowContent()
		[
			SNew(SBXBehaviorRelationMatrix)
		];

	RelationsCategory.AddCustomRow(LOCTEXT("StateMatrixRow", "状态关系矩阵"))
		.WholeRowContent()
		[
			SNew(SBXStateRelationMatrix)
		];
}

#undef LOCTEXT_NAMESPACE
