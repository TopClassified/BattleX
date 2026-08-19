#pragma once

#include "CoreMinimal.h"
#include "BXTLEditor.h"



// BUFF编辑器(继承Timeline编辑器,覆写图表类型和编辑器模式)
class FBXBuffEditor : public FBXTLEditor
{
	typedef FBXTLEditor Super;

#pragma region Important
public:
	FBXBuffEditor();

	virtual ~FBXBuffEditor();

	// 创建编辑器模式
	virtual TSharedPtr<FApplicationMode> CreateEditorMode() override;

	// 获取图表类型
	virtual UClass* GetGraphClass() const override;

	// 获取图表Schema类型
	virtual UClass* GetGraphSchemaClass() const override;

	// 关闭(注销BUFF节点工厂)
	virtual void OnClose() override;

#pragma endregion Important

};
