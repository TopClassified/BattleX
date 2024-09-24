#pragma once

#include "ComponentVisualizer.h"

#include "BXShapeComponent.h" 



class FBXShapeComponentVisualizer : public FComponentVisualizer
{
#pragma region Important
public:
	FBXShapeComponentVisualizer();
	
	virtual ~FBXShapeComponentVisualizer();

#pragma endregion Important



#pragma region ComponentVisualizer
public:
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

protected:
	void DrawCircle(FPrimitiveDrawInterface* PDI, const FVector& Center, const FVector& X, const FVector& Y, const float Radius);

	void DrawHalfCircle(FPrimitiveDrawInterface* PDI, const FVector& Center, const FVector& X, const FVector& Y, const float Radius);

#pragma endregion ComponentVisualizer

};
