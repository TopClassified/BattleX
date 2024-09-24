#include "BXTLAsset.h" 



#pragma region Important
UBXTLAsset::UBXTLAsset()
{
	Sections.Add(FBXTLSection());
	StartSectionIndexes.Add(0);
}

void UBXTLAsset::CollectResources()
{

}

#pragma endregion Important



#pragma region Editor
#if WITH_EDITOR
void UBXTLAsset::InitByEditor(UObject* WorldContext)
{
	
}

void UBXTLAsset::PreEditChange(FProperty* PropertyThatWillChange)
{
	Super::PreEditChange(PropertyThatWillChange);
	
}

void UBXTLAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UBXTLAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{

	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}

void UBXTLAsset::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	ID = FCString::Atoi(*GetName());

	RefreshSections();
}

void UBXTLAsset::RefreshSections()
{
	for (int32 i = 0; i < Sections.Num(); ++i)
	{
		FBXTLSection& Section = Sections[i];
		Section.KeyFrames.Empty();
		Section.TaskList.Empty();
		if (Section.Name.ToString().Contains(TEXT("Section_")) || Section.Name.IsEmpty())
		{
			Section.Name = FText::FromString(TEXT("Section_") + FString::FromInt(i));
		}

		for (int32 j = 0; j < Section.Groups.Num(); ++j)
		{
			FBXTLTaskGroup& Group = Section.Groups[j];
			if (Group.Name.ToString().Contains(TEXT("Group_")) || Group.Name.IsEmpty())
			{
				Group.Name = FText::FromString(TEXT("Group_") + FString::FromInt(j));
			}

			for (int32 k = 0; k < Group.TaskList.Num(); ++k)
			{
				TWeakObjectPtr<UBXTask> Task = Group.TaskList[k];
				if (Task.IsValid())
				{
					Section.TaskList.Add(Group.TaskList[k].Get());
				}
			}
		}

		for (int32 j = 0; j < Section.TaskList.Num(); ++j)
		{
			UBXTask* Task = Section.TaskList[j];
			if (!Task)
			{
				continue;
			}

			bool TriggerByTimeline = (Task->TriggerTypes & (1 << (int32)EBXTTriggerType::T_Timeline)) > 0;
			if (TriggerByTimeline)
			{
				float StartTime = Task->StartTime;

				int32 Index = -1;
				
				// 尝试找到关键帧
				for (int32 k = 0; k < Section.KeyFrames.Num(); ++k)
				{
					if (FMath::IsNearlyEqual(StartTime, Section.KeyFrames[k].Time))
					{
						Index = k;
					}
				}

				// 添加新的关键帧
				if (Index < 0)
				{
					for (int32 k = 0; k < Section.KeyFrames.Num(); ++k)
					{
						if (StartTime < Section.KeyFrames[k].Time)
						{
							Index = k;
							break;
						}
					}
				}
				else
				{
					Section.KeyFrames[Index].Tasks.Add(j);
					continue;
				}

				if (Index < 0)
				{
					Index = 0;
					Section.KeyFrames.Add(FBXTLKeyFrame(StartTime));
				}
				else
				{
					Section.KeyFrames.Insert(FBXTLKeyFrame(StartTime), Index);
				}
			}
		}
	}

	GetPackage()->MarkPackageDirty();
}

void UBXTLAsset::AlignTimeProperty(float InAlign)
{
	for (TArray<FBXTLSection>::TIterator It(Sections); It; ++It)
	{
		It->Duration = UBXFunctionLibrary::AlignTime(It->Duration, InAlign);

		for (TMap<EBXTLExtraType, FVector2f>::TIterator It2(It->ExtraTime); It2; ++It2)
		{
			It2->Value.X = UBXFunctionLibrary::AlignTime(It2->Value.X, InAlign);
			It2->Value.Y = UBXFunctionLibrary::AlignTime(It2->Value.Y, InAlign);
		}

		for (TArray<UBXTask*>::TIterator It2(It->TaskList); It2; ++It2)
		{
			(*It2)->AlignTimeProperty(InAlign);
		}
	}
}

void UBXTLAsset::CleanInvalidTaskDependency(UBXTask* InvalidTask)
{
	if (!IsValid(InvalidTask))
	{
		return;
	}

	for (int32 i = 0; i < Sections.Num(); ++i)
	{
		FBXTLSection& Section = Sections[i];

		for (int32 j = 0; j < Section.TaskList.Num(); ++j)
		{
			UBXTask* Task = Section.TaskList[j];
			if (!Task || Task == InvalidTask)
			{
				continue;
			}

			// 检查事件
			for (TMap<FName, FBXTEvent>::TIterator It(Task->Events); It; ++It)
			{
				for (TMap<TSoftObjectPtr<UBXTask>, float>::TIterator It2(It->Value.Event); It2; ++It2)
				{
					if (It2->Key.LoadSynchronous() == InvalidTask)
					{
						It2.RemoveCurrent();
					}
				}
			}

			// 检查碰撞传入数据
			for (TArray<FBXTInputInfo>::TIterator It(Task->CollisionInputDatas); It; ++It)
			{
				if (It->DataTask.LoadSynchronous() == InvalidTask)
				{
					It->DataTask = nullptr;
				}
			}

			// 检查传入数据
			for (TArray<FBXTInputInfo>::TIterator It(Task->InputDatas); It; ++It)
			{
				if (It->DataTask.LoadSynchronous() == InvalidTask)
				{
					It->DataTask = nullptr;
				}
			}
		}
	}
}

