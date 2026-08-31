#include "BXProjectileAssetFactory.h"

#include "BXProjectileAsset.h"



UBXProjectileAssetFactory::UBXProjectileAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UBXProjectileAsset::StaticClass();
}

UBXProjectileAssetFactory::~UBXProjectileAssetFactory()
{

}

bool UBXProjectileAssetFactory::ConfigureProperties()
{
	return true;
}

UObject* UBXProjectileAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return NewObject<UBXProjectileAsset>(InParent, Class, Name, Flags | RF_Transactional);
}

UObject* UBXProjectileAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}
