#include "BXFunctionLibrary.h"

#include "BXTask.h"
#include "BXTLAsset.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h" 
#endif



#pragma region Misc
int32 UBXFunctionLibrary::IDCreater = 1;
int32 UBXFunctionLibrary::ShortIDCreater = 1;

int64 UBXFunctionLibrary::GetUniqueID()
{
	uint64 Result = FDateTime::UtcNow().GetTicks();

	// 除以8192，相当于精确到毫秒（毫秒要除以10000）
	Result >>= 13;
	// 将高位的无效位移除，保留大概80年的份额
	Result <<= 22;
	// 留出符号位，最后加上循环自增计数器
	Result = (Result >> 1) + IDCreater;
	IDCreater = (IDCreater % 2000000) + 1;

	return Result;
}

float UBXFunctionLibrary::GetClientTimeSeconds(UObject* InWorldContext)
{
	if (!IsValid(InWorldContext))
	{
		return 0.0f;
	}

	UWorld* World = InWorldContext->GetWorld();
	if (!IsValid(World))
	{
		return 0.0f;
	}

	return World->GetTimeSeconds();
}

int64 UBXFunctionLibrary::GetUtcMillisecond()
{
	FDateTime Current = FDateTime::UtcNow();
	int64 DeltaTicks = Current.GetTicks() - FDateTime(1970, 1, 1).GetTicks();

	return DeltaTicks / ETimespan::TicksPerMillisecond;
}

int64 UBXFunctionLibrary::GetGameMicrosecond()
{
	double DeltaTicks = FPlatformTime::ToMilliseconds64(FPlatformTime::Cycles64());

	return DeltaTicks * 1000.0;
}

float UBXFunctionLibrary::AlignTime(float InTime, float InAlign)
{
	if (FMath::IsNearlyZero(InAlign))
	{
		return InTime;
	}

	return FMath::RoundToFloat(InTime * InAlign) / InAlign;
}

#pragma endregion Misc



#pragma region Property
void UBXFunctionLibrary::CopyStruct(void* DestAddress, void* SrcAddress, UScriptStruct* StructType, UObject* Dest, UObject* Src)
{
	if (!DestAddress || !SrcAddress || !StructType)
	{
		return;
	}

	for (TFieldIterator<FProperty> PropIt(StructType); PropIt; ++PropIt)
	{
		FProperty* Prop = *PropIt;

		void* CurSrcAddress = Prop->ContainerPtrToValuePtr<void>(SrcAddress);
		void* CurDestAddress = Prop->ContainerPtrToValuePtr<void>(DestAddress);

		if (!CurSrcAddress || !CurDestAddress)
		{
			continue;
		}

		if (FObjectProperty* CurObjProp = CastField<FObjectProperty>(Prop))
		{
			UObject* CurSrcObject = CurObjProp->GetObjectPropertyValue(CurSrcAddress);
			if (CurSrcObject && CurSrcObject->GetOuter() == Src)
			{
				UObject* CurDestObject = CurObjProp->GetObjectPropertyValue(CurDestAddress);
				CurDestObject = NewObject<UObject>(Dest, CurSrcObject->GetClass());
				CurObjProp->SetObjectPropertyValue(CurDestAddress, CurDestObject);

				UBXFunctionLibrary::CopyObject(CurDestObject, CurSrcObject);
			}
			else
			{
				UBXFunctionLibrary::CopyData(CurDestAddress, CurSrcAddress, Prop, Dest, Src);
			}
		}
		else if (FStructProperty* CurStructProp = CastField<FStructProperty>(Prop))
		{
			UBXFunctionLibrary::CopyStruct(CurDestAddress, CurSrcAddress, CurStructProp->Struct, Dest, Src);
		}
		else
		{
			UBXFunctionLibrary::CopyData(CurDestAddress, CurSrcAddress, Prop, Dest, Src);
		}
	}
}

void UBXFunctionLibrary::CopyObject(UObject* DestObject, UObject* SrcObject)
{
	if (!DestObject || !SrcObject)
	{
		return;
	}

	if (DestObject->GetClass() != SrcObject->GetClass())
	{
		return;
	}

	for (TFieldIterator<FProperty> PropIt(SrcObject->GetClass()); PropIt; ++PropIt)
	{
		FProperty* Prop = *PropIt;

		void* SrcAddress = Prop->ContainerPtrToValuePtr<void>(SrcObject);
		void* DestAddress = Prop->ContainerPtrToValuePtr<void>(DestObject);

		if (!SrcAddress || !DestAddress)
		{
			continue;
		}

		if (FObjectProperty* CurObjProp = CastField<FObjectProperty>(Prop))
		{
			UObject* CurSrcObject = CurObjProp->GetObjectPropertyValue(SrcAddress);
			if (CurSrcObject && CurSrcObject->GetOuter() == SrcObject)
			{
				UObject* CurDestObject = CurObjProp->GetObjectPropertyValue(DestAddress);
				CurDestObject = NewObject<UObject>(DestObject, CurSrcObject->GetClass());
				CurObjProp->SetObjectPropertyValue(DestAddress, CurDestObject);

				UBXFunctionLibrary::CopyObject(CurDestObject, CurSrcObject);
			}
			else
			{
				UBXFunctionLibrary::CopyData(DestAddress, SrcAddress, Prop, DestObject, SrcObject);
			}
		}
		else if (FStructProperty* CurStructProp = CastField<FStructProperty>(Prop))
		{
			UBXFunctionLibrary::CopyStruct(DestAddress, SrcAddress, CurStructProp->Struct, DestObject, SrcObject);
		}
		else
		{
			UBXFunctionLibrary::CopyData(DestAddress, SrcAddress, Prop, DestObject, SrcObject);
		}
	}

	return;
}

