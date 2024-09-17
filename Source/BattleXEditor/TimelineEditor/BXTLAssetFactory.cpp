#include "BXTLAssetFactory.h"

#include "BXTLEditorSettings.h"

#include "BXTLAsset.h"
#include "BXSkillAsset.h"



UBXTLAssetFactory::UBXTLAssetFactory(const FObjectInitializer& ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UBXTLAsset::StaticClass();
}

UObject* UBXTLAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	UBXTLAsset* NewTimeline = NewObject<UBXTLAsset>(InParent, Class, Name, Flags | RF_Transactional);
	if (NewTimeline)
	{
		const UBXTLEditorSettings* EdSettings = GetDefault<UBXTLEditorSettings>();

		UClass* PlayerType = EdSettings->PlayerType.Get() ? EdSettings->PlayerType.Get() : UBXTLPreviewActor::StaticClass();
		UClass* TargetType = EdSettings->TargetType.Get() ? EdSettings->TargetType.Get() : UBXTLPreviewActor::StaticClass();

		UBXTLPreviewActor* Information = NewObject<UBXTLPreviewActor>(NewTimeline, PlayerType, NAME_None);
		Information->SpawnTransform.SetLocation(FVector(-150.0f, 0.0f, 100.0f));
		NewTimeline->PlayerInformation = Information;

		Information = NewObject<UBXTLPreviewActor>(NewTimeline, TargetType, NAME_None);
		Information->SpawnTransform.SetLocation(FVector(150.0f, 0.0f, 100.0f));
		Information->SpawnTransform.SetRotation(FRotator(0.0f, 180.0f, 0.0f).Quaternion());
		NewTimeline->TargetInformation = Information;
	}

	return NewTimeline;
}

UObject* UBXTLAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}



UBXSkillAssetFactory::UBXSkillAssetFactory(const FObjectInitializer& ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UBXSkillAsset::StaticClass();
}

UObject* UBXSkillAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	UBXSkillAsset* NewSkill = NewObject<UBXSkillAsset>(InParent, Class, Name, Flags | RF_Transactional);
	if (NewSkill)
	{
		const UBXTLEditorSettings* EdSettings = GetDefault<UBXTLEditorSettings>();

		UClass* PlayerType = EdSettings->PlayerType.Get() ? EdSettings->PlayerType.Get() : UBXTLPreviewActor::StaticClass();
		UClass* TargetType = EdSettings->TargetType.Get() ? EdSettings->TargetType.Get() : UBXTLPreviewActor::StaticClass();

		UBXTLPreviewActor* Information = NewObject<UBXTLPreviewActor>(NewSkill, PlayerType, NAME_None);
		Information->SpawnTransform.SetLocation(FVector(-150.0f, 0.0f, 100.0f));
		NewSkill->PlayerInformation = Information;

		Information = NewObject<UBXTLPreviewActor>(NewSkill, TargetType, NAME_None);
		Information->SpawnTransform.SetLocation(FVector(150.0f, 0.0f, 100.0f));
		Information->SpawnTransform.SetRotation(FRotator(0.0f, 180.0f, 0.0f).Quaternion());
		NewSkill->TargetInformation = Information;
	}

	return NewSkill;
}

UObject* UBXSkillAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}
