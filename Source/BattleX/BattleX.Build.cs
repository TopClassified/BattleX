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
                "BattleX/Event",
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
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"EnhancedInput",
				"Slate",
				"SlateCore",
				"DeveloperSettings",
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
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
