#include "SBXTLTaskTab.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h" 



void SBXTLTaskDetailTab::Construct(const FArguments& InArgs)
{
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bHideSelectionTip = true;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	TSharedRef<SVerticalBox> Content = SNew(SVerticalBox);

	if (InArgs._TopContent.IsValid())
	{
		Content->AddSlot()
		.AutoHeight()
		[
			InArgs._TopContent.ToSharedRef()
		];
	}

	Content->AddSlot()
	.FillHeight(1.0f)
	[
		DetailsView.ToSharedRef()
	];

	if (InArgs._BottomContent.IsValid())
	{
		Content->AddSlot()
		.AutoHeight()
		[
			InArgs._BottomContent.ToSharedRef()
		];
	}

	ChildSlot
	[
		Content
	];
}

void SBXTLTaskDetailTab::SetDetailObject(UObject* InObject)
{
	DetailsView->SetObject(InObject, true);
}
