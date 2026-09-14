#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StructUtils/InstancedStruct.h"

#include "BXOperateStack.h"

#include "BXOperateStackTypes.generated.h"



// 通配参数占位类型(仅用于CustomThunk函数签名;实际引脚经CustomStructureParam变通配,任何结构体类型可接入)
USTRUCT(BlueprintType)
struct BATTLEX_API FBXOperateStackAny
{
	GENERATED_USTRUCT_BODY()

public:
};



// 操作记录栈的bool实例包装(运行期簿记不序列化;内部无UObject引用无GC顾虑)
USTRUCT(BlueprintType)
struct BATTLEX_API FBXOperateStackBool
{
	GENERATED_USTRUCT_BODY()

#pragma region Stack
public:
	// 核心模板栈(bool实例)
	TBXOperateStack<bool> Stack;

#pragma endregion Stack

};



// 操作记录栈的float实例包装(运行期簿记不序列化)
USTRUCT(BlueprintType)
struct BATTLEX_API FBXOperateStackFloat
{
	GENERATED_USTRUCT_BODY()

#pragma region Stack
public:
	// 核心模板栈(float实例)
	TBXOperateStack<float> Stack;

#pragma endregion Stack

};



// 操作记录栈的int32实例包装(运行期簿记不序列化)
USTRUCT(BlueprintType)
struct BATTLEX_API FBXOperateStackInt
{
	GENERATED_USTRUCT_BODY()

#pragma region Stack
public:
	// 核心模板栈(int32实例)
	TBXOperateStack<int32> Stack;

#pragma endregion Stack

};



/* 操作记录栈的结构体通配包装:核心=TBXOperateStack<FInstancedStruct>,类型擦除由FInstancedStruct承载,一个包装覆盖全部结构体值类型
 * 约定:一个栈一种值类型——Initialize的基线锚定类型,Push错型拒绝,GetEffectiveValue错型返回false不写出参 */
USTRUCT(BlueprintType)
struct BATTLEX_API FBXOperateStackStruct
{
	GENERATED_USTRUCT_BODY()

#pragma region Stack
public:
	// 核心模板栈(FInstancedStruct实例)
	TBXOperateStack<FInstancedStruct> Stack;

#pragma endregion Stack

};



/* bool版蓝图辅助函数库(只暴露最基本四个节点;C++侧绕过本库直接调模板成员函数)
 * 注:UHT不支持USTRUCT成员UFUNCTION(UhtFunctionParser的UFUNCTIONKeyword只注册Class/Interface等作用域),函数库是唯一蓝图路径;
 * BP无重载,每种基础类型独立库类承载同名节点 */
UCLASS()
class BATTLEX_API UBXOperateStackLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region BoolStack
public:
	// 登记/重登记基线值(清空全部记录,句柄全部失效)
	UFUNCTION(BlueprintCallable, Category = "OperateStack")
	static void Initialize(UPARAM(ref) FBXOperateStackBool& InStack, bool InOriginValue, FName InStackName);

	// 压入修改记录,返回句柄ID(0=失败);同级后压者胜
	UFUNCTION(BlueprintCallable, Category = "OperateStack")
	static int64 Push(UPARAM(ref) FBXOperateStackBool& InStack, bool InValue, FName InModifier, uint8 InPriority = 0);

	// 按句柄移除记录,生效值回落到余下栈顶;基线与未命中返回false
	UFUNCTION(BlueprintCallable, Category = "OperateStack")
	static bool RemoveByID(UPARAM(ref) FBXOperateStackBool& InStack, int64 InID);

	// 当前生效值=栈顶(未初始化时返回false)
	UFUNCTION(BlueprintPure, Category = "OperateStack")
	static bool GetEffectiveValue(const FBXOperateStackBool& InStack);

#pragma endregion BoolStack

};



