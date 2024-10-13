using UnrealBuildTool;



public class BattleX : ModuleRules
{
	public BattleX(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange
		(
			new string[] 
			{

			}
		);	
		
		PrivateIncludePaths.AddRange
		(
			new string[] 
			{
                "BattleX",
                "BattleX/Task",
                "BattleX/Unit",
                "BattleX/Lock",
                "BattleX/Gear",
                "BattleX/State",
                "BattleX/Movement",
                "BattleX/Timeline",
                "BattleX/Animation",
                "BattleX/Collision",
                "BattleX/Condition",
            }
		);
		
		PublicDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Core",
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "DeveloperSettings",

                "GameplayTags",
                "StructUtils",
            }
		);
		
		DynamicallyLoadedModuleNames.AddRange
		(
			new string[]
			{

			}
		);
	}
}
