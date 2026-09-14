#pragma once

#include "CoreMinimal.h"

// std::is_same_v/std::is_arithmetic_v(GetRecordValueText的if constexpr值文本化)
#include <type_traits>



// 操作记录条目(记录谁以什么目标值改动了竞争资源)
template <typename T>
struct FBXOperateRecord
{
#pragma region Data
public:
	// 句柄ID(0=基线);Push返回值,RemoveByID的定位键
	int64 UniqueID = 0;

	// 修改者标识(代理名/系统名)
	FName Modifier;

	// 该修改者声明的目标值(非增量,生效时的直接取值)
	T Value{};

	// 优先级,高者居栈顶;同级后压者居上
	uint8 Priority = 0;

#pragma endregion Data

};



/* 通用操作记录栈(纯C++结构体模板,参考KGCore UOperateStack的分层覆盖模型)
 * 裁决多系统对同一竞争资源的写冲突(如bMoveBlocked被多个行为代理与外部系统先后写,直接赋值互相踩踏):
 *  - 修改者Push(目标值,修改者名,优先级)拿到句柄ID,当前生效值恒等于栈顶(优先级最高;同优先级后压者胜)
 *  - 修改者结束时按句柄RemoveByID,生效值回落到余下栈顶而非盲目恢复旧值,天然免疫写入顺序竞态
 *  - 栈底[0]是基线记录(ID恒为0,不可移除),Initialize重新登记基线
 * 定位为最基础的记录簿:只记账不回写,生效值由调用方经GetEffectiveValue自行读取应用
 * 蓝图访问:核心为模板,UHT不支持模板USTRUCT——蓝图包装与类型库见OperateStack/BXOperateStackTypes.h
 * (基础类型薄壳+独立库类同名节点;结构体通配栈经FInstancedStruct+CustomThunk一个包装覆盖全部结构体类型)
 * 线程约定:仅游戏线程使用(纯数据记录,无锁) */
template <typename T>
struct TBXOperateStack
{
#pragma region Important
public:
	// 初始化:清空全部记录,登记基线值
	void Initialize(const T& InOriginValue, FName InStackName = NAME_None)
	{
		Records.Reset();

		FBXOperateRecord<T>& Baseline = Records.AddDefaulted_GetRef();
		Baseline.Value = InOriginValue;

		StackName = InStackName;
	}

	// 压入一条修改记录,返回句柄ID(0=失败);插入位置:严格更高优先级之下、同级记录之上(同级后压者胜)
	int64 Push(const T& InValue, FName InModifier, uint8 InPriority = 0)
	{
		if (Records.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[BXOperateStack] %s 未Initialize即Push,拒绝"), *StackName.ToString());
			return 0;
		}

		// 从栈顶向下找第一条优先级<=新记录的条目插到其后(基线Priority=0兜底,插入下标最低为1)
		int32 InsertIndex = 0;
		for (int32 i = Records.Num() - 1; i >= 0; --i)
		{
			if (Records[i].Priority <= InPriority)
			{
				InsertIndex = i + 1;
				break;
			}
		}

		FBXOperateRecord<T> NewRecord;
		NewRecord.UniqueID = NextID++;
		NewRecord.Modifier = InModifier;
		NewRecord.Value = InValue;
		NewRecord.Priority = InPriority;

		Records.Insert(MoveTemp(NewRecord), InsertIndex);
		return Records[InsertIndex].UniqueID;
	}

	// 按句柄移除记录,生效值回落到余下栈顶;ID=0(基线)与未命中返回false(重复移除静默,超时清理与正常收束双路径可无脑调用)
	bool RemoveByID(int64 InID)
	{
		if (InID == 0 || Records.Num() == 0)
		{
			return false;
		}

		// 从1起遍历:栈底基线不可移除
		for (int32 i = 1; i < Records.Num(); ++i)
		{
			if (Records[i].UniqueID == InID)
			{
				Records.RemoveAt(i);
				return true;
			}
		}

		return false;
	}

	// 按修改者注销其全部记录,生效值回落到余下栈顶;无记录返回false(与HasModifier配对,调用方免句柄簿记)
	bool RemoveByModifier(FName InModifier)
	{
		if (InModifier == NAME_None || Records.Num() == 0)
		{
			return false;
		}

		// 倒序遍历安全移除;从1起:栈底基线不可移除
		bool bRemoved = false;
		for (int32 i = Records.Num() - 1; i >= 1; --i)
		{
			if (Records[i].Modifier == InModifier)
			{
				Records.RemoveAt(i);
				bRemoved = true;
			}
		}

		return bRemoved;
	}