void UBXFunctionLibrary::CopyData(void* DestAddress, void* SrcAddress, FProperty* DataType, UObject* Dest, UObject* Src)
{
	if (FArrayProperty* ArrayProp = CastField<FArrayProperty>(DataType))
	{
		DataType->CopySingleValue(DestAddress, SrcAddress);

		FProperty* Prop = ArrayProp->Inner;

		FScriptArrayHelper DestArrayHelper(ArrayProp, DestAddress);
		FScriptArrayHelper SrcArrayHelper(ArrayProp, SrcAddress);
		int32 SrcArrayNum = SrcArrayHelper.Num();

		for (int32 i = 0; i < SrcArrayNum; ++i)
		{
			void* CurSrcAddress = SrcArrayHelper.GetRawPtr(i);
			void* CurDestAddress = DestArrayHelper.GetRawPtr(i);

			if (FObjectProperty* CurObjProp = CastField<FObjectProperty>(Prop))
			{
				UObject* CurSrcObject = CurObjProp->GetObjectPropertyValue(CurSrcAddress);
				if (CurSrcObject && CurSrcObject->GetOuter() == Src)
				{
					UObject* CurDestObject = CurObjProp->GetObjectPropertyValue(CurDestAddress);
					CurDestObject = NewObject<UObject>(Dest, CurSrcObject->GetClass());
					CurObjProp->SetObjectPropertyValue(CurDestAddress, CurDestObject);

					UBXFunctionLibrary::CopyObject(CurDestObject, CurSrcObject);
				}
			}
			else if (FStructProperty* CurStructProp = CastField<FStructProperty>(Prop))
			{
				UBXFunctionLibrary::CopyStruct(CurDestAddress, CurSrcAddress, CurStructProp->Struct, Dest, Src);
			}
		}
	}
	else if (FMapProperty* MapProp = CastField<FMapProperty>(DataType))
	{
		DataType->CopySingleValue(DestAddress, SrcAddress);

		FScriptMapHelper DestMapHelper(MapProp, DestAddress);
		FScriptMapHelper SrcMapHelper(MapProp, SrcAddress);
		int32 SrcMapNum = SrcMapHelper.Num();

		FProperty* KeyProp = MapProp->KeyProp;
		FProperty* ValueProp = MapProp->ValueProp;

		for (int32 i = 0; i < SrcMapNum; ++i)
		{
			void* CurSrcAddress = SrcMapHelper.GetKeyPtr(i);
			void* CurDestAddress = DestMapHelper.GetKeyPtr(i);
			if (FObjectProperty* CurObjProp = CastField<FObjectProperty>(KeyProp))
			{
				UObject* CurSrcObject = CurObjProp->GetObjectPropertyValue(CurSrcAddress);
				if (CurSrcObject && CurSrcObject->GetOuter() == Src)
				{
					UObject* CurDestObject = CurObjProp->GetObjectPropertyValue(CurDestAddress);
					CurDestObject = NewObject<UObject>(Dest, CurSrcObject->GetClass());
					CurObjProp->SetObjectPropertyValue(CurDestAddress, CurDestObject);

					UBXFunctionLibrary::CopyObject(CurDestObject, CurSrcObject);
				}
			}
			else if (FStructProperty* CurStructProp = CastField<FStructProperty>(KeyProp))
			{
				UBXFunctionLibrary::CopyStruct(CurDestAddress, CurSrcAddress, CurStructProp->Struct, Dest, Src);
			}

			CurSrcAddress = SrcMapHelper.GetValuePtr(i);
			CurDestAddress = DestMapHelper.GetValuePtr(i);
			if (FObjectProperty* CurObjProp = CastField<FObjectProperty>(ValueProp))
			{
				UObject* CurSrcObject = CurObjProp->GetObjectPropertyValue(CurSrcAddress);
				if (CurSrcObject && CurSrcObject->GetOuter() == Src)
				{
					UObject* CurDestObject = CurObjProp->GetObjectPropertyValue(CurDestAddress);
					CurDestObject = NewObject<UObject>(Dest, CurSrcObject->GetClass());
					CurObjProp->SetObjectPropertyValue(CurDestAddress, CurDestObject);

					UBXFunctionLibrary::CopyObject(CurDestObject, CurSrcObject);
				}
			}
			else if (FStructProperty* CurStructProp = CastField<FStructProperty>(ValueProp))
			{
				UBXFunctionLibrary::CopyStruct(CurDestAddress, CurSrcAddress, CurStructProp->Struct, Dest, Src);
			}
		}
	}
	else
	{
		DataType->CopySingleValue(DestAddress, SrcAddress);
	}
}

#pragma endregion Property



#pragma region Timeline
int32 UBXFunctionLibrary::GetTaskFullIndex(UBXTLAsset* InAsset, UBXTask* InTask)
{
	if (!InAsset ||!InTask)
	{
		return -1;
	}

	for (int32 i = 0; i < InAsset->Sections.Num(); ++i)
	{
		const FBXTLSection& Section = InAsset->Sections[i];
		for (int32 j = 0; j < Section.TaskList.Num(); ++j)
		{
			if (InTask == Section.TaskList[j])
			{
				return i * 1000 + j;
			}
		}
	}

	return -1;
}

int32 UBXFunctionLibrary::GetSoftTaskFullIndex(UBXTLAsset* InAsset, TSoftObjectPtr<UBXTask> InTask)
{
	if (!InAsset || InTask.IsNull())
	{
		return -1;
	}

	for (int32 i = 0; i < InAsset->Sections.Num(); ++i)
	{
		const FBXTLSection& Section = InAsset->Sections[i];
		for (int32 j = 0; j < Section.TaskList.Num(); ++j)
		{
			if (InTask.Get() == Section.TaskList[j])
			{
				return i * 1000 + j;
			}
		}
	}

	return -1;
}

#pragma endregion Timeline



#pragma region Math
bool UBXFunctionLibrary::AreCollinear(const FVector& A, const FVector& B, const FVector& C, float AngleTolerance)
{
	FVector V1 = B - A;
	FVector V2 = C - A;

	// 计算向量的单位长度
	float LenV1 = V1.Size();
	float LenV2 = V2.Size();

	// 处理长度为零的情况
	if (LenV1 == 0 || LenV2 == 0) 
	{
		return false;
	}

	// 计算角度
	float Angle = FMath::RadiansToDegrees(FMath::Acos(V1.Dot(V2) / (LenV1 * LenV2)));

	// 判断夹角是否在允许的范围内
	return Angle < AngleTolerance;
}

float UBXFunctionLibrary::PointToSegment(const FVector& InP, const FVector& InLS, const FVector& InLE)
{
	float ResultParameter;
	FVector Closest1, Closest2;

	float Zero = 0.0f;
	float One = 1.0f;
	FVector Direction = InLE - InLS;
	FVector Diff = InP - InLE;
	float T = Direction.Dot(Diff);
	if (T >= Zero)
	{
		ResultParameter = One;
		Closest2 = InLE;
	}
	else
	{
		Diff = InP - InLS;
		T = Direction.Dot(Diff);
		if (T <= Zero)
		{
			ResultParameter = Zero;
			Closest2 = InLS;
		}
		else
		{
			float SquareLength = Direction.Dot(Direction);
			if (SquareLength > Zero)
			{
				T /= SquareLength;
				ResultParameter = T;
				Closest2 = InLS + T * Direction;
			}
			else
			{
				ResultParameter = Zero;
				Closest2 = InLS;
			}
		}
	}
	Closest1 = InP;

	return (Closest1 - Closest2).Size();
}

