#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Misc/DefaultValueHelper.h"
#include "ScopedTransaction.h"
#include "SGraphPin.h" 



class SBXTLGraphTransitionPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SBXTLGraphTransitionPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

protected:
	virtual FSlateColor GetPinTextColor() const;

};