	// 替换某条记录声明的值(保持其优先级与栈位);基线不可替换(改基线请重新Initialize)
	bool ReplaceByID(int64 InID, const T& InNewValue)
	{
		if (InID == 0)
		{
			return false;
		}

		for (int32 i = 1; i < Records.Num(); ++i)
		{
			if (Records[i].UniqueID == InID)
			{
				Records[i].Value = InNewValue;
				return true;
			}
		}

		return false;
	}

	// 清除基线以外的全部记录(生效值回落到基线,所有句柄失效)
	void PopToOrigin()
	{
		if (Records.Num() <= 1)
		{
			return;
		}

		Records.SetNum(1);
	}

	// 清空一切(含基线);之后须重新Initialize才能再用(NextID只增不复用,旧句柄不会误命中新记录)
	void Reset()
	{
		Records.Reset();
		StackName = NAME_None;
	}

#pragma endregion Important



#pragma region Query
public:
	// 当前生效值=栈顶(未初始化时ensure并返回默认值)
	const T& GetEffectiveValue() const
	{
		if (!ensureMsgf(Records.Num() > 0, TEXT("[BXOperateStack] %s 未Initialize,GetEffectiveValue返回默认值"), *StackName.ToString()))
		{
			static T DefaultValue{};
			return DefaultValue;
		}

		return Records.Last().Value;
	}

	// 当前生效记录(谁在决定资源的值);未初始化返回nullptr
	const FBXOperateRecord<T>* GetTopRecord() const
	{
		return Records.Num() > 0 ? &Records.Last() : nullptr;
	}

	// 按句柄查记录(含基线ID=0);未命中返回nullptr
	const FBXOperateRecord<T>* FindByID(int64 InID) const
	{
		for (const FBXOperateRecord<T>& Record : Records)
		{
			if (Record.UniqueID == InID)
			{
				return &Record;
			}
		}

		return nullptr;
	}

	// 某修改者当前是否有记录在栈(跳过基线)
	bool HasModifier(FName InModifier) const
	{
		if (InModifier == NAME_None)
		{
			return false;
		}

		for (int32 i = 1; i < Records.Num(); ++i)
		{
			if (Records[i].Modifier == InModifier)
			{
				return true;
			}
		}

		return false;
	}

	// 是否已Initialize(基线在栈)
	bool IsInitialized() const { return Records.Num() > 0; }

	// 记录总数(含基线)
	int32 Num() const { return Records.Num(); }

	// 只读记录全集(下标0=基线,Last=栈顶);仅用于调试/UI展示
	const TArray<FBXOperateRecord<T>>& GetRecords() const { return Records; }

	FName GetStackName() const { return StackName; }

	// 自栈顶向栈底列出全部记录(调试用,值域仅bool/数值直接打印)
	FString GetDebugString() const
	{
		FString Result = FString::Printf(TEXT("[BXOperateStack %s] 栈顶→栈底 %d条:"), *StackName.ToString(), Records.Num());
		for (int32 i = Records.Num() - 1; i >= 0; --i)
		{
			const FBXOperateRecord<T>& Record = Records[i];
			Result += FString::Printf(TEXT("\n  [%d] id=%lld mod=%s pri=%u val=%s"),
				i, Record.UniqueID, *Record.Modifier.ToString(), Record.Priority, *GetRecordValueText(Record.Value));
		}
		return Result;
	}

#pragma endregion Query



#pragma region Internal
private:
	// 记录值文本化(T为任意类型无通用打印手段,复合类型仅提示)
	static FString GetRecordValueText(const T& InValue)
	{
		if constexpr (std::is_same_v<T, bool>)
		{
			return InValue ? TEXT("true") : TEXT("false");
		}
		else if constexpr (std::is_arithmetic_v<T>)
		{
			return FString::Printf(TEXT("%lld"), static_cast<long long>(InValue));
		}
		else
		{
			return TEXT("(复合类型,请自查GetRecords)");
		}
	}

	// 记录数组:优先级升序(下标0=基线,Last=栈顶=当前生效值)
	TArray<FBXOperateRecord<T>> Records;

	// 句柄ID计数器:只增不复用(防重新Initialize后旧句柄误删新记录);仅本栈内唯一
	int64 NextID = 1;

	// 栈名(诊断日志标识)
	FName StackName;

#pragma endregion Internal

};
