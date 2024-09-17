#include "BXTask.h"

#include "BXFunctionLibrary.h"



#pragma region Important
UBXTask::UBXTask()
{
	
}
#pragma endregion Important



#pragma region Editor
#if WITH_EDITOR
void UBXTask::SetDisplayName(FText InText)
{
	DisplayName = InText;
}

FText UBXTask::GetDisplayName() const
{
	if (DisplayName.IsEmpty())
	{
		FString Name = this->GetName();
		Name = Name.Replace(TEXT("BP_BXT_"), TEXT(""));
		Name = Name.Replace(TEXT("_C"), TEXT(""));
		Name = Name.Replace(TEXT("_"), TEXT(":"));

		return FText::FromString(Name);
	}

	return DisplayName;
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
