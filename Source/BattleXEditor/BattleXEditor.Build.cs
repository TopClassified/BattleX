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
                "BattleX/State",
                "BattleX/Event",
                "BattleX/Movement",
                "BattleX/Timeline",
                "BattleX/Animation",
                "BattleX/Collision",
                "BattleX/Condition",
                "BattleX/DecisionTree",

                "BattleXEditor",
                "BattleXEditor/CustomLayout",
                "BattleXEditor/SimpleEditor",
                "BattleXEditor/TimelineBase",
                "BattleXEditor/TimelineEditor",
                "BattleXEditor/DecisionTreeEditor",
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

                "GameplayTags",
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