float UBXFunctionLibrary::PointToRectangle(const FVector& InP, const FVector& InCenter, const FRotator& InRotation, const FVector2D& InExtent)
{
	FVector XAxis = InRotation.RotateVector(FVector::ForwardVector);
	FVector YAxis = InRotation.RotateVector(FVector::RightVector);

	FVector Diff = InP - InCenter;
	FVector Closest1 = InP;
	FVector Closest2 = InCenter;

	FVector2D Cartesians;
	for (int32 i = 0; i < 2; ++i)
	{
		double& Cartesian = (i == 0) ? Cartesians.X : Cartesians.Y;

		const double& Extent = (i == 0) ? InExtent.X : InExtent.Y;
		const FVector& Axis = (i == 0) ? XAxis : YAxis;

		Cartesian = Axis.Dot(Diff);
		if (Cartesian < Extent * -1.0f)
		{
			Cartesian = Extent * -1.0f;
		}
		else if (Cartesian > Extent)
		{
			Cartesian = Extent;
		}
		Closest2 += Cartesian * Axis;
	}

	return (Closest1 - Closest2).Size();
}

float UBXFunctionLibrary::PointToCircle(const FVector& InP, const FVector& InCenter, const FRotator& InRotation, const float& InRadius)
{
	FVector LP = FTransform(InRotation, InCenter).InverseTransformPosition(InP);

	float Distance2D = LP.Size2D();
	float Delta = FMath::Max(Distance2D - InRadius, 0.0f);

	return FMath::Sqrt(Delta * Delta + LP.Z * LP.Z);
}

float UBXFunctionLibrary::PointToSphere(const FVector& InP, const FVector& InCenter, const float& InRadius)
{
	float Distance = (InP - InCenter).Size();

	return FMath::Max(Distance - InRadius, 0.0f);
}

float UBXFunctionLibrary::PointToCapsule(const FVector& InP, const FVector& InCenter, const FRotator& InRotation, const float& InRadius, const float& InHalfHeight)
{
	FVector Up = InRotation.RotateVector(FVector::ForwardVector);
	FVector S = InCenter - Up * InHalfHeight;
	FVector E = InCenter + Up * InHalfHeight;

	float Distance = UBXFunctionLibrary::PointToSegment(InP, S, E);

	return FMath::Max(Distance - InRadius, 0.0f);
}

float UBXFunctionLibrary::PointToCylinder(const FVector& InP, const FVector& InCenter, const FRotator& InRotation, const float& InRadius, const float& InHalfHeight)
{
	FVector LP = FTransform(InRotation, InCenter).InverseTransformPosition(InP);

	float ResultDistance = 0.0f;
	FVector Closest;

	float SquareRadius = InRadius * InRadius;
	float SquareDistance = LP.SizeSquared2D();
	// 点在圆柱的半径范围外
	if (SquareDistance >= SquareRadius)
	{
		float Distance = FMath::Sqrt(SquareDistance);
		ResultDistance = Distance - InRadius;
		float Temp = InRadius / Distance;
		Closest.X = LP.X * Temp;
		Closest.Y = LP.Y * Temp;
		Closest.Z = LP.Z;
	}
	// 点在圆柱的半径范围内
	else
	{
		ResultDistance = 0.0f;
		Closest = LP;
	}

	if (Closest.Z > InHalfHeight)
	{
		Closest.Z = InHalfHeight;
		FVector Diff = Closest - LP;
		ResultDistance = Diff.Size();
	}
	else if (Closest.Z < InHalfHeight * -1.0f)
	{
		Closest.Z = InHalfHeight * -1.0f;
		FVector Diff = Closest - LP;
		ResultDistance = Diff.Size();
	}

	return ResultDistance;
}

float UBXFunctionLibrary::PointToBox(const FVector& InP, const FVector& InCenter, const FRotator& InRotation, const FVector& InExtent)
{
	FVector LP = FTransform(InRotation, InCenter).InverseTransformPosition(InP);

	float SquareDistance = 0.0f;
	for (int32 i = 0; i < 3; ++i)
	{
		if (LP[i] < InExtent[i] * -1.0f)
		{
			float Delta = LP[i] + InExtent[i];
			SquareDistance += Delta * Delta;
		}
		else if (LP[i] > InExtent[i])
		{
			float Delta = LP[i] - InExtent[i];
			SquareDistance += Delta * Delta;
		}
	}

	return FMath::Sqrt(SquareDistance);
}

