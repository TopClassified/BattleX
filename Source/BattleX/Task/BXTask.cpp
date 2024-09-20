#include "BXTask.h"

#include "BXFunctionLibrary.h"



#pragma region Important
UBXTask::UBXTask()
{
	
}
#pragma endregion Important



#pragma region Editor
#if WITH_EDITOR
void UBXTask::AlignTimeProperty(float InAlign)
{
	StartTime = UBXFunctionLibrary::AlignTime(StartTime, InAlign);
	Duration = UBXFunctionLibrary::AlignTime(Duration, InAlign);

	for (TMap<FName, FBXTEvent>::TIterator It(Events); It; ++It)
	{
		for (TMap<TSoftObjectPtr<class UBXTask>, float>::TIterator It2(It->Value.Event); It2; ++It2)
		{
			It2->Value = UBXFunctionLibrary::AlignTime(It2->Value, InAlign);
		}
	}
}

FText UBXTask::GetDisplayName() const
{
	FString Name = this->GetName();
	Name = Name.Replace(TEXT("BP_BXT_"), TEXT(""));
	Name = Name.Replace(TEXT("_C"), TEXT(""));
	Name = Name.Replace(TEXT("_"), TEXT(":"));

	return FText::FromString(Name);
}

FText UBXTask::GetAnnotation() const
{
	return Annotation;
}

void UBXTask::CopyDataFromOther(UBXTask* Other)
{
	UBXFunctionLibrary::CopyObject(this, Other);

	Events.Reset();

	InputDatas.Reset();
	OutputDatas.Reset();
	CollisionInputDatas.Reset();
}

bool UBXTask::RefreshProperty()
{
	return false;
}

void UBXTask::RefreshDataByPreviewObject_Implementation(UObject* InObject, const FBXTLPreviewObjectData& InData)
{

}

void UBXTask::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
#pragma endregion Editor
