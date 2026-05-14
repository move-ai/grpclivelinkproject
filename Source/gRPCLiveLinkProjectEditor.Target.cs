// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class gRPCLiveLinkProjectEditorTarget : TargetRules
{
    public gRPCLiveLinkProjectEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;

        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        CppStandard = CppStandardVersion.Cpp20;

        ExtraModuleNames.AddRange(new string[] { "gRPCLiveLinkProject" });
    }
}