// float版蓝图辅助函数库(与bool版同构,节点同名分库)
UCLASS()
class BATTLEX_API UBXOperateStackFloatLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region FloatStack
public:
	// 登记/重登记基线值(清空全部记录,句柄全部失效)
	UFUNCTION(BlueprintCallable, Category = "OperateStack")
	static void Initialize(UPARAM(ref) FBXOperateStackFloat& InStack, float InOriginValue, FName InStackName);

	// 压入修改记录,返回句柄ID(0=失败);同级后压者胜
	UFUNCTION(BlueprintCallable, Category = "OperateStack")
	static int64 Push(UPARAM(ref) FBXOperateStackFloat& InStack, float InValue, FName InModifier, uint8 InPriority = 0);

	// 按句柄移除记录,生效值回落到余下栈顶;基线与未命中返回false
	UFUNCTION(BlueprintCallable, Category = "OperateStack")
	static bool RemoveByID(UPARAM(ref) FBXOperateStackFloat& InStack, int64 InID);

	// 当前生效值=栈顶(未初始化时返回0)
	UFUNCTION(BlueprintPure, Category = "OperateStack")
	static float GetEffectiveValue(const FBXOperateStackFloat& InStack);

#pragma endregion FloatStack

};



// int32版蓝图辅助函数库(与bool版同构,节点同名分库)
UCLASS()
class BATTLEX_API UBXOperateStackIntLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region IntStack
public:
	// 登记/重登记基线值(清空全部记录,句柄全部失效)
	UFUNCTION(BlueprintCallable, Category = "OperateStack")
	static void Initialize(UPARAM(ref) FBXOperateStackInt& InStack, int32 InOriginValue, FName InStackName);

	// 压入修改记录,返回句柄ID(0=失败);同级后压者胜
	UFUNCTION(BlueprintCallable, Category = "OperateStack")
	static int64 Push(UPARAM(ref) FBXOperateStackInt& InStack, int32 InValue, FName InModifier, uint8 InPriority = 0);

	// 按句柄移除记录,生效值回落到余下栈顶;基线与未命中返回false
	UFUNCTION(BlueprintCallable, Category = "OperateStack")
	static bool RemoveByID(UPARAM(ref) FBXOperateStackInt& InStack, int64 InID);

	// 当前生效值=栈顶(未初始化时返回0)
	UFUNCTION(BlueprintPure, Category = "OperateStack")
	static int32 GetEffectiveValue(const FBXOperateStackInt& InStack);

#pragma endregion IntStack

};



/* 结构体版蓝图辅助函数库:Initialize/Push/GetEffectiveValue经CustomThunk+CustomStructureParam做通配引脚
 * (引脚连什么结构体类型就是什么类型,先例:KGCore execInitOS/UKismetArrayLibrary通配出引脚),一个包装覆盖全部结构体类型 */
