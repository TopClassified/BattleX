#pragma once

#include "CoreMinimal.h"
#include "SBXTLGraphNode.h"



// BUFF图表节点的自定义显示控件(在节点下方渲染层级配置)
class SBXBuffGraphNode : public SBXTLGraphNode
{
public:
	SLATE_BEGIN_ARGS(SBXBuffGraphNode) {}
	SLATE_END_ARGS()

	// 构造
	void Construct(const FArguments& InArgs, class UBXBuffGraphNode* InNode);

	// 更新节点显示
	void UpdateGraphNode() override;

	// 在节点下方添加控件
	virtual void CreateBelowWidgetControls(TSharedPtr<SVerticalBox> MainBox) override;

private:
	// 获取MinLayer值
	int32 GetMinLayer() const;

	// 获取MaxLayer值
	int32 GetMaxLayer() const;

	// MinLayer变更回调
	void OnMinLayerChanged(int32 NewValue);

	// MaxLayer变更回调
	void OnMaxLayerChanged(int32 NewValue);

	// 创建层级配置控件
	TSharedRef<SWidget> CreateLayerConfigWidget();

};
