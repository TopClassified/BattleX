#pragma once

#include "Factories/Factory.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

#include "BXDTAssetFactory.generated.h"



UCLASS(HideCategories = Object, MinimalAPI)
class UBXDecisionTreeAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UBXDecisionTreeAssetFactory(const FObjectInitializer& ObjectInitializer);
	virtual ~UBXDecisionTreeAssetFactory();

	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface	

public:
	UPROPERTY()
	UClass* DecisionTreeTypeClass = nullptr;

};






UCLASS(HideCategories = Object, MinimalAPI)
class UBXBeatenTreeAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UBXBeatenTreeAssetFactory(const FObjectInitializer& ObjectInitializer);
	virtual ~UBXBeatenTreeAssetFactory();

	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface	

public:
	UPROPERTY()
	UClass* BeatenTreeTypeClass = nullptr;

};






UCLASS(HideCategories = Object, MinimalAPI)
class UBXCombatTreeAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UBXCombatTreeAssetFactory(const FObjectInitializer& ObjectInitializer);
	virtual ~UBXCombatTreeAssetFactory();

	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface	

public:
	UPROPERTY()
	UClass* CombatTreeTypeClass = nullptr;

};