bool UBXTLAsset::AddTask(int32 SectionID, int32 GroupID, UBXTask* Task)
{
	if (!IsValid(Task))
	{
		return false;
	}

	if (!Sections.IsValidIndex(SectionID))
	{
		return false;
	}
	FBXTLSection& Section = Sections[SectionID];

	if (!Section.Groups.IsValidIndex(GroupID))
	{
		return false;
	}
	FBXTLTaskGroup& Group = Section.Groups[GroupID];
	Group.TaskList.Add(Task);

	RefreshSections();

	return true;
}

bool UBXTLAsset::RemoveTask(int32 SectionID, int32 GroupID, UBXTask* Task)
{
	if (!IsValid(Task))
	{
		return false;
	}

	if (!Sections.IsValidIndex(SectionID))
	{
		return false;
	}
	FBXTLSection& Section = Sections[SectionID];

	if (!Section.Groups.IsValidIndex(GroupID))
	{
		return false;
	}
	FBXTLTaskGroup& Group = Section.Groups[GroupID];

	CleanInvalidTaskDependency(Task);
	Group.TaskList.Remove(Task);

	RefreshSections();

	return true;
}

bool UBXTLAsset::RemoveTask(UBXTask* Task)
{
	for (int32 i = 0; i < Sections.Num(); ++i)
	{
		for (int32 j = 0; j < Sections[i].Groups.Num(); ++j)
		{
			int32 Index = Sections[i].Groups[j].TaskList.Find(Task);
			if (Index >= 0)
			{
				return RemoveTask(i, j, Task);
			}
		}
	}

	return false;
}

bool UBXTLAsset::GetSectionIDAndGroupID(UBXTask* Task, int32& OutSectionID, int32& OutGroupID)
{
	for (int32 i = 0; i < Sections.Num(); ++i)
	{
		for (int32 j = 0; j < Sections[i].Groups.Num(); ++j)
		{
			int32 Index = Sections[i].Groups[j].TaskList.Find(Task);
			if (Index >= 0)
			{
				OutSectionID = i;
				OutGroupID = j;
				return true;
			}
		}
	}

	return false;
}

bool UBXTLAsset::AddGroup(int32 SectionID, FText NewGroupName)
{
	if (!Sections.IsValidIndex(SectionID))
	{
		return false;
	}
	FBXTLSection& Section = Sections[SectionID];

	Section.Groups.Add(FBXTLTaskGroup(NewGroupName));

	RefreshSections();

	return true;
}

bool UBXTLAsset::RemoveGroup(int32 SectionID, int32 GroupID)
{
	if (!Sections.IsValidIndex(SectionID))
	{
		return false;
	}
	FBXTLSection& Section = Sections[SectionID];
	
	if (!Section.Groups.IsValidIndex(GroupID))
	{
		return false;
	}
	FBXTLTaskGroup& Group = Section.Groups[GroupID];

	for (int32 i = 0; i < Group.TaskList.Num(); ++i)
	{
		RemoveTask(SectionID, GroupID, Group.TaskList[i].Get());
	}

	Section.Groups.RemoveAt(GroupID);

	return true;
}

bool UBXTLAsset::AddSection(FText NewSectionName)
{
	Sections.Add(FBXTLSection(NewSectionName));

	RefreshSections();

	return true;
}

bool UBXTLAsset::RemoveSection(int32 SectionID)
{
	if (!Sections.IsValidIndex(SectionID))
	{
		return false;
	}
	FBXTLSection& Section = Sections[SectionID];

	for (int32 i = 0; i < Section.Groups.Num(); ++i)
	{
		RemoveGroup(SectionID, i);
	}

	Sections.RemoveAt(SectionID);

	return true;
}

void UBXTLAsset::ChangeSectionName(int32 SectionID, FText NewName)
{
	if (!Sections.IsValidIndex(SectionID))
	{
		return;
	}
	FBXTLSection& Section = Sections[SectionID];

	Section.Name = NewName;
}

void UBXTLAsset::ChangeSectionLoopTime(int32 SectionID, int32 NewLoop)
{
	if (!Sections.IsValidIndex(SectionID))
	{
		return;
	}
	FBXTLSection& Section = Sections[SectionID];

	Section.LoopTime = NewLoop;
}

void UBXTLAsset::ChangeSectionDuration(int32 SectionID, float NewDuration)
{
	if (!Sections.IsValidIndex(SectionID))
	{
		return;
	}
	FBXTLSection& Section = Sections[SectionID];

	Section.Duration = NewDuration;
}
#endif
#pragma endregion Editor
