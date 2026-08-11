#pragma once

#include "CoreMinimal.h"
#include "TabFactory/BXTLEditorMode.h"

class FBXBuffEditor;



// BUFF编辑器模式(继承Timeline模式,替换布局:去掉Timeline窗口,Graph占满下方)
class FBXBuffEditorMode : public FBXTLEditorMode
{
public:
	FBXBuffEditorMode(const FName& InModeName, TSharedRef<class FBXBuffEditor> InEditor);

	virtual ~FBXBuffEditorMode() {}

};
