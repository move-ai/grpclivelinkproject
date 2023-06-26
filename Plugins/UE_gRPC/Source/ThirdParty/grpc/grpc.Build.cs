using UnrealBuildTool;
using System;
using System.IO;

public class grpc : ModuleRules
{
    public grpc(ReadOnlyTargetRules Target): base(Target)
    {
        Type = ModuleType.External;

        PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));
 
        if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            PublicDefinitions.AddRange(
            new string[]
            {
                "GOOGLE_PROTOBUF_NO_RTTI=1",
                "GOOGLE_PROTOBUF_USE_UNALIGNED=0",
                "GPR_FORBID_UNREACHABLE_CODE=0"
            });

        
            PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_bad_optional_access.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_bad_variant_access.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_base.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_city.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_civil_time.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_cord.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_debugging_internal.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_demangle_internal.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_exponential_biased.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_graphcycles_internal.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_hash.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_int128.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_log_severity.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_malloc_internal.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_raw_hash_set.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_raw_logging_internal.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_spinlock_wait.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_stacktrace.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_status.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_statusor.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_str_format_internal.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_strings.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_strings_internal.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_symbolize.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_synchronization.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_throw_delegate.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_time.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_time_zone.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libabsl_wyhash.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libaddress_sorting.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libgpr.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libgrpc.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libgrpc++.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libgrpc++_alts.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libgrpc++_error_details.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libgrpc_plugin_support.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libgrpcpp_channelz.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libgrpc++_reflection.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libgrpc_unsecure.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libgrpc++_unsecure.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libre2.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libupb.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libprotobuf.a"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libcares.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win64) {
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

            PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

            PrivateDependencyModuleNames.AddRange(new string[] {  });

            PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI");
            PublicDefinitions.Add("GPR_FORBID_UNREACHABLE_CODE");
            PublicDefinitions.Add("GRPC_ALLOW_EXCEPTIONS=0");


            PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "address_sorting.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "cares.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "gpr.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "grpc_unsecure.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "grpc++_unsecure.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "libprotobuf.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "upb.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_base.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_malloc_internal.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_raw_logging_internal.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_spinlock_wait.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_throw_delegate.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_time.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_time_zone.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_graphcycles_internal.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_synchronization.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_cord.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_str_format_internal.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_strings.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_strings_internal.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_status.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_statusor.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_bad_optional_access.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_stacktrace.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_symbolize.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "absl_int128.lib"));

            AddEngineThirdPartyPrivateStaticDependencies(Target, "zlib");
        }
        else
        {
            throw new Exception("Currently " + Target.Platform.ToString() + " is unsupported");
        }

    }
}

