#include "BXSettings.h"

#include "BXStructs.h"
#include "BXManager.h" 



UBXSettings::UBXSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FDirectoryPath DefaultPath;
	DefaultPath.Path = "/Game/BattleX/Projectile";
	ProjectileAssetPaths.Add(DefaultPath);
}
