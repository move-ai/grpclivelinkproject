// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class UE_gRPC : ModuleRules
{
	public UE_gRPC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		if (Target.Platform == UnrealTargetPlatform.Linux)
        {
			bEnableUndefinedIdentifierWarnings = false;
			bEnableExceptions = true;
		}
		// PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI");
        // PublicDefinitions.Add("GPR_FORBID_UNREACHABLE_CODE");
        // PublicDefinitions.Add("GRPC_ALLOW_EXCEPTIONS=0");

		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
