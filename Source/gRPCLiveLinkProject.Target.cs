// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class gRPCLiveLinkProjectTarget : TargetRules
{
    public gRPCLiveLinkProjectTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;

        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        CppStandard = CppStandardVersion.Cpp20;

        ExtraModuleNames.AddRange(new string[] { "gRPCLiveLinkProject" });
    }
}