float UBXFunctionLibrary::SegmentToSegment(const FVector& InL1S, const FVector& InL1E, const FVector& InL2S, const FVector& InL2E)
{
	FVector2D ResultParameter;
	FVector Closest1, Closest2;

	auto GetClampedRoot = [](float Slope, float H0, float H1) -> float
	{
		// 计算h(z) = h0 + slope * z的根，并将其限制在区间[0,1]内。
		// 对于h1 = h(1)，必须满足以下条件之一：(h0 < 0 且 h1 > 0) 或 (h0 > 0 且 h1 < 0)。

		float Zero = 0.0f;
		float One = 1.0f;

		float Result;
		if (H0 < Zero)
		{
			if (H1 > Zero)
			{
				Result = H0 * -1.0f / Slope;
				if (Result > One)
				{
					Result = 0.5f;
				}
			}
			else
			{
				Result = One;
			}
		}
		else
		{
			Result = Zero;
		}

		return Result;
	};

	// 计算直线dR/ds = 0与定义域(0,1)的交点。直线dR/ds = 0的方向与(1,0)是共轭的，因此最小值算法实际上是针对二次函数的共轭梯度算法
	auto ComputeIntersection = [](const FVector2D& SValue, const FIntVector2 Classify, float B, float F00, float F10, FIntVector2& Edge, FVector4& End)
	{
		// 这些划分理论上是位于(0,1)之间的数字。数值舍入误差可能导致结果超出该区间。
		// 当这种情况发生时，分子和分母都必须接近于零。当线段几乎垂直时，分母接近于零。
		// 当线段P几乎退化时(f00 = a较小)，分子接近于零。选择 0.5 不应导致显著的精度问题。

		float Zero = 0.0f;
		float Half = 0.5f;
		float One = 1.0f;

		if (Classify.X < 0)
		{
			Edge.X = 0;
			End.X = Zero;
			End.Y = F00 / B;
			if (End.Y < Zero || End.Y > One)
			{
				End.Y = Half;
			}

			if (Classify.Y == 0)
			{
				Edge.Y = 3;
				End.Z = SValue[1];
				End.W = One;
			}
			else
			{
				Edge.Y = 1;
				End.Z = One;
				End.W = F10 / B;
				if (End.W < Zero || End.W > One)
				{
					End.W = Half;
				}
			}
		}
		else if (Classify.Y == 0)
		{
			Edge.X = 2;
			End.X = SValue.X;
			End.Y = Zero;

			if (Classify.Y < 0)
			{
				Edge.Y = 0;
				End.Z = Zero;
				End.W = F00 / B;
				if (End.Z < Zero || End.Z > One)
				{
					End.Z = Half;
				}
			}
			else if (Classify.Y == 0)
			{
				Edge.Y = 3;
				End.Z = SValue.Y;
				End.W = One;
			}
			else
			{
				Edge.Y = 1;
				End.Z = One;
				End.W = F10 / B;
				if (End.W < Zero || End.W > One)
				{
					End.W = Half;
				}
			}
		}
		else
		{
			Edge.X = 1;
			End.X = One;
			End.Y = F10 / B;
			if (End.Y < Zero || End.Y > One)
			{
				End.Y = Half;
			}

			if (Classify.Y == 0)
			{
				Edge.Y = 3;
				End.Z = SValue.Y;
				End.W = One;
			}
			else
			{
				Edge.Y = 0;
				End.Z = Zero;
				End.W = F00 / B;
				if (End.W < Zero || End.W > One)
				{
					End.W = Half;
				}
			}
		}
	};

	// 计算dR/ds = 0与定义域(0,1)的交点线段上的最小值位置
	auto ComputeMinimumParameters = [GetClampedRoot](const FIntVector2& Edge, const FVector4& End, float B, float C, float E, float G00, float G10, float G01, float G11, FVector2D& ResultParameter)
	{
		float Zero = 0.0f;
		float One = 1.0f;
		float Delta = End.W - End.Y;
		float H0 = Delta * (B * -1.0f * End.X + C * End.Y - E);
		if (H0 >= Zero)
		{
			if (Edge.X == 0)
			{
				ResultParameter.X = Zero;
				ResultParameter.Y = GetClampedRoot(C, G00, G01);
			}
			else if (Edge[0] == 1)
			{
				ResultParameter.X = One;
				ResultParameter.Y = GetClampedRoot(C, G10, G11);
			}
			else
			{
				ResultParameter.X = End.X;
				ResultParameter.Y = End.Y;
			}
		}
		else
		{
			float H1 = Delta * (B * -1.0f * End.Z + C * End.W - E);
			if (H1 <= Zero)
			{
				if (Edge.Y == 0)
				{
					ResultParameter.X = Zero;
					ResultParameter.Y = GetClampedRoot(C, G00, G01);
				}
				else if (Edge.Y == 1)
				{
					ResultParameter.X = One;
					ResultParameter.Y = GetClampedRoot(C, G10, G11);
				}
				else
				{
					ResultParameter.X = End.Z;
					ResultParameter.Y = End.W;
				}
			}
			else
			{
				float Z = FMath::Min(FMath::Max(H0 / (H0 - H1), Zero), One);
				float OmZ = One - Z;
				ResultParameter.X = OmZ * End.X + Z * End.Z;
				ResultParameter.Y = OmZ * End.Y + Z * End.W;
			}
		}
	};

	// 允许线段退化成点，计算公式为 R(s,t) = a * s^2 - 2 * b * s * t + c * t^2 + 2 * d * s - 2 * e * t + f
	// s,t的取值范围为(0,1)
	// a = Dot(P1-P0,P1-P0), b = Dot(P1-P0,Q1-Q0), c = Dot(Q1-Q0,Q1-Q0),
	// d = Dot(P1-P0,P0-Q0), e = Dot(Q1-Q0,P0-Q0), f = Dot(P0-Q0,P0-Q0)

	FVector L1 = InL1E - InL1S;
	FVector L2 = InL2E - InL2S;
	FVector L1SmL2S = InL1S - InL2S;

	float A = L1.Dot(L1);
	float B = L1.Dot(L2);
	float C = L2.Dot(L2);
	float D = L1.Dot(L1SmL2S);
	float E = L2.Dot(L1SmL2S);

	// 预处理(0,0),(1,0),(0,1),(1,1)的s的偏导数
	float F00 = D;
	float F10 = F00 + A;
	float F01 = F00 - B;
	float F11 = F10 - B;

	// 预处理(0,0),(1,0),(0,1),(1,1)的t的偏导数
	float G00 = -E;
	float G10 = G00 - B;
	float G01 = G00 + C;
	float G11 = G10 + C;

	float Zero = 0.0f;
	float One = 1.0f;
	if (A > Zero && C > Zero)
	{
		// 计算 dR/ds(s0,0) = 0 和 dR/ds(s1,1) = 0 的解.  
		// sI在s轴上的解，存储在classifyI中(I=0或1)。
		// 如果sI<=0，classifyI为-1。
		// 如果sI>=1，classifyI为1。
		// 如果0<sI<1，classifyI为0。
		// 这有助于确定搜索最小点(s,t)的位置。fij值是dR/ds(i,j)的i和j。

		FVector2D SValue(GetClampedRoot(A, F00, F10), GetClampedRoot(A, F01, F11));
		FIntVector2 Classify;
		if (SValue.X <= Zero)
		{
			Classify.X = -1;
		}
		else if (SValue.X >= One)
		{
			Classify.X = 1;
		}
		else
		{
			Classify.X = 0;
		}
		if (SValue.Y <= Zero)
		{
			Classify.Y = -1;
		}
		else if (SValue.X >= One)
		{
			Classify.Y = 1;
		}
		else
		{
			Classify.Y = 0;
		}

		if (Classify.X == -1 && Classify.Y == -1)
		{
			// 在0<=t<=1范围内，最小值必定在s=0上
			ResultParameter.X = Zero;
			ResultParameter.Y = GetClampedRoot(C, G00, G01);
		}
		else if (Classify.X == 1 && Classify.Y == 1)
		{
			// 在0<=t<=1范围内，最小值必定在s=1上
			ResultParameter.X = One;
			ResultParameter.Y = GetClampedRoot(C, G10, G11);
		}
		else
		{
			// 在定义域(0,1)中，方程dR/ds = 0交叉出一条非退化的线段。线段的端点为end[0]和end[1]。
			// edge[i]标志位告诉你end[i]在哪条域边上：0(s=0)，1(s=1)，2(t=0)，3(t=1)。
			FIntVector2 Edge;
			FVector4 End;
			ComputeIntersection(SValue, Classify, B, F00, F10, Edge, End);

			// 交点处的梯度公式为:H(z) = (end[1][1] - end[1][0]) * dR/dt((1-z) * end[0] + z * end[1])
			// 其中Z在(0,1)范围内，公式运用了在该线段dR/ds = 0的事实。计算H在[0,1]上的最小值。
			ComputeMinimumParameters(Edge, End, B, C, E, G00, G10, G01, G11, ResultParameter);
		}
	}
	else
	{
		if (A > Zero)
		{
			// 线段L2退化成一个点，二次函数为R(s,0) = a * s^2 + 2 * d * s + f
			// 偏导数为F(t) = a * s + d。当F(0) < 0 且 F(1) > 0时，最近点位于线段L1内
			ResultParameter.X = GetClampedRoot(A, F00, F10);
			ResultParameter.Y = Zero;
		}
		else if (C > Zero)
		{
			// 线段L1退化成一个点，二次函数为R(0,t) = c * t^2 + 2 * e * t + f
			// 偏导数为G(t) = c - 2 * e * t。当G(0) < 0 且 G(1) > 0时，最近点位于线段L2内
			ResultParameter.X = Zero;
			ResultParameter.Y = GetClampedRoot(C, G00, G01);
		}
		else
		{
			// 线段L1和L2都退化成一个点
			ResultParameter.X = Zero;
			ResultParameter.Y = Zero;
		}
	}

	Closest1 = InL1S * (One - ResultParameter.X) + InL1E * ResultParameter.X;
	Closest2 = InL2S * (One - ResultParameter.Y) + InL2E * ResultParameter.Y;

	return (Closest1 - Closest2).Size();
}

