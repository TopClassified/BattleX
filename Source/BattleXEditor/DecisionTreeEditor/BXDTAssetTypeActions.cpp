#include "BXDTAssetTypeActions.h"
#include "Misc/MessageDialog.h"

#include "BXDTEditor.h"

#include "BXDecisionTreeTemplate.h"
#include "BeatenTree/BXBeatenTreeTemplate.h"
#include "CombatTree/BXCombatTreeTemplate.h"



#define LOCTEXT_NAMESPACE "AssetTypeActions"

FBXDecisionTreeAssetTypeActions::FBXDecisionTreeAssetTypeActions(EAssetTypeCategories::Type InAssetCategory) : MyAssetCategory(InAssetCategory)
{
	
}

FText FBXDecisionTreeAssetTypeActions::GetName() const
{
	return LOCTEXT("FBXDecisionTreeAssetTypeActions", "DecisionTree");
}

FColor FBXDecisionTreeAssetTypeActions::GetTypeColor() const
{
	return FColor::Green;
}

UClass* FBXDecisionTreeAssetTypeActions::GetSupportedClass() const
{
	return UBXDecisionTreeTemplate::StaticClass();
}

void FBXDecisionTreeAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UBXDecisionTreeTemplate* Template = Cast<UBXDecisionTreeTemplate>(*ObjIt))
		{
			TSharedRef<FBXDTEditor> NewEditor(new FBXDTEditor());
			NewEditor->InitEditor(Mode, EditWithinLevelEditor, Template);
		}
	}
}

uint32 FBXDecisionTreeAssetTypeActions::GetCategories()
{
	MyAssetCategory = EAssetTypeCategories::Type::Gameplay;

	return MyAssetCategory;
}

void FBXDecisionTreeAssetTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	
}






FBXBeatenTreeAssetTypeActions::FBXBeatenTreeAssetTypeActions(EAssetTypeCategories::Type InAssetCategory) : MyAssetCategory(InAssetCategory)
{
	
}

FText FBXBeatenTreeAssetTypeActions::GetName() const
{
	return LOCTEXT("FBXBeatenTreeAssetTypeActions", "BeatenTree");
}

FColor FBXBeatenTreeAssetTypeActions::GetTypeColor() const
{
	return FColor::Blue;
}

UClass* FBXBeatenTreeAssetTypeActions::GetSupportedClass() const
{
	return UBXBeatenTreeTemplate::StaticClass();
}

void FBXBeatenTreeAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UBXDecisionTreeTemplate* Template = Cast<UBXDecisionTreeTemplate>(*ObjIt))
		{
			TSharedRef<FBXDTEditor> NewEditor(new FBXDTEditor());
			NewEditor->InitEditor(Mode, EditWithinLevelEditor, Template);
		}
	}
}

uint32 FBXBeatenTreeAssetTypeActions::GetCategories()
{
	MyAssetCategory = EAssetTypeCategories::Type::Gameplay;

	return MyAssetCategory;
}

void FBXBeatenTreeAssetTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{

}






FBXCombatTreeAssetTypeActions::FBXCombatTreeAssetTypeActions(EAssetTypeCategories::Type InAssetCategory) : MyAssetCategory(InAssetCategory)
{
	
}

FText FBXCombatTreeAssetTypeActions::GetName() const
{
	return LOCTEXT("FBXCombatTreeAssetTypeActions", "CombatTree");
}

FColor FBXCombatTreeAssetTypeActions::GetTypeColor() const
{
	return FColor::Red;
}

UClass* FBXCombatTreeAssetTypeActions::GetSupportedClass() const
{
	return UBXCombatTreeTemplate::StaticClass();
}

void FBXCombatTreeAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UBXDecisionTreeTemplate* Template = Cast<UBXDecisionTreeTemplate>(*ObjIt))
		{
			TSharedRef<FBXDTEditor> NewEditor(new FBXDTEditor());
			NewEditor->InitEditor(Mode, EditWithinLevelEditor, Template);
		}
	}
}

uint32 FBXCombatTreeAssetTypeActions::GetCategories()
{
	MyAssetCategory = EAssetTypeCategories::Type::Gameplay;

	return MyAssetCategory;
}

void FBXCombatTreeAssetTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{

}

#undef LOCTEXT_NAMESPACE