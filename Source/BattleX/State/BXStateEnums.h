#pragma once



// 禁用行为的原因
UENUM(BlueprintType)
enum class EBXForbiddenBehaviorReason : uint8
{
	FB_OtherBehavior           = 0              UMETA(DisplayName = "由其他行为禁止"),

	FB_TMax                                     UMETA(Hidden)
};
