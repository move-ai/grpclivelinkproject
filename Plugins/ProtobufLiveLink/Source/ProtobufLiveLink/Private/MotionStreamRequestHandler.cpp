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
#endif
// static void MemoryBarrier() {}
// #pragma intrinsic(_InterlockedCompareExchange64)
// #define InterlockedCompareExchangeAcquire64 _InterlockedCompareExchange64
// #define InterlockedCompareExchangeRelease64 _InterlockedCompareExchange64
// #define InterlockedCompareExchangeNoFence64 _InterlockedCompareExchange64
// #define InterlockedCompareExchange64 _InterlockedCompareExchange64

#include "MotionStreamRequestHandler.h"

#include <grpc/support/log.h>

#include <iostream>


using namespace Mocap;


MotionStreamRequestHandler::MotionStreamRequestHandler(HandlerTag tag,
                                               MocapServer::Stub* stub,
                                               grpc::CompletionQueue* cq,
                                               ILiveLinkClient *client,
                                               FGuid sourceGuid)
    : tag_(tag)
    , stub_(stub)
    , cq_(cq)
    , state_(CallState::NewCall)
    , counter_(0)
    , Client_m(client)
    , SourceGuid_m(sourceGuid)
{
    UE_LOG(ModuleLog, Warning, TEXT("MotionStreamRequestHandler constructor. Guid = %s"), *SourceGuid_m.ToString());
    onNext(true);
}


bool MotionStreamRequestHandler::onNext(bool ok)
{
    try {
        if (ok) {
            if (state_ == CallState::NewCall) {
                this->handleNewCallState();
            }
            else if (state_ == CallState::SendingRequest) {
                this->handleSendingRequestState();
            }
            else if (state_ == CallState::ReceivingFile) {
                this->handleReceivingFileState();
            }
            else if (state_ == CallState::CallComplete) {
                this->handleCallCompleteState();
                return false; //TODO comment
            }
        }
        else {
            UE_LOG(ModuleLog, Warning, TEXT("UE4 Streaming failed. Close connection"));
            state_ = CallState::CallComplete;
            rpc_->Finish(&status_, tag_);
        }

        return true;
    }
    catch (std::exception& e) {
        gpr_log(GPR_ERROR, "Download processing error: %s", e.what());
    }
    catch (...) {
        gpr_log(GPR_ERROR, "Download processing error: unknown exception caught");
    }

    if (state_ == CallState::NewCall) {
        //TODO comment
        return false;
    }

    ctx_.TryCancel();

    return true;
}


void MotionStreamRequestHandler::cancel()
{
    ctx_.TryCancel();
}


void MotionStreamRequestHandler::handleNewCallState()
{
    rpc_ = stub_->PrepareAsyncGetMocapStream(&ctx_, request_, cq_);
    state_ = CallState::SendingRequest;
    rpc_->StartCall(tag_);
}


void MotionStreamRequestHandler::handleSendingRequestState()
{
    state_ = CallState::ReceivingFile;
    response_.Clear();
    rpc_->Read(&response_, tag_);
}


void MotionStreamRequestHandler::handleReceivingFileState()
{
    if(counter_ % 10000 == 0){
        UE_LOG(ModuleLog, Warning, TEXT("UE4 Received n poses = %i"), response_.poses_size());
    }
    counter_ +=1;

    _ProcessNewFrame();

    //keep waiting for incoming data
    response_.Clear();
    rpc_->Read(&response_, tag_);
}


void MotionStreamRequestHandler::handleCallCompleteState()
{
    switch (status_.error_code()) {
    case grpc::OK:
        UE_LOG(ModuleLog, Warning, TEXT("UE4 Streaming completed"));
        break;

    case grpc::CANCELLED:
        UE_LOG(ModuleLog, Warning, TEXT("UE4 Streaming Canceled"));
        break;

    default:
        FString detail(status_.error_details().c_str());
        FString msg(status_.error_message().c_str());
        UE_LOG(ModuleLog, Warning, TEXT("UE4 Streaming failed. Code = %i. Msg = %s. Detail = %s"), status_.error_code(), *msg, *detail);
        break;
    }
}



void MotionStreamRequestHandler::_ProcessNewFrame()
{
    TSet<uint32> newSubjectIds;
    // std::set<uint32> newSubjectIds;
    state_ = CallState::ReceivingFile;

    //find out new subject Ids
    int nposes = response_.poses_size();
    for(int i = 0; i < nposes; i++){
        const Mocap::Pose &pose = response_.poses(i);
        int subId = pose.subjectid();
        //this is a new subject
        if(!Subjects_m.count(subId)){
            newSubjectIds.Add(pose.subjectid());
        }
    }

    //query structure of new Subjects
    // int32 Count = FruitSet.Num();
    if(newSubjectIds.Num() > 0 && Client_m != nullptr){
        _InitializeNewSubjects(newSubjectIds);
    }

    //now update new poses for subject streams.
    for(int i =0; i < nposes; i++){
        const Mocap::Pose &pose = response_.poses(i);
        uint32 subid = pose.subjectid();
        if(Subjects_m.count(subid)){
            Subjects_m[subid]->OnNewPose(pose);
        }
    }
}

void MotionStreamRequestHandler::_InitializeNewSubjects(TSet<uint32> newSubjectIds)
{
    //query subject structure
    UE_LOG(ModuleLog, Warning, TEXT("%d new subjects are found. Query their structures"), newSubjectIds.Num());
    Mocap::StructureRequest  modelRequest;
    for (auto& Elem : newSubjectIds)
    {
        modelRequest.add_structureid(Elem);
    }
    Mocap::StructureResponse modelResponse;
    //create a new context. reuse stream ctx will cause crashing.
    grpc::ClientContext myCtx;
    myCtx.set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(10));
    
    grpc::Status status = stub_->GetStructure(&myCtx, modelRequest, &modelResponse);
    if (status.ok()){
        int nmodels = modelResponse.structures_size();
        for(int i = 0; i < nmodels; i++){
            const Mocap::Structure &model = modelResponse.structures(i);
            //got a matched structure
            if (newSubjectIds.Find(model.structureid())){
                UE_LOG(ModuleLog, Warning, TEXT("A New Subject is Tracked: %i"), model.structureid());
                std::unique_ptr<LiveLinkSubjectStream> subStream = std::make_unique<LiveLinkSubjectStream>(Client_m, SourceGuid_m);
                subStream->OnInitialized(model);
                Subjects_m[model.structureid()] = std::move(subStream);
            }else{
                //TODO: 
                //returned Structure does not match.
                UE_LOG(ModuleLog, Warning, TEXT("Returned model ID are not matched"));
            }
        }
    }else{
        //std::cout <<"Failed to query structure" << std::endl;
        UE_LOG(ModuleLog, Warning, TEXT("Failed to query structure. Code = %i. %s"), status.error_code(), *FString(status.error_message().c_str()));
    }
}
#if PLATFORM_WINDOWS
#pragma warning(pop)
#endif