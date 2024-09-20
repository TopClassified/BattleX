#include "SBXTLGraphTransitionPin.h"

#include "BXTask.h"
#include "BXTLAsset.h"



void SBXTLGraphTransitionPin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

FSlateColor SBXTLGraphTransitionPin::GetPinTextColor() const
{
	return FLinearColor::Black;
}
