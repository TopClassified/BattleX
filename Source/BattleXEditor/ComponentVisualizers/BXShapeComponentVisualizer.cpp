#include "BXShapeComponentVisualizer.h"

#include "BXFunctionLibrary.h"



#pragma region Important
FBXShapeComponentVisualizer::FBXShapeComponentVisualizer() : FComponentVisualizer()
{

}

FBXShapeComponentVisualizer::~FBXShapeComponentVisualizer()
{

}

#pragma endregion Important



#pragma region ComponentVisualizer
void FBXShapeComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (!View || !PDI)
	{
		return;
	}

	const UBXShapeComponent* ShapeComponent = Cast<UBXShapeComponent>(Component);
	if (!ShapeComponent)
	{
		return;
	}

	AActor* Owner = ShapeComponent->GetOwner();
	if (!Owner)
	{
		return;
	}

	UClass* ActorClass = Owner->GetClass();
	if (!ActorClass)
	{
		return;
	}

	USceneComponent* AttachParent = Owner->GetRootComponent();
	for (TMap<FName, FBXShapeInformation>::TConstIterator It(ShapeComponent->ShapeInformations); It; ++It)
	{
		AttachParent = UBXFunctionLibrary::GetSceneComponentByNameAndClass(Owner, It->Value.AttachToComponent, nullptr, false);
		if (!AttachParent)
		{
			AttachParent = Owner->GetRootComponent();
		}

		FTransform WTransform = It->Value.Relation * AttachParent->GetSocketTransform(It->Value.AttachToSocket.BoneName);

		if (It->Value.ShapeType == EBXShapeType::ST_Sphere)
		{
			int32 Segments = 16;
			FVector WLocation = WTransform.GetLocation();

			const float AngleInc = 2.f * UE_PI / Segments;
			int32 NumSegmentsY = Segments;
			float Latitude = AngleInc;
			float SinY1 = 0.0f;
			float CosY1 = 1.0f;

			while (NumSegmentsY--)
			{
				const float SinY2 = FMath::Sin(Latitude);
				const float CosY2 = FMath::Cos(Latitude);

				FVector Vertex1 = FVector(SinY1, 0.0f, CosY1) * It->Value.ShapeSize.X + WLocation;
				FVector Vertex3 = FVector(SinY2, 0.0f, CosY2) * It->Value.ShapeSize.X + WLocation;
				float Longitude = AngleInc;

				int32 NumSegmentsX = Segments;
				while (NumSegmentsX--)
				{
					const float SinX = FMath::Sin(Longitude);
					const float CosX = FMath::Cos(Longitude);

					const FVector Vertex2 = FVector((CosX * SinY1), (SinX * SinY1), CosY1) * It->Value.ShapeSize.X + WLocation;
					const FVector Vertex4 = FVector((CosX * SinY2), (SinX * SinY2), CosY2) * It->Value.ShapeSize.X + WLocation;

					PDI->DrawLine(Vertex1, Vertex2, FLinearColor::Red, SDPG_Foreground);
					PDI->DrawLine(Vertex1, Vertex3, FLinearColor::Red, SDPG_Foreground);

					Vertex1 = Vertex2;
					Vertex3 = Vertex4;
					Longitude += AngleInc;
				}

				SinY1 = SinY2;
				CosY1 = CosY2;
				Latitude += AngleInc;
			}
		}
		else if (It->Value.ShapeType == EBXShapeType::ST_Capsule)
		{
			const FVector Origin = WTransform.GetLocation();
			const FMatrix Axes = FQuatRotationTranslationMatrix(WTransform.GetRotation(), FVector::ZeroVector);
			const FVector XAxis = Axes.GetScaledAxis(EAxis::X);
			const FVector YAxis = Axes.GetScaledAxis(EAxis::Y);
			const FVector ZAxis = Axes.GetScaledAxis(EAxis::Z);

			float Radius = It->Value.ShapeSize.X;
			float HalfHeight = It->Value.ShapeSize.Y;

			// 绘制原切面
			const float HalfAxis = FMath::Max<float>(HalfHeight - Radius, 1.f);
			const FVector TopEnd = Origin + HalfAxis * ZAxis;
			const FVector BottomEnd = Origin - HalfAxis * ZAxis;

			DrawCircle(PDI, TopEnd, XAxis, YAxis, Radius);
			DrawCircle(PDI, BottomEnd, XAxis, YAxis, Radius);

			// 绘制穹顶
			DrawHalfCircle(PDI, TopEnd, YAxis, ZAxis, Radius);
			DrawHalfCircle(PDI, TopEnd, XAxis, ZAxis, Radius);

			const FVector NegZAxis = -ZAxis;

			DrawHalfCircle(PDI, BottomEnd, YAxis, NegZAxis, Radius);
			DrawHalfCircle(PDI, BottomEnd, XAxis, NegZAxis, Radius);

			// 绘制竖线
			PDI->DrawLine(TopEnd + Radius * XAxis, BottomEnd + Radius * XAxis, FLinearColor::Red, SDPG_Foreground);
			PDI->DrawLine(TopEnd - Radius * XAxis, BottomEnd - Radius * XAxis, FLinearColor::Red, SDPG_Foreground);
			PDI->DrawLine(TopEnd + Radius * YAxis, BottomEnd + Radius * YAxis, FLinearColor::Red, SDPG_Foreground);
			PDI->DrawLine(TopEnd - Radius * YAxis, BottomEnd - Radius * YAxis, FLinearColor::Red, SDPG_Foreground);
		}
		else if (It->Value.ShapeType == EBXShapeType::ST_Box)
		{
			FVector Box = It->Value.ShapeSize;

			FVector Start = WTransform.TransformPosition(FVector(Box.X, Box.Y, Box.Z));
			FVector End = WTransform.TransformPosition(FVector(Box.X, -Box.Y, Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);

			Start = WTransform.TransformPosition(FVector(Box.X, -Box.Y, Box.Z));
			End = WTransform.TransformPosition(FVector(-Box.X, -Box.Y, Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);

			Start = WTransform.TransformPosition(FVector(-Box.X, -Box.Y, Box.Z));
			End = WTransform.TransformPosition(FVector(-Box.X, Box.Y, Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);

			Start = WTransform.TransformPosition(FVector(-Box.X, Box.Y, Box.Z));
			End = WTransform.TransformPosition(FVector(Box.X, Box.Y, Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);

			Start = WTransform.TransformPosition(FVector(Box.X, Box.Y, -Box.Z));
			End = WTransform.TransformPosition(FVector(Box.X, -Box.Y, -Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);

			Start = WTransform.TransformPosition(FVector(Box.X, -Box.Y, -Box.Z));
			End = WTransform.TransformPosition(FVector(-Box.X, -Box.Y, -Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);

			Start = WTransform.TransformPosition(FVector(-Box.X, -Box.Y, -Box.Z));
			End = WTransform.TransformPosition(FVector(-Box.X, Box.Y, -Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);

			Start = WTransform.TransformPosition(FVector(-Box.X, Box.Y, -Box.Z));
			End = WTransform.TransformPosition(FVector(Box.X, Box.Y, -Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);

			Start = WTransform.TransformPosition(FVector(Box.X, Box.Y, Box.Z));
			End = WTransform.TransformPosition(FVector(Box.X, Box.Y, -Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);

			Start = WTransform.TransformPosition(FVector(Box.X, -Box.Y, Box.Z));
			End = WTransform.TransformPosition(FVector(Box.X, -Box.Y, -Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);

			Start = WTransform.TransformPosition(FVector(-Box.X, -Box.Y, Box.Z));
			End = WTransform.TransformPosition(FVector(-Box.X, -Box.Y, -Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);

			Start = WTransform.TransformPosition(FVector(-Box.X, Box.Y, Box.Z));
			End = WTransform.TransformPosition(FVector(-Box.X, Box.Y, -Box.Z));
			PDI->DrawLine(Start, End, FLinearColor::Red, SDPG_Foreground);
		}
	}
}

void FBXShapeComponentVisualizer::DrawCircle(FPrimitiveDrawInterface* PDI, const FVector& Center, const FVector& X, const FVector& Y, const float Radius)
{
	int32 NumSides = 16;
	const float	AngleDelta = 2.0f * UE_PI / NumSides;
	FVector	LastVertex = Center + X * Radius;

	for (int32 SideIndex = 0; SideIndex < NumSides; SideIndex++)
	{
		const FVector Vertex = Center + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
		PDI->DrawLine(LastVertex, Vertex, FLinearColor::Red, SDPG_Foreground);
		LastVertex = Vertex;
	}
}

void FBXShapeComponentVisualizer::DrawHalfCircle(FPrimitiveDrawInterface* PDI, const FVector& Center, const FVector& X, const FVector& Y, const float Radius)
{
	int32 NumSides = 16;
	const float AngleDelta = 2.0f * UE_PI / NumSides;
	FVector	LastVertex = Center + X * Radius;

	for (int32 SideIndex = 0; SideIndex < (NumSides / 2); SideIndex++)
	{
		FVector	Vertex = Center + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
		PDI->DrawLine(LastVertex, Vertex, FLinearColor::Red, SDPG_Foreground);
		LastVertex = Vertex;
	}
}

#pragma endregion ComponentVisualizer