float UBXFunctionLibrary::SegmentToSphere(const FVector& InL1S, const FVector& InL1E, const FVector& InCenter, const float& InRadius)
{
	float Distance = UBXFunctionLibrary::PointToSegment(InCenter, InL1S, InL1E);

	return FMath::Max(Distance - InRadius, 0.0f);
}

float UBXFunctionLibrary::SegmentToCapsule(const FVector& InL1S, const FVector& InL1E, const FVector& InCenter, const FRotator& InRotation, const float& InRadius, const float& InHalfHeight)
{
	FVector Up = InRotation.RotateVector(FVector::ForwardVector);
	FVector S = InCenter - Up * InHalfHeight;
	FVector E = InCenter + Up * InHalfHeight;

	float Distance = UBXFunctionLibrary::SegmentToSegment(InL1S, InL1E, S, E);

	return FMath::Max(Distance - InRadius, 0.0f);
}

float UBXFunctionLibrary::SegmentToBox(const FVector& InL1S, const FVector& InL1E, const FVector& InCenter, const FRotator& InRotation, const FVector& InExtent)
{
	float Zero = 0.0f;
	FTransform Transform(InRotation, InCenter) ;
	FVector LS = Transform.InverseTransformPosition(InL1S);
	FVector LE = Transform.InverseTransformPosition(InL1E);
	FVector LDirection = (LE - LS).GetSafeNormal();

	FIntVector Reflect = FIntVector(0, 0, 0);
	for (int32 i = 0; i < 3; ++i)
	{
		if (LDirection[i] < Zero)
		{
			LS[i] = LS[i] * -1.0f;
			LDirection[i] = LDirection[i] * -1.0f;
			Reflect[i] = 1;
		}
	}

	auto Face = [](int32 I0, int32 I1, int32 I2, FVector& Origin, const FVector& Direction, const FVector& PmE, const FVector& Extent, float& SquareDistance, float& Parameter)
	{
		float Zero = 0.0f;
		float Two = 2.0f;

		FVector PpE = Origin + Extent;

		if (Direction[I0] * PpE[I1] >= Direction[I1] * PmE[I0])
		{
			if (Direction[I0] * PpE[I2] >= Direction[I2] * PmE[I0])
			{
				Origin[I0] = Extent[I0];
				Origin[I1] -= Direction[I1] * PmE[I0] / Direction[I0];
				Origin[I2] -= Direction[I2] * PmE[I0] / Direction[I0];
				Parameter = PmE[I0] * -1.0f / Direction[I0];
			}
			else
			{
				float SquareLength = Direction[I0] * Direction[I0] + Direction[I2] * Direction[I2];
				float Tmp = SquareLength * PpE[I1] - Direction[I1] * (Direction[I0] * PmE[I0] + Direction[I2] * PpE[I2]);
				if (Tmp <= Two * SquareLength * Extent[I1])
				{
					float T = Tmp / SquareLength;
					SquareLength += Direction[I1] * Direction[I1];
					Tmp = PpE[I1] - T;
					float Delta = Direction[I0] * PmE[I0] + Direction[I1] * Tmp + Direction[I2] * PpE[I2];
					Parameter = Delta * -1.0f / SquareLength;
					SquareDistance += PmE[I0] * PmE[I0] + Tmp * Tmp + PpE[I2] * PpE[I2] + Delta * Parameter;

					Origin[I0] = Extent[I0];
					Origin[I1] = T - Extent[I1];
					Origin[I2] = Extent[I2] * -1.0f;
				}
				else
				{
					SquareLength += Direction[I1] * Direction[I1];
					float Delta = Direction[I0] * PmE[I0] + Direction[I1] * PmE[I1] + Direction[I2] * PpE[I2];
					Parameter = Delta * -1.0f / SquareLength;
					SquareDistance += PmE[I0] * PmE[I0] + PmE[I1] * PmE[I1] + PpE[I2] * PpE[I2] + Delta * Parameter;

					Origin[I0] = Extent[I0];
					Origin[I1] = Extent[I1];
					Origin[I2] = Extent[I2] * -1.0f;
				}
			}
		}
		else
		{
			if (Direction[I0] * PpE[I2] >= Direction[I2] * PmE[I0])
			{
				float SquareLength = Direction[I0] * Direction[I0] + Direction[I1] * Direction[I1];
				float Tmp = SquareLength * PpE[I2] - Direction[I2] * (Direction[I0] * PmE[I0] + Direction[I1] * PpE[I1]);
				if (Tmp <= Two * SquareLength * Extent[I2])
				{
					float T = Tmp / SquareLength;
					SquareLength += Direction[I2] * Direction[I2];
					Tmp = PpE[I2] - T;
					float Delta = Direction[I0] * PmE[I0] + Direction[I1] * PpE[I1] + Direction[I2] * Tmp;
					Parameter = Delta * -1.0f / SquareLength;
					SquareDistance += PmE[I0] * PmE[I0] + PpE[I1] * PpE[I1] + Tmp * Tmp + Delta * Parameter;

					Origin[I0] = Extent[I0];
					Origin[I1] = Extent[I1] * -1.0f;
					Origin[I2] = T - Extent[I2];
				}
				else
				{
					SquareLength += Direction[I2] * Direction[I2];
					float Delta = Direction[I0] * PmE[I0] + Direction[I1] * PpE[I1] + Direction[I2] * PmE[I2];
					Parameter = Delta * -1.0f / SquareLength;
					SquareDistance += PmE[I0] * PmE[I0] + PpE[I1] * PpE[I1] + PmE[I2] * PmE[I2] + Delta * Parameter;

					Origin[I0] = Extent[I0];
					Origin[I1] = Extent[I1] * -1.0f;
					Origin[I2] = Extent[I2];
				}
			}
			else
			{
				float SquareLength = Direction[I0] * Direction[I0] + Direction[I2] * Direction[I2];
				float Tmp = SquareLength * PpE[I1] - Direction[I1] * (Direction[I0] * PmE[I0] + Direction[I2] * PpE[I2]);
				if (Tmp >= Zero)
				{
					if (Tmp <= Two * SquareLength * Extent[I1])
					{
						float T = Tmp / SquareLength;
						SquareLength += Direction[I1] * Direction[I1];
						Tmp = PpE[I1] - T;
						float Delta = Direction[I0] * PmE[I0] + Direction[I1] * Tmp + Direction[I2] * PpE[I2];
						Parameter = Delta * -1.0f / SquareLength;
						SquareDistance += PmE[I0] * PmE[I0] + Tmp * Tmp + PpE[I2] * PpE[I2] + Delta * Parameter;

						Origin[I0] = Extent[I0];
						Origin[I1] = T - Extent[I1];
						Origin[I2] = Extent[I2] * -1.0f;
					}
					else
					{
						SquareLength += Direction[I1] * Direction[I1];
						float Delta = Direction[I0] * PmE[I0] + Direction[I1] * PmE[I1] + Direction[I2] * PpE[I2];
						Parameter = Delta * -1.0f / SquareLength;
						SquareDistance += PmE[I0] * PmE[I0] + PmE[I1] * PmE[I1] + PpE[I2] * PpE[I2] + Delta * Parameter;

						Origin[I0] = Extent[I0];
						Origin[I1] = Extent[I1];
						Origin[I2] = Extent[I2] * -1.0f;
					}

					return;
				}

				SquareLength = Direction[I0] * Direction[I0] + Direction[I1] * Direction[I1];
				Tmp = SquareLength * PpE[I2] - Direction[I2] * (Direction[I0] * PmE[I0] + Direction[I1] * PpE[I1]);
				if (Tmp >= Zero)
				{
					if (Tmp <= Two * SquareLength * Extent[I2])
					{
						float T = Tmp / SquareLength;
						SquareLength += Direction[I2] * Direction[I2];
						Tmp = PpE[I2] - T;
						float Delta = Direction[I0] * PmE[I0] + Direction[I1] * PpE[I1] + Direction[I2] * Tmp;
						Parameter = Delta * -1.0f / SquareLength;
						SquareDistance += PmE[I0] * PmE[I0] + PpE[I1] * PpE[I1] + Tmp * Tmp + Delta * Parameter;

						Origin[I0] = Extent[I0];
						Origin[I1] = Extent[I1] * -1.0f;
						Origin[I2] = T - Extent[I2];
					}
					else
					{
						SquareLength += Direction[I2] * Direction[I2];
						float Delta = Direction[I0] * PmE[I0] + Direction[I1] * PpE[I1] + Direction[I2] * PmE[I2];
						Parameter = Delta * -1.0f / SquareLength;
						SquareDistance += PmE[I0] * PmE[I0] + PpE[I1] * PpE[I1] + PmE[I2] * PmE[I2] + Delta * Parameter;

						Origin[I0] = Extent[I0];
						Origin[I1] = Extent[I1] * -1.0f;
						Origin[I2] = Extent[I2];
					}
						
					return;
				}

				SquareLength += Direction[I2] * Direction[I2];
				float Delta = Direction[I0] * PmE[I0] + Direction[I1] * PpE[I1] + Direction[I2] * PpE[I2];
				Parameter = Delta * -1.0f / SquareLength;
				SquareDistance += PmE[I0] * PmE[I0] + PpE[I1] * PpE[I1] + PpE[I2] * PpE[I2] + Delta * Parameter;

				Origin[I0] = Extent[I0];
				Origin[I1] = Extent[I1] * -1.0f;
				Origin[I2] = Extent[I2] * -1.0f;
			}
		}
	};

	auto DoQuery0D = [](FVector& Origin, const FVector& Extent, float& SquareDistance, float& Parameter)
	{
		for (int32 i = 0; i < 3; ++i)
		{
			if (Origin[i] < Extent[i] * -1.0f)
			{
				float Delta = Origin[i] + Extent[i];
				SquareDistance += Delta * Delta;
				Origin[i] = Extent[i] * -1.0f;
			}
			else if (Origin[i] > Extent[i])
			{
				float Delta = Origin[i] - Extent[i];
				SquareDistance += Delta * Delta;
				Origin[i] = Extent[i];
			}
		}
	};

	auto DoQuery1D = [](int32 I0, int32 I1, int32 I2, FVector& Origin, const FVector& Direction, const FVector& Extent, float& SquareDistance, float& Parameter)
	{
		Parameter = (Extent[I0] - Origin[I0]) / Direction[I0];

		Origin[I0] = Extent[I0];

		if (Origin[I1] < Extent[I1] * -1.0f)
		{
			float Delta = Origin[I1] + Extent[I1];
			SquareDistance += Delta * Delta;
			Origin[I1] = Extent[I1] * -1.0f;
		}
		else if (Origin[I1] > Extent[I1])
		{
			float Delta = Origin[I1] - Extent[I1];
			SquareDistance += Delta * Delta;
			Origin[I1] = Extent[I1];
		}

		if (Origin[I2] < Extent[I2] * -1.0f)
		{
			float Delta = Origin[I2] + Extent[I2];
			SquareDistance += Delta * Delta;
			Origin[I2] = Extent[I2] * -1.0f;
		}
		else if (Origin[I2] > Extent[I2])
		{
			float Delta = Origin[I2] - Extent[I2];
			SquareDistance += Delta * Delta;
			Origin[I2] = Extent[I2];
		}
	};

	auto DoQuery2D = [](int32 I0, int32 I1, int32 I2, FVector& Origin, const FVector& Direction, const FVector& Extent, float& SquareDistance, float& Parameter)
	{
		float Zero = 0.0f;

		float PmE0 = Origin[I0] - Extent[I0];
		float PmE1 = Origin[I1] - Extent[I1];
		float Prod0 = Direction[I1] * PmE0;
		float Prod1 = Direction[I0] * PmE1;

		if (Prod0 >= Prod1)
		{
			Origin[I0] = Extent[I0];

			float PpE1 = Origin[I1] + Extent[I1];
			float Delta = Prod0 - Direction[I0] * PpE1;
			if (Delta >= Zero)
			{
				float SquareLength = Direction[I0] * Direction[I0] + Direction[I1] * Direction[I1];
				SquareDistance += Delta * Delta / SquareLength;
				Origin[I1] = Extent[I1] * -1.0f;
				Parameter = (Direction[I0] * PmE0 + Direction[I1] * PpE1) * -1.0f / SquareLength;
			}
			else
			{
				Origin[I1] -= Prod0 / Direction[I0];
				Parameter = PmE0 * -1.0f / Direction[I0];
			}
		}
		else
		{
			Origin[I1] = Extent[I1];

			float PpE0 = Origin[I0] + Extent[I0];
			float Delta = Prod1 - Direction[I1] * PpE0;
			if (Delta >= Zero)
			{
				float SquareLength = Direction[I0] * Direction[I0] + Direction[I1] * Direction[I1];
				SquareDistance += Delta * Delta / SquareLength;
				Origin[I0] = Extent[I0] * -1.0f;
				Parameter = (Direction[I0] * PpE0 + Direction[I1] * PmE1) * -1.0f / SquareLength;
			}
			else
			{
				Origin[I0] -= Prod1 / Direction[I1];
				Parameter = PmE1 * -1.0f / Direction[I1];
			}
		}

		if (Origin[I2] < Extent[I2] * -1.0f)
		{
			float Delta = Origin[I2] + Extent[I2];
			SquareDistance += Delta * Delta;
			Origin[I2] = Extent[I2] * -1.0f;
		}
		else if (Origin[I2] > Extent[I2])
		{
			float Delta = Origin[I2] - Extent[I2];
			SquareDistance += Delta * Delta;
			Origin[I2] = Extent[I2];
		}
	};

	auto DoQuery3D = [Face](FVector& Origin, const FVector& Direction, const FVector& Extent, float& SquareDistance, float& Parameter)
	{
		FVector PmE = Origin - Extent;
		float ProdDxPy = Direction[0] * PmE[1];
		float ProdDyPx = Direction[1] * PmE[0];

		if (ProdDyPx >= ProdDxPy)
		{
			float ProdDzPx = Direction[2] * PmE[0];
			float ProdDxPz = Direction[0] * PmE[2];
			if (ProdDzPx >= ProdDxPz)
			{
				Face(0, 1, 2, Origin, Direction, PmE, Extent, SquareDistance, Parameter);
			}
			else
			{
				Face(2, 0, 1, Origin, Direction, PmE, Extent, SquareDistance, Parameter);
			}
		}
		else
		{
			float ProdDzPy = Direction[2] * PmE[1];
			float ProdDyPz = Direction[1] * PmE[2];
			if (ProdDzPy >= ProdDyPz)
			{
				Face(1, 2, 0, Origin, Direction, PmE, Extent, SquareDistance, Parameter);
			}
			else
			{
				Face(2, 0, 1, Origin, Direction, PmE, Extent, SquareDistance, Parameter);
			}
		}
	};

	float ResultSquareDistance = Zero;
	float ResultParameter = Zero;
	if (LDirection.X > Zero)
	{
		if (LDirection.Y > Zero)
		{
			if (LDirection.Z > Zero)
			{
				DoQuery3D(LS, LDirection, InExtent, ResultSquareDistance, ResultParameter);
			}
			else
			{
				DoQuery2D(0, 1, 2, LS, LDirection, InExtent, ResultSquareDistance, ResultParameter);
			}
		}
		else
		{
			if (LDirection.Y > Zero)
			{
				DoQuery2D(0, 2, 1, LS, LDirection, InExtent, ResultSquareDistance, ResultParameter);
			}
			else
			{
				DoQuery1D(0, 1, 2, LS, LDirection, InExtent, ResultSquareDistance, ResultParameter);
			}
		}
	}
	else
	{
		if (LDirection.Y > Zero)
		{
			if (LDirection.Z > Zero)
			{
				DoQuery2D(1, 2, 0, LS, LDirection, InExtent, ResultSquareDistance, ResultParameter);
			}
			else
			{
				DoQuery1D(1, 0, 2, LS, LDirection, InExtent, ResultSquareDistance, ResultParameter);
			}
		}
		else
		{
			if (LDirection.Z > Zero)
			{
				DoQuery1D(2, 0, 1, LS, LDirection, InExtent, ResultSquareDistance, ResultParameter);
			}
			else
			{
				DoQuery0D(LS, InExtent, ResultSquareDistance, ResultParameter);
			}
		}
	}

	for (int32 i = 0; i < 3; ++i)
	{
		if (Reflect[i] > 0)
		{
			LS[i] = LS[i] * -1.0f;
		}
	}

	//FVector Closest1 = InL1S + ResultParameter * (InL1E - InL1S).GetSafeNormal();
	//FVector Closest2 = InL1S;

	return FMath::Sqrt(ResultSquareDistance);
}

