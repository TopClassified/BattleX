using UnrealBuildTool;



public class BattleXEditor : ModuleRules
{
	public BattleXEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "BattleX/Timeline",
                "BattleX/Collision",
                "BattleX/Condition",

                "BattleXEditor",
                "BattleXEditor/CustomLayout",
                "BattleXEditor/SimpleEditor",
                "BattleXEditor/TimelineBase",
                "BattleXEditor/TimelineEditor",
            }
		);
			
		PublicDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Core",
                "CoreUObject",
            }
		);
			
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Engine",
                "InputCore",
                "SourceControl",
                "TimeManagement",
                "ApplicationCore",
                "DeveloperSettings",

                "UnrealEd",
                "ToolMenus",
                "Blutility",
                "EditorStyle",
                "GraphEditor",
                "LevelEditor",
                "AdvancedPreviewScene",
                "ComponentVisualizers",
                "InteractiveToolsFramework",

                "Slate",
				"SlateCore",
                "KismetWidgets",
                "EditorWidgets",
                "SequencerCore",
                "BlueprintGraph",
                "SequencerWidgets",

                "StructUtils",

                "BattleX",
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
