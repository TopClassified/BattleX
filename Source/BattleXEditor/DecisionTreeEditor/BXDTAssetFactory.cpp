#include "BXDTAssetFactory.h"

#include "BXDecisionTreeTemplate.h"
#include "BeatenTree/BXBeatenTreeTemplate.h"
#include "CombatTree/BXCombatTreeTemplate.h"

#include "Modules/ModuleManager.h"
#include "ClassViewerModule.h"
#include "ClassViewerFilter.h"

#include "Editor.h"
#include "Input/Reply.h"
#include "Widgets/SWindow.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Input/SComboButton.h"



#define LOCTEXT_NAMESPACE "DecisionTreeAssetFactory"

UBXDecisionTreeAssetFactory::UBXDecisionTreeAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UBXDecisionTreeAssetFactory::StaticClass();
}

UBXDecisionTreeAssetFactory::~UBXDecisionTreeAssetFactory()
{
	
}

bool UBXDecisionTreeAssetFactory::ConfigureProperties()
{
	class FDTTypeFilter : public IClassViewerFilter
	{
	public:
		virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
		{
			return InClass->IsChildOf<UBXDecisionTreeType>() && !InClass->HasAnyClassFlags(CLASS_Abstract);
		}

		virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
		{
			return InClass->IsChildOf(UBXDecisionTreeType::StaticClass()) && !InClass->HasAnyClassFlags(CLASS_Abstract);
		}
	};

	class FBXDecisionTreeFactoryUI : public TSharedFromThis<FBXDecisionTreeFactoryUI>
	{
	public:
		FReply OnCreate()
		{
			if (PickerWindow.IsValid())
			{
				PickerWindow->RequestDestroyWindow();
			}
			return FReply::Handled();
		}

		FReply OnCancel()
		{
			if (CachedFactory.IsValid())
			{
				CachedFactory->DecisionTreeTypeClass = nullptr;
			}

			if (PickerWindow.IsValid())
			{
				PickerWindow->RequestDestroyWindow();
			}
			
			return FReply::Handled();
		}

		TSharedRef<SWidget> GenerateDTTypePicker()
		{
			FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

			FClassViewerInitializationOptions Options;
			Options.Mode = EClassViewerMode::ClassPicker;
			Options.bIsActorsOnly = false;
			Options.bIsBlueprintBaseOnly = false;
			Options.ClassFilters.Add(MakeShared<FDTTypeFilter>());
			Options.ExtraPickerCommonClasses.Add(NULL);

			return SNew(SBox)
				.WidthOverride(330)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.FillHeight(1.0f)
					.MaxHeight(500)
					[
						SNew(SBorder)
						.Padding(4)
						.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
						[
							ClassViewerModule.CreateClassViewer(Options, FOnClassPicked::CreateSP(this, &FBXDecisionTreeFactoryUI::OnPickedDTTypeClassType))
						]
					]
				];
		}

		void OpenDecisionTreeConfigurator(UBXDecisionTreeAssetFactory* TheFactory)
		{
			CachedFactory = TheFactory;

			FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

			PickerWindow = SNew(SWindow)
				.Title(LOCTEXT("DecisionTreeConfigurator", "Select Decision Tree Options"))
				.ClientSize(FVector2D(350, 100))
				.SupportsMinimize(false)
				.SupportsMaximize(false)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("Menu.Background"))
					.Padding(10)
					[
						SNew(SVerticalBox)
						+SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(DTTypePickerAnchor, SComboButton)
							.ContentPadding(FMargin(2, 2, 2, 1))
							.MenuPlacement(MenuPlacement_BelowAnchor)
							.ButtonContent()
							[
								SNew(STextBlock)
								.Text(this, &FBXDecisionTreeFactoryUI::OnGetDTTypeTextValue)
							]
							.OnGetMenuContent(this, &FBXDecisionTreeFactoryUI::GenerateDTTypePicker)
						]

						+SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SHorizontalBox)
							+SHorizontalBox::Slot()
							.AutoWidth()
							.HAlign(HAlign_Left)
							[
								SNew(SButton)
								.Text(LOCTEXT("OK", "OK"))
								.OnClicked(this, &FBXDecisionTreeFactoryUI::OnCreate)
							]

							+ SHorizontalBox::Slot()
							.AutoWidth()
							.HAlign(HAlign_Right)
							[
								SNew(SButton)
								.Text(LOCTEXT("Cancel", "Cancel"))
								.OnClicked(this, &FBXDecisionTreeFactoryUI::OnCancel)
							]
						]
					]
				];

			GEditor->EditorAddModalWindow(PickerWindow.ToSharedRef());
			PickerWindow.Reset();
		}

	public:
		FText OnGetDTTypeTextValue() const
		{
			FString OutString;

			if (CachedFactory->DecisionTreeTypeClass)
			{
				if (UBXDecisionTreeType* CurDTT = Cast<UBXDecisionTreeType>(CachedFactory->DecisionTreeTypeClass->GetDefaultObject()))
				{
					OutString = TEXT("编辑类型为:") + CurDTT->GetTypeName();
				}
			}
			else
			{
				OutString = TEXT("无效的编辑类型集合!");
			}

			return FText::FromString(*OutString);
		}

		void OnPickedDTTypeClassType(UClass* ChosenClass) const
		{
			CachedFactory->DecisionTreeTypeClass = ChosenClass;
		}

	private:
		TSharedPtr<SWindow> PickerWindow;
		TSharedPtr<SComboButton> DTTypePickerAnchor;

		TWeakObjectPtr<UBXDecisionTreeAssetFactory> CachedFactory = nullptr;
	};

	TSharedRef<FBXDecisionTreeFactoryUI> DecisionTreeFactory = MakeShareable(new FBXDecisionTreeFactoryUI());
	DecisionTreeFactory->OpenDecisionTreeConfigurator(this);

	return true;
};