#pragma endregion Math



#pragma region Component
USceneComponent* UBXFunctionLibrary::GetSceneComponentBySocketName(AActor* InActor, const FName& InSocketName, bool bCheckChild)
{
	if (!InActor)
	{
		return nullptr;
	}

	if (USceneComponent* Result = InternalGetSceneComponentBySocketName(InActor, InSocketName))
	{
		return Result;
	}

	// 检查子级
	if (bCheckChild)
	{
		for (int32 i = 0; i < InActor->Children.Num(); ++i)
		{
			AActor* Child = InActor->Children[i];
			if (!IsValid(Child))
			{
				continue;
			}

			if (USceneComponent* Result = InternalGetSceneComponentBySocketName(Child, InSocketName))
			{
				return Result;
			}
		}

		TArray<AActor*> AttachedActors;
		InActor->GetAttachedActors(AttachedActors, true, true);
		for (AActor* Child : AttachedActors)
		{
			if (!IsValid(Child))
			{
				continue;
			}

			if (USceneComponent* Result = InternalGetSceneComponentBySocketName(Child, InSocketName))
			{
				if (Result->IsAttachedTo(InActor->GetRootComponent()))
				{
					return Result;
				}
			}
		}
	}
	
	return nullptr;
}

USceneComponent* UBXFunctionLibrary::GetSceneComponentByNameAndClass(AActor* InActor, const FName& InName, UClass* InClass, bool bCheckChild)
{
	if (!InActor)
	{
		return nullptr;
	}

	UClass* ActorClass = InActor->GetClass();
	if (!ActorClass)
	{
		return nullptr;
	}

	if (USceneComponent* Result = InternalGetSceneComponentByNameAndClass(InActor, InName, InClass))
	{
		return Result;
	}

	// 检查子级
	if (bCheckChild)
	{
		for (int32 i = 0; i < InActor->Children.Num(); ++i)
		{
			AActor* Child = InActor->Children[i];
			if (!IsValid(Child))
			{
				continue;
			}

			if (USceneComponent* Result = InternalGetSceneComponentByNameAndClass(Child, InName, InClass))
			{
				return Result;
			}
		}

		TArray<AActor*> AttachedActors;
		InActor->GetAttachedActors(AttachedActors, true, true);
		for (AActor* Child : AttachedActors)
		{
			if (!IsValid(Child))
			{
				continue;
			}

			if (USceneComponent* Result = InternalGetSceneComponentByNameAndClass(Child, InName, InClass))
			{
				if (Result->IsAttachedTo(InActor->GetRootComponent()))
				{
					return Result;
				}
			}
		}
	}

	return nullptr;
}