UCLASS()
class BATTLEX_API UBXOperateStackStructLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#pragma region StructStack
public:
	// 登记/重登记基线值(清空全部记录,句柄全部失效;基线类型即本栈的值类型锚点)
	UFUNCTION(BlueprintCallable, Category = "OperateStack", CustomThunk, meta = (CustomStructureParam = "InOriginValue"))
	static void Initialize(UPARAM(ref) FBXOperateStackStruct& InStack, const FBXOperateStackAny& InOriginValue, FName InStackName);

	// 压入修改记录,返回句柄ID(0=失败:未初始化或与基线类型不符);同级后压者胜
	UFUNCTION(BlueprintCallable, Category = "OperateStack", CustomThunk, meta = (CustomStructureParam = "InValue"))
	static int64 Push(UPARAM(ref) FBXOperateStackStruct& InStack, const FBXOperateStackAny& InValue, FName InModifier, uint8 InPriority = 0);

	// 按句柄移除记录,生效值回落到余下栈顶;基线与未命中返回false
	UFUNCTION(BlueprintCallable, Category = "OperateStack")
	static bool RemoveByID(UPARAM(ref) FBXOperateStackStruct& InStack, int64 InID);

	// 输出当前生效值=栈顶(未初始化或出参与栈值类型不符返回false且不写OutValue)
	UFUNCTION(BlueprintPure, Category = "OperateStack", CustomThunk, meta = (CustomStructureParam = "OutValue"))
	static bool GetEffectiveValue(const FBXOperateStackStruct& InStack, FBXOperateStackAny& OutValue);

	DECLARE_FUNCTION(execInitialize)
	{
		// 参数1:栈(普通结构体引用,先缓存地址再步进通配参数)
		Stack.MostRecentProperty = nullptr;
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.StepCompiledIn<FStructProperty>(nullptr);
		void* StackAddress = Stack.MostRecentPropertyAddress;

		// 参数2:通配基线值
		Stack.MostRecentProperty = nullptr;
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.StepCompiledIn<FStructProperty>(nullptr);
		void* OriginAddress = Stack.MostRecentPropertyAddress;
		FStructProperty* OriginProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		// 参数3:栈名
		P_GET_PROPERTY(FNameProperty, InStackName);
		P_FINISH;
		P_NATIVE_BEGIN;
		if (StackAddress && OriginProperty && OriginAddress)
		{
			StructInitialize(*reinterpret_cast<FBXOperateStackStruct*>(StackAddress), OriginAddress, OriginProperty->Struct, InStackName);
		}
		P_NATIVE_END;
	}

	DECLARE_FUNCTION(execPush)
	{
		// 参数1:栈
		Stack.MostRecentProperty = nullptr;
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.StepCompiledIn<FStructProperty>(nullptr);
		void* StackAddress = Stack.MostRecentPropertyAddress;

		// 参数2:通配值
		Stack.MostRecentProperty = nullptr;
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.StepCompiledIn<FStructProperty>(nullptr);
		void* ValueAddress = Stack.MostRecentPropertyAddress;
		FStructProperty* ValueProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		// 参数3/4:修改者/优先级
		P_GET_PROPERTY(FNameProperty, InModifier);
		P_GET_PROPERTY(FByteProperty, InPriority);
		P_FINISH;
		P_NATIVE_BEGIN;
		int64 Result = 0;
		if (StackAddress && ValueProperty && ValueAddress)
		{
			Result = StructPush(*reinterpret_cast<FBXOperateStackStruct*>(StackAddress), ValueAddress, ValueProperty->Struct, InModifier, InPriority);
		}
		*(int64*)RESULT_PARAM = Result;
		P_NATIVE_END;
	}

	DECLARE_FUNCTION(execGetEffectiveValue)
	{
		// 参数1:栈
		Stack.MostRecentProperty = nullptr;
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.StepCompiledIn<FStructProperty>(nullptr);
		void* StackAddress = Stack.MostRecentPropertyAddress;

		// 参数2:通配出参
		Stack.MostRecentProperty = nullptr;
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.StepCompiledIn<FStructProperty>(nullptr);
		void* OutAddress = Stack.MostRecentPropertyAddress;
		FStructProperty* OutProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		P_FINISH;
		P_NATIVE_BEGIN;
		bool bResult = false;
		if (StackAddress && OutProperty && OutAddress)
		{
			bResult = StructGetEffective(*reinterpret_cast<FBXOperateStackStruct*>(StackAddress), OutAddress, OutProperty->Struct);
		}
		*(bool*)RESULT_PARAM = bResult;
		P_NATIVE_END;
	}

private:
	// Initialize落地(基线值按通配引脚实际类型写入,类型锚点随之确立)
	static void StructInitialize(FBXOperateStackStruct& InStack, const void* InOriginData, const UScriptStruct* InOriginType, FName InStackName);

	// Push落地(与基线类型不符拒绝)
	static int64 StructPush(FBXOperateStackStruct& InStack, const void* InValueData, const UScriptStruct* InValueType, FName InModifier, uint8 InPriority);

	// GetEffectiveValue落地(出参类型与栈值类型不符返回false不写出参)
	static bool StructGetEffective(const FBXOperateStackStruct& InStack, void* OutData, const UScriptStruct* OutType);

#pragma endregion StructStack

};
