#include "BXOperateStackTypes.h"



#pragma region BoolStack
void UBXOperateStackLibrary::Initialize(FBXOperateStackBool& InStack, bool InOriginValue, FName InStackName)
{
	InStack.Stack.Initialize(InOriginValue, InStackName);
}

int64 UBXOperateStackLibrary::Push(FBXOperateStackBool& InStack, bool InValue, FName InModifier, uint8 InPriority)
{
	return InStack.Stack.Push(InValue, InModifier, InPriority);
}

bool UBXOperateStackLibrary::RemoveByID(FBXOperateStackBool& InStack, int64 InID)
{
	return InStack.Stack.RemoveByID(InID);
}

bool UBXOperateStackLibrary::GetEffectiveValue(const FBXOperateStackBool& InStack)
{
	return InStack.Stack.GetEffectiveValue();
}

#pragma endregion BoolStack



#pragma region FloatStack
void UBXOperateStackFloatLibrary::Initialize(FBXOperateStackFloat& InStack, float InOriginValue, FName InStackName)
{
	InStack.Stack.Initialize(InOriginValue, InStackName);
}

int64 UBXOperateStackFloatLibrary::Push(FBXOperateStackFloat& InStack, float InValue, FName InModifier, uint8 InPriority)
{
	return InStack.Stack.Push(InValue, InModifier, InPriority);
}

bool UBXOperateStackFloatLibrary::RemoveByID(FBXOperateStackFloat& InStack, int64 InID)
{
	return InStack.Stack.RemoveByID(InID);
}

float UBXOperateStackFloatLibrary::GetEffectiveValue(const FBXOperateStackFloat& InStack)
{
	return InStack.Stack.GetEffectiveValue();
}

#pragma endregion FloatStack



#pragma region IntStack
void UBXOperateStackIntLibrary::Initialize(FBXOperateStackInt& InStack, int32 InOriginValue, FName InStackName)
{
	InStack.Stack.Initialize(InOriginValue, InStackName);
}

int64 UBXOperateStackIntLibrary::Push(FBXOperateStackInt& InStack, int32 InValue, FName InModifier, uint8 InPriority)
{
	return InStack.Stack.Push(InValue, InModifier, InPriority);
}

bool UBXOperateStackIntLibrary::RemoveByID(FBXOperateStackInt& InStack, int64 InID)
{
	return InStack.Stack.RemoveByID(InID);
}

int32 UBXOperateStackIntLibrary::GetEffectiveValue(const FBXOperateStackInt& InStack)
{
	return InStack.Stack.GetEffectiveValue();
}

#pragma endregion IntStack



#pragma region StructStack
bool UBXOperateStackStructLibrary::RemoveByID(FBXOperateStackStruct& InStack, int64 InID)
{
	return InStack.Stack.RemoveByID(InID);
}

void UBXOperateStackStructLibrary::StructInitialize(FBXOperateStackStruct& InStack, const void* InOriginData, const UScriptStruct* InOriginType, FName InStackName)
{
	FInstancedStruct Origin;
	Origin.InitializeAs(InOriginType, static_cast<const uint8*>(InOriginData));
	InStack.Stack.Initialize(Origin, InStackName);
}

int64 UBXOperateStackStructLibrary::StructPush(FBXOperateStackStruct& InStack, const void* InValueData, const UScriptStruct* InValueType, FName InModifier, uint8 InPriority)
{
	// 一个栈一种值类型:与基线锚点不符直接拒绝(基线由Initialize写入)
	if (const FBXOperateRecord<FInstancedStruct>* Baseline = InStack.Stack.FindByID(0))
	{
		if (Baseline->Value.GetScriptStruct() != InValueType)
		{
			UE_LOG(LogTemp, Warning, TEXT("[BXOperateStack] %s Push值类型与基线不符(基线:%s,传入:%s),拒绝"),
				*InStack.Stack.GetStackName().ToString(),
				*GetNameSafe(Baseline->Value.GetScriptStruct()), *GetNameSafe(InValueType));
			return 0;
		}
	}

	FInstancedStruct Value;
	Value.InitializeAs(InValueType, static_cast<const uint8*>(InValueData));
	return InStack.Stack.Push(Value, InModifier, InPriority);
}

bool UBXOperateStackStructLibrary::StructGetEffective(const FBXOperateStackStruct& InStack, void* OutData, const UScriptStruct* OutType)
{
	if (!InStack.Stack.IsInitialized() || OutType == nullptr)
	{
		return false;
	}

	const FBXOperateRecord<FInstancedStruct>* Top = InStack.Stack.GetTopRecord();
	if (Top == nullptr || Top->Value.GetScriptStruct() != OutType)
	{
		return false;
	}

	OutType->CopyScriptStruct(OutData, Top->Value.GetMemory());
	return true;
}

#pragma endregion StructStack
