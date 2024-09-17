#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"



class FBXTLEditorMode : public FApplicationMode
{
public:
	FBXTLEditorMode(const FName& InModeName, TSharedRef<class FBXTLEditor> InEditor);

	virtual ~FBXTLEditorMode() {}

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) final override;

	virtual void AddTabFactory(FCreateWorkflowTabFactory FactoryCreator) final override;

	virtual void RemoveTabFactory(FName TabFactoryID) final override;

	virtual void CreateModeTabs(const TSharedRef<class FBXTLEditor> InEditor, FWorkflowAllowedTabSet& OutTabFactories);

protected:
	TWeakPtr<class FBXTLEditor> CachedEditor;

	FWorkflowAllowedTabSet TabFactories;

};