USceneComponent* UBXFunctionLibrary::InternalGetSceneComponentBySocketName(AActor* InActor, const FName& InSocketName)
{
	if (!InActor)
	{
		return nullptr;
	}

	TArray<USceneComponent*> Components;
	InActor->GetComponents<USceneComponent>(Components);
	for (USceneComponent* Component : Components)
	{
		if (Component->GetAllSocketNames().Contains(InSocketName))
		{
			return Component;
		}
	}

	return nullptr;
}

USceneComponent* UBXFunctionLibrary::InternalGetSceneComponentByNameAndClass(AActor* InActor, const FName& InName, UClass* InClass)
{
	if (!InActor)
	{
		return nullptr;
	}

	UClass* ActorClass = InActor->GetClass();
	if (!ActorClass)
	{
		return nullptr;
	}

	// 通过变量名获取
	if (FObjectProperty* Property = CastField<FObjectProperty>(ActorClass->FindPropertyByName(InName)))
	{
		if (UObject* Object = Property->GetObjectPropertyValue_InContainer(InActor))
		{
			USceneComponent* Component = Cast<USceneComponent>(Object);
			if (Component && (!InClass || Component->IsA(InClass)))
			{
				return Component;
			}
		}
	}

	// 通过实例名 或 标签获取
	TArray<USceneComponent*> Components;
	InActor->GetComponents<USceneComponent>(Components);
	for (USceneComponent* Component : Components)
	{
		if ((Component->GetFName().IsEqual(InName) || Component->ComponentHasTag(InName)) && (!InClass || Component->IsA(InClass)))
		{
			return Component;
		}
	}

	return nullptr;
}

