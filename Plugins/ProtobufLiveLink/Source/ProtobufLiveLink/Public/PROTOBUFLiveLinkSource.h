// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include <memory>
#include <unordered_map>
#include "ILiveLinkSource.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "IMessageContext.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "../Private/MocapExchange.grpc.pb.h"
#include "../Private/MocapExchange.pb.h"
#include "../Private/MotionStreamRequestHandler.h"
#include <grpcpp/grpcpp.h>

using namespace Mocap;

class FRunnableThread;
class FSocket;
class ILiveLinkClient;
class ISocketSubsystem;

class PROTOBUFLIVELINK_API FPROTOBUFLiveLinkSource : public ILiveLinkSource, public FRunnable
{
public:

	FPROTOBUFLiveLinkSource(FIPv4Endpoint Endpoint);

	virtual ~FPROTOBUFLiveLinkSource();

	// Begin ILiveLinkSource Interface
	
	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override;

	virtual bool IsSourceStillValid() const override;

	virtual bool RequestSourceShutdown() override;

	virtual FText GetSourceType() const override { return SourceType; };
	virtual FText GetSourceMachineName() const override { return SourceMachineName; }
	virtual FText GetSourceStatus() const override { return SourceStatus; }

	// End ILiveLinkSource Interface

	// Begin FRunnable Interface

	virtual bool Init() override { return true; }
	virtual uint32 Run() override;
	void Start();
	virtual void Stop() override;
	virtual void Exit() override { }

	// End FRunnable Interface

	void HandleReceivedData(TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> ReceivedData);

private:
	enum ConnectionState{
		UNINITIALIZED = 0,
		SUCCEED = 1,
		FAILED = 2,
	};

	ILiveLinkClient* Client;

	// Our identifier in LiveLink
	FGuid SourceGuid;
	
	std::unique_ptr<MocapServer::Stub> ClientStub;
    grpc::CompletionQueue StreamQueue;
    std::unique_ptr<HandlerInterface> ConnectionHandler;
	bool ConnectionState_m = ConnectionState::UNINITIALIZED;
	
	FIPv4Endpoint DeviceEndpoint;

	FMessageAddress ConnectionAddress;

	FText SourceType;
	FText SourceMachineName;
	FText SourceStatus;

	// Threadsafe Bool for terminating the main thread loop
	FThreadSafeBool Stopping;

	// Thread to run socket operations on
	FRunnableThread* Thread;

	// Name of the sockets thread
	FString ThreadName;

	// Time to wait between attempted receives
	FTimespan WaitTime;

	// List of subjects we've already encountered
	TSet<int> EncounteredSubjects;
	TMap<uint, Mocap::Structure>  SubjectStructures;
};
