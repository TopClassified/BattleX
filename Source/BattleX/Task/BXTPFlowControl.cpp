#include "BXTPFlowControl.h"
#include "BXTFlowControl.h"
#include "Condition/BXConditionManager.h"

void UBXTPSwitch::Start(FBXTLRunTimeData& InOutRTData, FBXTLSectionRTData& InOutRTSData, FBXTLTaskRTData& InOutRTTData)
{
	UBXTSwitch* Task = Cast<UBXTSwitch>(InOutRTTData.Task);
	if (!Task)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BXTSwitch] Task cast failed"));
		return;
	}

	UBXConditionManager* CondMgr = UBXConditionManager::Get(this);
	if (!CondMgr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BXTSwitch] ConditionManager is null"));
		return;
	}

	int64 Scope = UBXTProcessor::GenerateContextScope(InOutRTData, InOutRTTData);

	// 参数缓存：基类Param类型固定，Switch执行期间所有Case共用同一Param
	TOptional<FBXTaskConditionParameter> ParamOpt;
	bool bParamBuilt = false;

	for (int32 i = 0; i < Task->Cases.Num(); ++i)
	{
		const FBXTSwitchCase& Case = Task->Cases[i];
		if (!Case.Condition || !Case.EventTag.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("[BXTSwitch] Case[%d] skipped: Condition=%s EventTag=%s"), i, Case.Condition ? *Case.Condition->GetName() : TEXT("null"), Case.EventTag.IsValid() ? *Case.EventTag.ToString() : TEXT("invalid"));
			continue;
		}

		// 首次使用时构建参数，后续Case复用
		if (!bParamBuilt)
		{
			ParamOpt = CondMgr->BuildTaskConditionParameter<FBXTaskConditionParameter>(Case.Condition, InOutRTData, InOutRTSData, InOutRTTData);
			if (!ParamOpt.IsSet())
			{
				UE_LOG(LogTemp, Warning, TEXT("[BXTSwitch] Case[%d] BuildTaskConditionParameter failed, Condition=%s"), i, *Case.Condition->GetName());
				continue;
			}
			bParamBuilt = true;
		}

		if (CondMgr->CheckCondition(Case.Condition, *ParamOpt))
		{
			UBXTProcessor::AddPendingTask(InOutRTData, InOutRTSData, InOutRTTData, Scope, Case.EventTag);
			return;
		}
	}

	if (Task->DefaultEventTag.IsValid())
	{
		UBXTProcessor::AddPendingTask(InOutRTData, InOutRTSData, InOutRTTData, Scope, Task->DefaultEventTag);
	}
}
