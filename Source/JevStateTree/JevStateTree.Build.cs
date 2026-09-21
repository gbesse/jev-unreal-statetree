// Purpose: Declare the Unreal modules required by the runtime StateTree integration and gateway client.
using UnrealBuildTool;

public class JevStateTree : ModuleRules
{
    public JevStateTree(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "StateTreeModule", "DeveloperSettings" });
        PrivateDependencyModuleNames.AddRange(new[] { "HTTP", "Json", "JsonUtilities" });
    }
}
