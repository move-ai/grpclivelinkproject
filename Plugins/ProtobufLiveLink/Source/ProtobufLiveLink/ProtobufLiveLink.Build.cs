// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProtobufLiveLink : ModuleRules
{
	public ProtobufLiveLink(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"LiveLinkInterface",
				"LiveLinkMessageBusFramework",
				"LiveLink",
				"LiveLinkComponents",
				"Messaging",
#if UE_5_0_OR_LATER
				"LiveLinkAnimationCore",
#endif
			});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"InputCore",
				"Json",
				"JsonUtilities",
				"Networking",
				"Slate",
				"SlateCore",
				"Sockets",
				// "protobuf",
				"grpc",
				"zlib",
				"OpenSSL",
				// "cares",
				"LiveLink",
				"LiveLinkComponents",
			});
	}
}