UObject* UBXDecisionTreeAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	if (!DecisionTreeTypeClass)
	{
		return nullptr;
	}

	UBXDecisionTreeTemplate* NewAsset = nullptr;
	NewAsset = NewObject<UBXDecisionTreeTemplate>(InParent, Class, Name, Flags | RF_Transactional);
	NewAsset->TreeType = DecisionTreeTypeClass;

	return NewAsset;
}

UObject* UBXDecisionTreeAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (!DecisionTreeTypeClass)
	{
		return nullptr;
	}

	UBXDecisionTreeTemplate* NewAsset = nullptr;
	NewAsset = NewObject<UBXDecisionTreeTemplate>(InParent, Class, Name, Flags | RF_Transactional);
	NewAsset->TreeType = DecisionTreeTypeClass;

	return NewAsset;
}






UBXBeatenTreeAssetFactory::UBXBeatenTreeAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UBXBeatenTreeTemplate::StaticClass();
}

UBXBeatenTreeAssetFactory::~UBXBeatenTreeAssetFactory()
{

}

bool UBXBeatenTreeAssetFactory::ConfigureProperties()
{
	return true;
};

UObject* UBXBeatenTreeAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return NewObject<UBXBeatenTreeTemplate>(InParent, Class, Name, Flags | RF_Transactional);
}

UObject* UBXBeatenTreeAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}






UBXCombatTreeAssetFactory::UBXCombatTreeAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UBXCombatTreeTemplate::StaticClass();
}

UBXCombatTreeAssetFactory::~UBXCombatTreeAssetFactory()
{

}

bool UBXCombatTreeAssetFactory::ConfigureProperties()
{
	return true;
};

UObject* UBXCombatTreeAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return NewObject<UBXCombatTreeTemplate>(InParent, Class, Name, Flags | RF_Transactional);
}

UObject* UBXCombatTreeAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}

#undef LOCTEXT_NAMESPACE
