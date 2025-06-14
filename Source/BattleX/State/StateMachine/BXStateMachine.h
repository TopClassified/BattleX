#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"

#include "BXStateMachine.generated.h"



// 状态机执行的函数
UENUM(BlueprintType, Meta = (Bitflags))
enum class EBXStateMachineFunction : uint8
{
	SMF_NativeInitialize         = 0                  UMETA(DisplayName = "C++初始化"),
	SMF_BPInitialize                                  UMETA(DisplayName = "蓝图初始化"),
	SMF_NativeCleanup                                 UMETA(DisplayName = "C++清理"),
	SMF_BPCleanup                                     UMETA(DisplayName = "蓝图清理"),
	SMF_NativeUpdate                                  UMETA(DisplayName = "C++更新"),
	SMF_BPUpdate                                      UMETA(DisplayName = "蓝图更新"),
	SMF_NativeEnterState                              UMETA(DisplayName = "C++进入状态"),
	SMF_BPEnterState                                  UMETA(DisplayName = "蓝图进入状态"),
	SMF_NativeExitState                               UMETA(DisplayName = "C++退出状态"),
	SMF_BPExitState                                   UMETA(DisplayName = "蓝图退出状态"),

	SMF_TMax                                          UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EBXStateMachineFunction);



UCLASS(Abstract, BlueprintType, Blueprintable)
class BATTLEX_API UBXStateMachine : public UObject
{
	GENERATED_BODY()

#pragma region Important
public:
	// 初始化
	UFUNCTION(BlueprintCallable)
	bool Initialize();
	virtual bool NativeInitialize();
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptInitialize();
	
	// 逆初始化
	UFUNCTION(BlueprintCallable)
	bool Deinitialize();
	virtual bool NativeDeinitialize();
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptDeinitialize();

	// 更新
	UFUNCTION(BlueprintCallable)
	void Update(float InDeltaTime);
	virtual void NativeUpdate(float InDeltaTime);
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptUpdate(float InDeltaTime);

public:
	// 要执行的函数
	UPROPERTY(EditDefaultsOnly, Meta = (Bitmask, BitmaskEnum = "/Script/BattleX.EBXStateMachineFunction"))
	int32 StateMachineFunctions = 341;
	
#pragma endregion Important



#pragma region State
public:
	// 进入状态
	UFUNCTION(BlueprintCallable)
	bool EnterState(const FGameplayTag& InState, const FInstancedStruct& InParameter);
	virtual bool NativeEnterState(const FGameplayTag& InState, const FInstancedStruct& InParameter);
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptEnterState(const FGameplayTag& InState, const FInstancedStruct& InParameter);

	// 退出状态
	UFUNCTION(BlueprintCallable)
	bool ExitState(const FGameplayTag& InState, const FInstancedStruct& InParameter);
	virtual bool NativeExitState(const FGameplayTag& InState, const FInstancedStruct& InParameter);
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptExitState(const FGameplayTag& InState, const FInstancedStruct& InParameter);

#pragma endregion State
	
};
