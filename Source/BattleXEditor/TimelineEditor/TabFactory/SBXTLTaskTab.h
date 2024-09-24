#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "IDetailsView.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h" 



class SBXTLTaskDetailTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBXTLTaskDetailTab) {}
	SLATE_ARGUMENT(TSharedPtr<class SWidget>, TopContent)
	SLATE_ARGUMENT(TSharedPtr<class SWidget>, BottomContent)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetDetailObject(UObject* InObject);

private:
	TSharedPtr<class IDetailsView> DetailsView;

};
