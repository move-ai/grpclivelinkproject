// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#include "../Public/PROTOBUFLiveLinkSource.h"

#include <grpcpp/grpcpp.h>

#include "ILiveLinkClient.h"
#include "LiveLinkTypes.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkAnimationTypes.h"

#include "Async/Async.h"
#include "Common/UdpSocketBuilder.h"
#include "HAL/RunnableThread.h"
#include "Json.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#include "Roles/LiveLinkCameraTypes.h"
#include "Roles/LiveLinkCameraRole.h"
#include "Roles/LiveLinkLightRole.h"
#include "Roles/LiveLinkLightTypes.h"
#include "Roles/LiveLinkTransformRole.h"
#include "Roles/LiveLinkTransformTypes.h"

#include "Doggo.pb.h"
//enable logging step 2
DEFINE_LOG_CATEGORY(ModuleLog)

#define LOCTEXT_NAMESPACE "PROTOBUFLiveLinkSource"

#define RECV_BUFFER_SIZE 1024 * 1024

FPROTOBUFLiveLinkSource::FPROTOBUFLiveLinkSource(FIPv4Endpoint InEndpoint)
: Stopping(false)
, Thread(nullptr)
, WaitTime(FTimespan::FromMilliseconds(100))
{
	// defaults
	DeviceEndpoint = InEndpoint;
	SourceType = LOCTEXT("PROTOBUFLiveLinkSourceType", "PROTOBUF LiveLink");
	SourceMachineName = LOCTEXT("PROTOBUFLiveLinkSourceMachineName", "localhost");
	Channel_m = grpc::CreateChannel(std::string(TCHAR_TO_UTF8(*DeviceEndpoint.ToString())), grpc::InsecureChannelCredentials());
	ConnectionState_m = Channel_m->GetState(true);
	SourceStatus = LOCTEXT("SourceStatus_Connecting", "Connecting");
	Start();
	
	UE_LOG(ModuleLog, Warning, TEXT("Hello. I am gRPC client"));	
}

FPROTOBUFLiveLinkSource::~FPROTOBUFLiveLinkSource()
{
	std::cout << "FPROTOBUFLiveLinkSource::~FPROTOBUFLiveLinkSource() Started"<<std::endl;
	Stop();
	if (Thread != nullptr)
	{
		Thread->WaitForCompletion();
		delete Thread;
		Thread = nullptr;
	}

	std::cout << "FPROTOBUFLiveLinkSource::~FPROTOBUFLiveLinkSource() Thread stopped"<<std::endl;
	if (ConnectionHandler){
		ConnectionHandler->cancel();
	}
	
	std::cout << "FPROTOBUFLiveLinkSource::~FPROTOBUFLiveLinkSource() Connection canceled"<<std::endl;
	StreamQueue.Shutdown();
	std::cout << "FPROTOBUFLiveLinkSource::~FPROTOBUFLiveLinkSource() Finished"<<std::endl;
}

void FPROTOBUFLiveLinkSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	Client = InClient;
	SourceGuid = InSourceGuid;
}


bool FPROTOBUFLiveLinkSource::IsSourceStillValid() const
{
	// Source is valid if we have a valid thread and socket
	// TODO
	bool bIsSourceValid = !Stopping && Thread != nullptr && ClientStub;
	return bIsSourceValid;
}


bool FPROTOBUFLiveLinkSource::RequestSourceShutdown()
{
	Stop();

	return true;
}
// FRunnable interface

void FPROTOBUFLiveLinkSource::Start()
{
	ThreadName = "MoveAI Mocap Receiver";
	ThreadName.AppendInt(FAsyncThreadIndex::GetNext());
	
	Thread = FRunnableThread::Create(this, *ThreadName, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

void FPROTOBUFLiveLinkSource::Stop()
{	if(!Stopping){
		Stopping = true;	
		UE_LOG(ModuleLog, Warning, TEXT("Prepare to shutdown LiveLink Source %s"), *DeviceEndpoint.ToString());
	}
}

uint32 FPROTOBUFLiveLinkSource::Run()
{
	uint nMaxTries = 50;
	if(_TryConnect(nMaxTries)){
		SourceStatus = LOCTEXT("SourceStatus_Receiving", "Receiving");
		while(!Stopping){
			void* tag = nullptr;
			bool ok = false;
			auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(500);
			if (StreamQueue.AsyncNext(&tag, &ok, deadline)) {
				if (tag && tag == static_cast<void*>(&ConnectionHandler)) {
					auto res = ConnectionHandler->onNext(ok);
					if (!res) {
						ConnectionHandler.reset();
						SourceStatus = LOCTEXT("SourceStatus_LostConnection", "Lost Connection");
						break;
					}
				}else {
					UE_LOG(ModuleLog, Warning, TEXT("Invalid tag delivered by notification queue"));
				}
			} else {
				UE_LOG(ModuleLog, Warning, TEXT("Notification queue has been shut down unexpectedly"));
				SourceStatus = LOCTEXT("SourceStatus_LostConnection", "Lost Connection");
				break;
			}
		}

		UE_LOG(ModuleLog, Warning, TEXT("LiveLink Source %s Thread exited."), *DeviceEndpoint.ToString());
		return 0;

	}else{
		SourceStatus = LOCTEXT("SourceStatus_DeviceNotFound", "Device Not Found");
		return 0;
	}
}

bool FPROTOBUFLiveLinkSource::_TryConnect(uint nMaxTries)
{
	//wait for connection to be ready
	int tryCnt = 1;
	UE_LOG(ModuleLog, Warning, TEXT("\t Trying to connect to = %s"), *DeviceEndpoint.ToString());
	while (tryCnt <= nMaxTries && !Stopping){
		UE_LOG(ModuleLog, Warning, TEXT("\t Num of tries so far = %d"), tryCnt);
		bool succeed = Channel_m->WaitForConnected(std::chrono::system_clock::now() + std::chrono::seconds(1));
		if (!succeed){
			ConnectionState_m = grpc_connectivity_state::GRPC_CHANNEL_SHUTDOWN;
		}else{
			ConnectionState_m = grpc_connectivity_state::GRPC_CHANNEL_READY;
			ClientStub = std::unique_ptr<MocapServer::Stub>(MocapServer::NewStub(Channel_m));
			ConnectionHandler.reset(new MotionStreamRequestHandler(&ConnectionHandler, ClientStub.get(), &StreamQueue));
			UE_LOG(ModuleLog, Warning, TEXT("Connected to the host %s"), *DeviceEndpoint.ToString());	
			return true;
		}
		tryCnt += 1;
	}
	return false;
}

#undef LOCTEXT_NAMESPACE