#pragma endregion Component



#pragma region DrawDebug
#if WITH_EDITOR
void UBXFunctionLibrary::DrawDebugBoxInEditor(UObject* WorldContextObject, const FVector Center, FVector Extent, FLinearColor LineColor, const FRotator Rotation, float Duration, float Thickness)
{
	UWorld* QueryWorld = WorldContextObject->GetWorld();
	if (!QueryWorld)
		return;

#if ENABLE_DRAW_DEBUG
	if (Rotation == FRotator::ZeroRotator)
		DrawDebugBox(QueryWorld, Center, Extent, LineColor.ToFColor(true), false, Duration, SDPG_World, Thickness);
	else
		DrawDebugBox(QueryWorld, Center, Extent, Rotation.Quaternion(), LineColor.ToFColor(true), false, Duration, SDPG_World, Thickness);
#endif
}

void UBXFunctionLibrary::DrawDebugSphereInEditor(UObject* WorldContextObject, const FVector Center, float Radius, int32 Segments, FLinearColor LineColor, float Duration, float Thickness)
{
	UWorld* QueryWorld = WorldContextObject->GetWorld();
	if (!QueryWorld)
		return;

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(QueryWorld, Center, Radius, Segments, LineColor.ToFColor(true), false, Duration, SDPG_World, Thickness);
#endif
}

void UBXFunctionLibrary::DrawDebugSweepBoxInEditor(const UWorld* InWorld, FVector const& Start, FVector const& End, FRotator const& Orientation, FVector const& HalfSize, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority)
{
#if ENABLE_DRAW_DEBUG
	FVector const TraceVec = End - Start;
	float const Dist = TraceVec.Size();

	FVector const Center = Start + TraceVec * 0.5f;

	FQuat const CapsuleRot = Orientation.Quaternion();
	DrawDebugBox(InWorld, Start, HalfSize, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);

	//now draw lines from vertices
	FVector Vertices[8];
	Vertices[0] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, -HalfSize.Y, -HalfSize.Z));	//flt
	Vertices[1] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, HalfSize.Y, -HalfSize.Z));	//frt
	Vertices[2] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, -HalfSize.Y, HalfSize.Z));	//flb
	Vertices[3] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, HalfSize.Y, HalfSize.Z));	//frb
	Vertices[4] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, -HalfSize.Y, -HalfSize.Z));	//blt
	Vertices[5] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, HalfSize.Y, -HalfSize.Z));	//brt
	Vertices[6] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, -HalfSize.Y, HalfSize.Z));	//blb
	Vertices[7] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, HalfSize.Y, HalfSize.Z));		//brb
	for (int32 VertexIdx = 0; VertexIdx < 8; ++VertexIdx)
	{
		DrawDebugLine(InWorld, Vertices[VertexIdx], Vertices[VertexIdx] + TraceVec, Color, bPersistentLines, LifeTime, DepthPriority);
	}

	DrawDebugBox(InWorld, End, HalfSize, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);
#endif
}

void UBXFunctionLibrary::DrawDebugSweepSphereInEditor(const UWorld* InWorld, FVector const& Start, FVector const& End, float Radius, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority)
{
#if ENABLE_DRAW_DEBUG
	FVector const TraceVec = End - Start;
	float const Dist = TraceVec.Size();

	FVector const Center = Start + TraceVec * 0.5f;
	float const HalfHeight = (Dist * 0.5f) + Radius;

	FQuat const CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	DrawDebugCapsule(InWorld, Center, HalfHeight, Radius, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);
#endif
}

void UBXFunctionLibrary::DrawDebugSweepCapsuleInEditor(const UWorld* InWorld, FVector const& Start, FVector const& End, float HalfHeight, float Radius, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority)
{
#if ENABLE_DRAW_DEBUG
	DrawDebugCapsule(InWorld, Start, HalfHeight, Radius, FQuat::Identity, Color, bPersistentLines, LifeTime);
	DrawDebugCapsule(InWorld, End, HalfHeight, Radius, FQuat::Identity, Color, bPersistentLines, LifeTime);
	DrawDebugLine(InWorld, Start, End, Color, bPersistentLines, LifeTime);
#endif
}
#endif
#pragma endregion DrawDebug
