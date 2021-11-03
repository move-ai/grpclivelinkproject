// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#if PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning (disable : 4005)
#pragma warning (disable : 4125)
#pragma warning (disable : 4582)
#pragma warning (disable : 4583)
#pragma warning (disable : 4647)
#pragma warning (disable : 4668)
#pragma warning (disable : 4800)
#pragma warning (disable : 4946)

// static void MemoryBarrier() {}
// #pragma intrinsic(_InterlockedCompareExchange64)
// #define InterlockedCompareExchangeAcquire64 _InterlockedCompareExchange64
// #define InterlockedCompareExchangeRelease64 _InterlockedCompareExchange64
// #define InterlockedCompareExchangeNoFence64 _InterlockedCompareExchange64
// #define InterlockedCompareExchange64 _InterlockedCompareExchange64
#endif
#include "PROTOBUFLiveLinkSourceFactory.h"
#include "PROTOBUFLiveLinkSource.h"
#include "./SPROTOBUFLiveLinkSourceFactory.h"

#define LOCTEXT_NAMESPACE "PROTOBUFLiveLinkSourceFactory"

FText UPROTOBUFLiveLinkSourceFactory::GetSourceDisplayName() const
{
	return LOCTEXT("SourceDisplayName", "PROTOBUF LiveLink");
}

FText UPROTOBUFLiveLinkSourceFactory::GetSourceTooltip() const
{
	return LOCTEXT("SourceTooltip", "Creates a connection to a PROTOBUF UDP Stream");
}

TSharedPtr<SWidget> UPROTOBUFLiveLinkSourceFactory::BuildCreationPanel(FOnLiveLinkSourceCreated InOnLiveLinkSourceCreated) const
{
	return SNew(SPROTOBUFLiveLinkSourceFactory)
		.OnOkClicked(SPROTOBUFLiveLinkSourceFactory::FOnOkClicked::CreateUObject(this, &UPROTOBUFLiveLinkSourceFactory::OnOkClicked, InOnLiveLinkSourceCreated));
}

TSharedPtr<ILiveLinkSource> UPROTOBUFLiveLinkSourceFactory::CreateSource(const FString& InConnectionString) const
{
	FIPv4Endpoint DeviceEndPoint;
	if (!FIPv4Endpoint::Parse(InConnectionString, DeviceEndPoint))
	{
		return TSharedPtr<ILiveLinkSource>();
	}

	return MakeShared<FPROTOBUFLiveLinkSource>(DeviceEndPoint);
}

void UPROTOBUFLiveLinkSourceFactory::OnOkClicked(FIPv4Endpoint InEndpoint, FOnLiveLinkSourceCreated InOnLiveLinkSourceCreated) const
{
	InOnLiveLinkSourceCreated.ExecuteIfBound(MakeShared<FPROTOBUFLiveLinkSource>(InEndpoint), InEndpoint.ToString());
}

#undef LOCTEXT_NAMESPACE
#pragma warning( pop )