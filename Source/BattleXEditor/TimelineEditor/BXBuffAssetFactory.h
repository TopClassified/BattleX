#pragma once

#include "Factories/Factory.h"

#include "BXBuffAssetFactory.generated.h"



UCLASS(HideCategories = Object, MinimalAPI)
class UBXBuffAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UBXBuffAssetFactory(const FObjectInitializer& ObjectInitializer);

	virtual ~UBXBuffAssetFactory() {}

	virtual bool ConfigureProperties() override { return true; }

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};
