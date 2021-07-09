// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
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