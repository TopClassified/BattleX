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

	// UE5.8: FApplicationMode::AddTabFactory/RemoveTabFactory 已改为非虚函数,此处为普通成员(操作本Mode的TabFactories,隐藏基类版本)
	void AddTabFactory(FCreateWorkflowTabFactory FactoryCreator);

	void RemoveTabFactory(FName TabFactoryID);

	virtual void CreateModeTabs(const TSharedRef<class FBXTLEditor> InEditor, FWorkflowAllowedTabSet& OutTabFactories);

protected:
	TWeakPtr<class FBXTLEditor> CachedEditor;

	FWorkflowAllowedTabSet TabFactories;

};
