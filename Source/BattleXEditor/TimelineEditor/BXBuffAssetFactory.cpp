#include "BXBuffAssetFactory.h"

#include "BattleXTimelineEditor.h"

#include "BXBuffAsset.h"


UBXBuffAssetFactory::UBXBuffAssetFactory(const FObjectInitializer& ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UBXBuffAsset::StaticClass();
}

UObject* UBXBuffAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	UBXBuffAsset* NewBuff = NewObject<UBXBuffAsset>(InParent, Class, Name, Flags | RF_Transactional);
	if (NewBuff)
	{
		const UBattleXTimelineEditor* EdSettings = GetDefault<UBattleXTimelineEditor>();

		UClass* PlayerType = EdSettings->PlayerType.Get() ? EdSettings->PlayerType.Get() : UBXTLPreviewActor::StaticClass();
		UClass* TargetType = EdSettings->TargetType.Get() ? EdSettings->TargetType.Get() : UBXTLPreviewActor::StaticClass();

		UBXTLPreviewActor* Information = NewObject<UBXTLPreviewActor>(NewBuff, PlayerType, NAME_None);
		Information->SpawnTransform.SetLocation(FVector(-150.0f, 0.0f, 100.0f));
		NewBuff->PlayerInformation = Information;

		Information = NewObject<UBXTLPreviewActor>(NewBuff, TargetType, NAME_None);
		Information->SpawnTransform.SetLocation(FVector(150.0f, 0.0f, 100.0f));
		Information->SpawnTransform.SetRotation(FRotator(0.0f, 180.0f, 0.0f).Quaternion());
		NewBuff->TargetInformation = Information;
	}

	return NewBuff;
}

UObject* UBXBuffAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}
