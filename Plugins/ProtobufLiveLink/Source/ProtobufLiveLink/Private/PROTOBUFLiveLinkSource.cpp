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

	std::string host = "localhost:55555";
	auto channel = grpc::CreateChannel(host, grpc::InsecureChannelCredentials());
	auto state = channel->GetState(true);
	
	if (state != grpc_connectivity_state::GRPC_CHANNEL_READY){
		UE_LOG(ModuleLog, Warning, TEXT("Failed to connect to the host %s. Check if the host is available"), *FString(host.data()));	
		SourceStatus = LOCTEXT("SourceStatus_DeviceNotFound", "Device Not Found");

	}else{
		ClientStub = std::unique_ptr<MocapServer::Stub>(MocapServer::NewStub(channel));
		ConnectionHandler.reset(new MotionStreamRequestHandler(&ConnectionHandler, ClientStub.get(), &StreamQueue));
		UE_LOG(ModuleLog, Warning, TEXT("Connected to the host %s"), *FString(host.data()));	

		Start();
		SourceStatus = LOCTEXT("SourceStatus_Receiving", "Receiving");
	}

}

FPROTOBUFLiveLinkSource::~FPROTOBUFLiveLinkSource()
{
	Stop();
	if (Thread != nullptr)
	{
		Thread->WaitForCompletion();
		delete Thread;
		Thread = nullptr;
	}
	if (ConnectionHandler){
		ConnectionHandler->cancel();
	}
	
	StreamQueue.Shutdown();
    // drain the queue
    void* ignoredTag = nullptr;
    bool ok = false;
    while (StreamQueue.Next(&ignoredTag, &ok));

	// if (Socket != nullptr)
	// {
	// 	Socket->Close();
	// 	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
	// }
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
{
	Stopping = true;
}

uint32 FPROTOBUFLiveLinkSource::Run()
{
	void* tag = nullptr;
	bool ok = false;
	while(!Stopping){
		if (StreamQueue.Next(&tag, &ok)) {
			if (tag) {
                    //TODO assert
                    auto res = ConnectionHandler->onNext(ok);
                    if (!res) {
                        //TODO comment
                        ConnectionHandler.reset();
                        break;
                    }
                }
                else {
					UE_LOG(ModuleLog, Warning, TEXT("Invalid tag delivered by notification queue"));
                }
            }
		else {
			UE_LOG(ModuleLog, Warning, TEXT("Notification queue has been shut down unexpectedly"));
			break;
		}

		//query subject structure
		//ClientStub->RequestGetStructure(Context)

	}

	return 0;

	// TSharedRef<FInternetAddr> Sender = SocketSubsystem->CreateInternetAddr();
	
	// while (!Stopping)
	// {
	// 	if (Socket->Wait(ESocketWaitConditions::WaitForRead, WaitTime))
	// 	{
	// 		uint32 Size;

	// 		while (Socket->HasPendingData(Size))
	// 		{
	// 			int32 Read = 0;

	// 			if (Socket->RecvFrom(RecvBuffer.GetData(), RecvBuffer.Num(), Read, *Sender))
	// 			{
	// 				if (Read > 0)
	// 				{
	// 					TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> ReceivedData = MakeShareable(new TArray<uint8>());
	// 					ReceivedData->SetNumUninitialized(Read);
	// 					memcpy(ReceivedData->GetData(), RecvBuffer.GetData(), Read);
	// 					AsyncTask(ENamedThreads::GameThread, [this, ReceivedData]() { HandleReceivedData(ReceivedData); });
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	// return 0;
}

void FPROTOBUFLiveLinkSource::HandleReceivedData(TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> ReceivedData)
{
	FString ProtobufFString;
	ProtobufFString.Empty(ReceivedData->Num());
	for (uint8& Byte : *ReceivedData.Get())
	{
		ProtobufFString += TCHAR(Byte);
	}
	// UE_LOG(ModuleLog, Warning, TEXT("ProtobufFString: %s"), *ProtobufFString);

	Doggo TestDoggo;
	std::string const ProtobufString = TCHAR_TO_UTF8(*ProtobufFString);
	TestDoggo.ParseFromString(ProtobufString);
	FString nameAsFString = TestDoggo.name().c_str();
	UE_LOG(ModuleLog, Warning, TEXT("Dog name: %s"), *nameAsFString);
}

// void FPROTOBUFLiveLinkSource::HandleReceivedData(TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> ReceivedData)
// {
// 	//UE_LOG(ModuleLog, Warning, TEXT("HandleReceiveData"));
// 	FString JsonString;
// 	JsonString.Empty(ReceivedData->Num());
// 	for (uint8& Byte : *ReceivedData.Get())
// 	{
// 		JsonString += TCHAR(Byte);
// 	}

// 	//type values from the JSON
// 	FString validtypes = FString("CharacterSubject CharacterAnimation CameraSubject CameraAnimation LightSubject LightAnimation TransformSubject TransformAnimation");
	
// 	TSharedPtr<FJsonObject> JsonObject;
// 	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
// 	if (FJsonSerializer::Deserialize(Reader, JsonObject))
// 	{
// 		for (TPair<FString, TSharedPtr<FJsonValue>>& JsonField : JsonObject->Values)
// 		{
// 			FName SubjectName(*JsonField.Key);
// 			const TArray<TSharedPtr<FJsonValue>>& BoneArray = JsonField.Value->AsArray();

// 			//type of JSON data found in first value of the BoneArray
// 			//contents of BoneArray depends on the type - Character, Camera, Light, Transform
// 			const TSharedPtr<FJsonValue>& MyType = BoneArray[0];
// 			const TSharedPtr<FJsonObject> MyTypeObject = MyType->AsObject();

// 			FString MyTypeName;
// 			if (MyTypeObject->TryGetStringField(TEXT("Type"), MyTypeName))
// 			{
// 				if (validtypes.Contains(MyTypeName))
// 				{
// 					//UE_LOG(ModuleLog, Warning, TEXT("HandleReceiveData - object type: %s"), *FString(MyTypeName));
// 				}
// 				else
// 				{
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid type: %s"), *FString(MyTypeName));
// 					return;
// 				}

// 			}
// 			else
// 			{
// 				// Invalid Json Format
// 				UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid json 'Type' value not found"));
// 				return;
// 			}

// 			bool bCreateSubject = !EncounteredSubjects.Contains(SubjectName);

// 			// Camera Subject

// 			if (bCreateSubject && MyTypeName == "CameraSubject")
// 			{
// 				UE_LOG(ModuleLog, Warning, TEXT("HandleReceiveData - CameraSubject"));
// 				FLiveLinkStaticDataStruct StaticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkCameraStaticData::StaticStruct());
// 				FLiveLinkCameraStaticData& CameraData = *StaticDataStruct.Cast<FLiveLinkCameraStaticData>();

// 				const TSharedPtr<FJsonValue>& Cam = BoneArray[1];
// 				const TSharedPtr<FJsonObject> CamObject = Cam->AsObject();

// 				bool FieldOfView, AspectRatio, FocalLength, ProjectionMode;
// 				if (CamObject->TryGetBoolField(TEXT("FieldOfView"), FieldOfView))
// 				{
// 					if (FieldOfView)
// 						CameraData.bIsFieldOfViewSupported = true;
// 					else
// 						CameraData.bIsFieldOfViewSupported = false;
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid data,  FieldOfView value not found."));
// 					CameraData.bIsFieldOfViewSupported = false;
// 				}

// 				if (CamObject->TryGetBoolField(TEXT("AspectRatio"), AspectRatio))
// 				{
// 					if (AspectRatio)
// 						CameraData.bIsAspectRatioSupported = true;
// 					else
// 						CameraData.bIsAspectRatioSupported = false;
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid data, AspectRatio value not found."));
// 					CameraData.bIsAspectRatioSupported = false;
// 				}

// 				if (CamObject->TryGetBoolField(TEXT("FocalLength"), FocalLength))
// 				{
// 					if (FocalLength)
// 						CameraData.bIsFocalLengthSupported = true;
// 					else
// 						CameraData.bIsFocalLengthSupported = false;
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid data, FocalLength value not found."));
// 					CameraData.bIsFocalLengthSupported = false;
// 				}

// 				if (CamObject->TryGetBoolField(TEXT("ProjectionMode"), ProjectionMode))
// 				{
// 					if (ProjectionMode)
// 						CameraData.bIsProjectionModeSupported = true;
// 					else
// 						CameraData.bIsProjectionModeSupported = false;
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid data, ProjectionMode value not found."));
// 					CameraData.bIsProjectionModeSupported = false;
// 				}

// 				Client->PushSubjectStaticData_AnyThread({ SourceGuid, SubjectName }, ULiveLinkCameraRole::StaticClass(), MoveTemp(StaticDataStruct));
// 				EncounteredSubjects.Add(SubjectName);
// 			}

// 			//Light Subject

// 			if (bCreateSubject && MyTypeName == "LightSubject")
// 			{
// 				UE_LOG(ModuleLog, Warning, TEXT("HandleReceiveData - LightSubject"));
// 				FLiveLinkStaticDataStruct StaticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkLightStaticData::StaticStruct());
// 				FLiveLinkLightStaticData& LightData = *StaticDataStruct.Cast<FLiveLinkLightStaticData>();

// 				const TSharedPtr<FJsonValue>& Lit = BoneArray[1];
// 				const TSharedPtr<FJsonObject> LitObject = Lit->AsObject();

// 				bool Intensity, LightColor, InnerConeAngle, OuterConeAngle;

// 				if (LitObject->TryGetBoolField(TEXT("Intensity"), Intensity))
// 				{
// 					if (Intensity)
// 						LightData.bIsIntensitySupported = true;
// 					else
// 						LightData.bIsIntensitySupported = false;
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid data, Intensity value not found."));
// 					LightData.bIsIntensitySupported = false;
// 				}

// 				if (LitObject->TryGetBoolField(TEXT("LightColor"), LightColor))
// 				{
// 					if (LightColor)
// 						LightData.bIsLightColorSupported = true;
// 					else
// 						LightData.bIsLightColorSupported = false;
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid data, LightColor value not found."));
// 					LightData.bIsLightColorSupported = false;
// 				}

// 				if (LitObject->TryGetBoolField(TEXT("InnerConeAngle"), InnerConeAngle))
// 				{
// 					if (InnerConeAngle)
// 						LightData.bIsInnerConeAngleSupported = true;
// 					else
// 						LightData.bIsInnerConeAngleSupported = false;
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid data, InnerConeAngle value not found."));
// 					LightData.bIsInnerConeAngleSupported = false;
// 				}

// 				if (LitObject->TryGetBoolField(TEXT("InnerConeAngle"), OuterConeAngle))
// 				{
// 					if (OuterConeAngle)
// 						LightData.bIsOuterConeAngleSupported = true;
// 					else
// 						LightData.bIsOuterConeAngleSupported = false;
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid data, OuterConeAngle value not found"));
// 					LightData.bIsOuterConeAngleSupported = false;
// 				}

// 				Client->PushSubjectStaticData_AnyThread({ SourceGuid, SubjectName }, ULiveLinkLightRole::StaticClass(), MoveTemp(StaticDataStruct));
// 				EncounteredSubjects.Add(SubjectName);
// 			}

// 			// Transform Subject

// 			if (bCreateSubject && MyTypeName == "TransformSubject")
// 			{
// 				UE_LOG(ModuleLog, Warning, TEXT("HandleReceiveData - TransformSubject"));
// 				FLiveLinkStaticDataStruct StaticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkTransformStaticData::StaticStruct());

// 				Client->PushSubjectStaticData_AnyThread({ SourceGuid, SubjectName }, ULiveLinkTransformRole::StaticClass(), MoveTemp(StaticDataStruct));
// 				EncounteredSubjects.Add(SubjectName);
// 			}

// 			// Character Subject

// 			if (bCreateSubject && MyTypeName == "CharacterSubject")
// 			{
// 				UE_LOG(ModuleLog, Warning, TEXT("HandleReceiveData - CharacterSubject"));
// 				FLiveLinkStaticDataStruct StaticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkSkeletonStaticData::StaticStruct());
// 				FLiveLinkSkeletonStaticData& StaticData = *StaticDataStruct.Cast<FLiveLinkSkeletonStaticData>();

// 				StaticData.BoneNames.SetNumUninitialized(BoneArray.Num()-1);
// 				StaticData.BoneParents.SetNumUninitialized(BoneArray.Num()-1);

// 				for (int BoneIdx = 0; BoneIdx < BoneArray.Num()-1; ++BoneIdx)
// 				{
// 					const TSharedPtr<FJsonValue>& Bone = BoneArray[BoneIdx+1];
// 					const TSharedPtr<FJsonObject> BoneObject = Bone->AsObject();

// 					FString BoneName;
// 					if (BoneObject->TryGetStringField(TEXT("Name"), BoneName))
// 					{
// 						StaticData.BoneNames[BoneIdx] = FName(*BoneName);
// 					}
// 					else
// 					{
// 						// Invalid Json Format
// 						UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid bone name"));
// 						return;
// 					}

// 					int32 BoneParentIdx;
// 					if (BoneObject->TryGetNumberField("Parent", BoneParentIdx))
// 					{
// 						StaticData.BoneParents[BoneIdx] = BoneParentIdx;
// 					}
// 					else
// 					{
// 						// Invalid Json Format
// 						UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid bone parent index"));
// 						return;
// 					}
// 				}

// 				Client->PushSubjectStaticData_AnyThread({ SourceGuid, SubjectName }, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticDataStruct));
// 				EncounteredSubjects.Add(SubjectName);
// 			}

// 			// Transform Animation

// 			if (MyTypeName == "TransformAnimation") 
// 			{
// 				FLiveLinkFrameDataStruct FrameDataStruct = FLiveLinkFrameDataStruct(FLiveLinkTransformFrameData::StaticStruct());
// 				FLiveLinkTransformFrameData& FrameData = *FrameDataStruct.Cast<FLiveLinkTransformFrameData>();

// 				const TSharedPtr<FJsonValue>& Transform = BoneArray[1];
// 				const TSharedPtr<FJsonObject> TransformObject = Transform->AsObject();

// 				const TArray<TSharedPtr<FJsonValue>>* LocationArray;
// 				FVector TransformLocation;
// 				if (TransformObject->TryGetArrayField(TEXT("Location"), LocationArray)
// 					&& LocationArray->Num() == 3) // X, Y, Z
// 				{
// 					double X = (*LocationArray)[0]->AsNumber();
// 					double Y = (*LocationArray)[1]->AsNumber();
// 					double Z = (*LocationArray)[2]->AsNumber();
// 					TransformLocation = FVector(X, Y, Z);
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid transform location"));
// 					return;
// 				}

// 				const TArray<TSharedPtr<FJsonValue>>* RotationArray;
// 				FQuat TransformQuat;
// 				if (TransformObject->TryGetArrayField(TEXT("Rotation"), RotationArray)
// 					&& RotationArray->Num() == 4) // X, Y, Z, W
// 				{
// 					double X = (*RotationArray)[0]->AsNumber();
// 					double Y = (*RotationArray)[1]->AsNumber();
// 					double Z = (*RotationArray)[2]->AsNumber();
// 					double W = (*RotationArray)[3]->AsNumber();
// 					TransformQuat = FQuat(X, Y, Z, W);
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid transform rotation"));
// 					return;
// 				}

// 				const TArray<TSharedPtr<FJsonValue>>* ScaleArray;
// 				FVector TransformScale;
// 				if (TransformObject->TryGetArrayField(TEXT("Scale"), ScaleArray)
// 					&& ScaleArray->Num() == 3) // X, Y, Z
// 				{
// 					double X = (*ScaleArray)[0]->AsNumber();
// 					double Y = (*ScaleArray)[1]->AsNumber();
// 					double Z = (*ScaleArray)[2]->AsNumber();
// 					TransformScale = FVector(X, Y, Z);
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid transform scale"));
// 					return;
// 				}

// 				FrameData.Transform = FTransform(TransformQuat, TransformLocation, TransformScale);

// 				Client->PushSubjectFrameData_AnyThread({ SourceGuid, SubjectName }, MoveTemp(FrameDataStruct));
// 			}

// 			// Camera Animation

// 			if (MyTypeName == "CameraAnimation")
// 			{
// 				FLiveLinkFrameDataStruct FrameDataStruct = FLiveLinkFrameDataStruct(FLiveLinkCameraFrameData::StaticStruct());
// 				FLiveLinkCameraFrameData& FrameData = *FrameDataStruct.Cast<FLiveLinkCameraFrameData>();

// 				const TSharedPtr<FJsonValue>& Camera = BoneArray[1];
// 				const TSharedPtr<FJsonObject> CameraObject = Camera->AsObject();

// 				const TArray<TSharedPtr<FJsonValue>>* LocationArray;
// 				FVector CameraLocation;
// 				if (CameraObject->TryGetArrayField(TEXT("Location"), LocationArray)
// 					&& LocationArray->Num() == 3) // X, Y, Z
// 				{
// 					double X = (*LocationArray)[0]->AsNumber();
// 					double Y = (*LocationArray)[1]->AsNumber();
// 					double Z = (*LocationArray)[2]->AsNumber();
// 					CameraLocation = FVector(X, Y, Z);
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid camera location"));
// 					return;
// 				}

// 				const TArray<TSharedPtr<FJsonValue>>* RotationArray;
// 				FQuat CameraQuat;
// 				if (CameraObject->TryGetArrayField(TEXT("Rotation"), RotationArray)
// 					&& RotationArray->Num() == 4) // X, Y, Z, W
// 				{
// 					double X = (*RotationArray)[0]->AsNumber();
// 					double Y = (*RotationArray)[1]->AsNumber();
// 					double Z = (*RotationArray)[2]->AsNumber();
// 					double W = (*RotationArray)[3]->AsNumber();
// 					CameraQuat = FQuat(X, Y, Z, W);
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid camera rotation"));
// 					return;
// 				}

// 				const TArray<TSharedPtr<FJsonValue>>* ScaleArray;
// 				FVector CameraScale;
// 				if (CameraObject->TryGetArrayField(TEXT("Scale"), ScaleArray)
// 					&& ScaleArray->Num() == 3) // X, Y, Z
// 				{
// 					double X = (*ScaleArray)[0]->AsNumber();
// 					double Y = (*ScaleArray)[1]->AsNumber();
// 					double Z = (*ScaleArray)[2]->AsNumber();
// 					CameraScale = FVector(X, Y, Z);
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					//UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid camera scale"));
// 					//return;
// 					CameraScale = FVector(1, 1, 1);
// 				}

// 				FrameData.Transform = FTransform(CameraQuat, CameraLocation, CameraScale);

// 				const TArray<TSharedPtr<FJsonValue>>* CamValuesArray;
// 				if (CameraObject->TryGetArrayField(TEXT("Values"), CamValuesArray)
// 					&& CamValuesArray->Num() == 3) // X, Y, Z
// 				{
// 					double horizontalFieldOfView = (*CamValuesArray)[0]->AsNumber();
// 					double aspectRatio = (*CamValuesArray)[1]->AsNumber();
// 					double focalLength = (*CamValuesArray)[2]->AsNumber();

// 					FrameData.FieldOfView = horizontalFieldOfView;
// 					FrameData.AspectRatio = aspectRatio;
// 					FrameData.FocalLength = focalLength;
// 					//FrameData.ProjectionMode = ELiveLinkCameraProjectionMode::Perspective;
// 				}
// 				//else
// 				//{
// 				//	// Invalid Json Format
// 				//	UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid camera attributes"));
// 				//	return;
// 				//}
				

// 				Client->PushSubjectFrameData_AnyThread({ SourceGuid, SubjectName }, MoveTemp(FrameDataStruct));
// 			}

// 			// Camera Animation

// 			if (MyTypeName == "LightAnimation")
// 			{
// 				FLiveLinkFrameDataStruct FrameDataStruct = FLiveLinkFrameDataStruct(FLiveLinkLightFrameData::StaticStruct());
// 				FLiveLinkLightFrameData& FrameData = *FrameDataStruct.Cast<FLiveLinkLightFrameData>();

// 				const TSharedPtr<FJsonValue>& Light = BoneArray[1];
// 				const TSharedPtr<FJsonObject> LightObject = Light->AsObject();

// 				const TArray<TSharedPtr<FJsonValue>>* LocationArray;
// 				FVector LightLocation;
// 				if (LightObject->TryGetArrayField(TEXT("Location"), LocationArray)
// 					&& LocationArray->Num() == 3) // X, Y, Z
// 				{
// 					double X = (*LocationArray)[0]->AsNumber();
// 					double Y = (*LocationArray)[1]->AsNumber();
// 					double Z = (*LocationArray)[2]->AsNumber();
// 					LightLocation = FVector(X, Y, Z);
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid light location"));
// 					return;
// 				}

// 				const TArray<TSharedPtr<FJsonValue>>* RotationArray;
// 				FQuat LightQuat;
// 				if (LightObject->TryGetArrayField(TEXT("Rotation"), RotationArray)
// 					&& RotationArray->Num() == 4) // X, Y, Z, W
// 				{
// 					double X = (*RotationArray)[0]->AsNumber();
// 					double Y = (*RotationArray)[1]->AsNumber();
// 					double Z = (*RotationArray)[2]->AsNumber();
// 					double W = (*RotationArray)[3]->AsNumber();
// 					LightQuat = FQuat(X, Y, Z, W);
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid light rotation"));
// 					return;
// 				}

// 				const TArray<TSharedPtr<FJsonValue>>* ScaleArray;
// 				FVector LightScale;
// 				if (LightObject->TryGetArrayField(TEXT("Scale"), ScaleArray)
// 					&& ScaleArray->Num() == 3) // X, Y, Z
// 				{
// 					double X = (*ScaleArray)[0]->AsNumber();
// 					double Y = (*ScaleArray)[1]->AsNumber();
// 					double Z = (*ScaleArray)[2]->AsNumber();
// 					LightScale = FVector(X, Y, Z);
// 				}
// 				else
// 				{
// 					// Invalid Json Format
// 					//UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid light scale"));
// 					//return;
// 					LightScale = FVector(1, 1, 1);
// 				}

// 				FrameData.Transform = FTransform(LightQuat, LightLocation, LightScale);

// 				const TArray<TSharedPtr<FJsonValue>>* IntensityArray;
// 				if (LightObject->TryGetArrayField(TEXT("Intensity"), IntensityArray)
// 					&& IntensityArray->Num() == 1) // Intensity
// 				{
// 					double Intensity = (*IntensityArray)[0]->AsNumber();
// 					FrameData.Intensity = Intensity;
// 				}
// 				//else
// 				//{
// 				//	// Invalid Json Format
// 				//	UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid light intensity"));
// 				//	return;
// 				//}

// 				const TArray<TSharedPtr<FJsonValue>>* ColorArray;
// 				if (LightObject->TryGetArrayField(TEXT("LightColor"), ColorArray)
// 					&& ColorArray->Num() == 3) // Intensity
// 				{
// 					uint8 R = (*ColorArray)[0]->AsNumber();
// 					uint8 G = (*ColorArray)[1]->AsNumber();
// 					uint8 B = (*ColorArray)[2]->AsNumber();
// 					FrameData.LightColor = FColor(R,G,B,255);
// 				}
// 				//else
// 				//{
// 				//	// Invalid Json Format
// 				//	UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - invalid light color"));
// 				//	return;
// 				//}

// 				const TArray<TSharedPtr<FJsonValue>>* ConeArray;
// 				if (LightObject->TryGetArrayField(TEXT("Angle"), ConeArray)
// 					&& ConeArray->Num() == 2) // InnerConeAngle, OuterConeAngle
// 				{
// 					double InnerConeAngle = (*ConeArray)[0]->AsNumber();
// 					double OuterConeAngle = (*ConeArray)[1]->AsNumber();
// 					FrameData.InnerConeAngle = InnerConeAngle;
// 					FrameData.OuterConeAngle = OuterConeAngle;
// 				}
// 				//else
// 				//{
// 				//	// Invalid Json Format
// 				//	UE_LOG(ModuleLog, Warning, TEXT("HandleReceiveData - invalid light angle or not a spotlight"));
// 				//}

// 				Client->PushSubjectFrameData_AnyThread({ SourceGuid, SubjectName }, MoveTemp(FrameDataStruct));
// 			}

// 			// Character Animation

// 			if (MyTypeName == "CharacterAnimation") 
// 			{
// 				FLiveLinkFrameDataStruct FrameDataStruct = FLiveLinkFrameDataStruct(FLiveLinkAnimationFrameData::StaticStruct());
// 				FLiveLinkAnimationFrameData& FrameData = *FrameDataStruct.Cast<FLiveLinkAnimationFrameData>();

// 				FrameData.Transforms.SetNumUninitialized(BoneArray.Num()-1);

// 				for (int BoneIdx = 0; BoneIdx < BoneArray.Num()-1; ++BoneIdx)
// 				{
// 					const TSharedPtr<FJsonValue>& Bone = BoneArray[BoneIdx+1];
// 					const TSharedPtr<FJsonObject> BoneObject = Bone->AsObject();

// 					const TArray<TSharedPtr<FJsonValue>>* LocationArray;
// 					FVector BoneLocation;
// 					if (BoneObject->TryGetArrayField(TEXT("Location"), LocationArray)
// 						&& LocationArray->Num() == 3) // X, Y, Z
// 					{
// 						double X = (*LocationArray)[0]->AsNumber();
// 						double Y = (*LocationArray)[1]->AsNumber();
// 						double Z = (*LocationArray)[2]->AsNumber();
// 						BoneLocation = FVector(X, Y, Z);
// 					}
// 					else
// 					{
// 						// Invalid Json Format
// 						UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - index[%i] invalid location"), BoneIdx);
// 						return;
// 					}

// 					const TArray<TSharedPtr<FJsonValue>>* RotationArray;
// 					FQuat BoneQuat;
// 					if (BoneObject->TryGetArrayField(TEXT("Rotation"), RotationArray)
// 						&& RotationArray->Num() == 4) // X, Y, Z, W
// 					{
// 						double X = (*RotationArray)[0]->AsNumber();
// 						double Y = (*RotationArray)[1]->AsNumber();
// 						double Z = (*RotationArray)[2]->AsNumber();
// 						double W = (*RotationArray)[3]->AsNumber();
// 						BoneQuat = FQuat(X, Y, Z, W);
// 					}
// 					else
// 					{
// 						// Invalid Json Format
// 						UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - index[%i] invalid rotation"), BoneIdx);
// 						return;
// 					}

// 					const TArray<TSharedPtr<FJsonValue>>* ScaleArray;
// 					FVector BoneScale;
// 					if (BoneObject->TryGetArrayField(TEXT("Scale"), ScaleArray)
// 						&& ScaleArray->Num() == 3) // X, Y, Z
// 					{
// 						double X = (*ScaleArray)[0]->AsNumber();
// 						double Y = (*ScaleArray)[1]->AsNumber();
// 						double Z = (*ScaleArray)[2]->AsNumber();
// 						BoneScale = FVector(X, Y, Z);
// 					}
// 					else
// 					{
// 						// Invalid Json Format
// 						UE_LOG(ModuleLog, Error, TEXT("HandleReceiveData - index[%i] invalid scale"), BoneIdx);
// 						return;
// 					}

// 					FrameData.Transforms[BoneIdx] = FTransform(BoneQuat, BoneLocation, BoneScale);
// 				}

// 				Client->PushSubjectFrameData_AnyThread({ SourceGuid, SubjectName }, MoveTemp(FrameDataStruct));
// 			}
// 		}
// 	}
// }

#undef LOCTEXT_NAMESPACE
