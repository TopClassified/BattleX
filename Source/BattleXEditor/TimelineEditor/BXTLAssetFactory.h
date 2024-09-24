#pragma once

#include "Factories/Factory.h"

#include "BXTLAssetFactory.generated.h" 



UCLASS(HideCategories = Object, MinimalAPI)
class UBXTLAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UBXTLAssetFactory(const FObjectInitializer& ObjectInitializer);

	virtual ~UBXTLAssetFactory() {}

	virtual bool ConfigureProperties() override { return true; }

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};



UCLASS(HideCategories = Object, MinimalAPI)
class UBXSkillAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UBXSkillAssetFactory(const FObjectInitializer& ObjectInitializer);

	virtual ~UBXSkillAssetFactory() {}

	virtual bool ConfigureProperties() override { return true; }

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};
