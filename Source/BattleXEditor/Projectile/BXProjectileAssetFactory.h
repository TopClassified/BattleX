#pragma once

#include "Factories/Factory.h"

#include "BXProjectileAssetFactory.generated.h"



UCLASS(HideCategories = Object, MinimalAPI)
class UBXProjectileAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UBXProjectileAssetFactory(const FObjectInitializer& ObjectInitializer);
	virtual ~UBXProjectileAssetFactory();

	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~ End UFactory Interface
};
