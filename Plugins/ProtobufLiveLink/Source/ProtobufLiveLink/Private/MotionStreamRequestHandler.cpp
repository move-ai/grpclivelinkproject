#include "MotionStreamRequestHandler.h"

#include <grpc/support/log.h>

#include <iostream>


using namespace Mocap;


MotionStreamRequestHandler::MotionStreamRequestHandler(HandlerTag tag,
                                               MocapServer::Stub* stub,
                                               grpc::CompletionQueue* cq)
    : tag_(tag)
    , stub_(stub)
    , cq_(cq)
    , state_(CallState::NewCall)
    , counter_(0)
{
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
            else if (state_ == CallState::ExpectingHeader) {
                this->handleExpectingHeaderState();
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
    std::cout <<"handleNewCallState" << std::endl;
    rpc_ = stub_->PrepareAsyncGetMocapStream(&ctx_, request_, cq_);
    state_ = CallState::SendingRequest;
    rpc_->StartCall(tag_);
}


void MotionStreamRequestHandler::handleSendingRequestState()
{
    std::cout <<"handleSendingRequestState" << std::endl;
    state_ = CallState::ExpectingHeader;
    response_.Clear();
    rpc_->Read(&response_, tag_);
}


void MotionStreamRequestHandler::handleExpectingHeaderState()
{
    std::cout <<"handleExpectingHeaderState" << std::endl;

    int nposes = response_.poses_size();
    for(int i = 0; i < nposes; i++){
        auto &pose = response_.poses(i);
        std::cout << "\t" << pose.subjectid();
    }

    //keep waiting for incoming data
    state_ = CallState::ReceivingFile;
    response_.Clear();
    rpc_->Read(&response_, tag_);
    
    // if (response_.subjectid() >= 0) {
    //     state_ = CallState::ReceivingFile;
    //     response_.Clear();
    //     rpc_->Read(&response_, tag_);
    // }
    // else {
    //     state_ = CallState::CallComplete;
    //     rpc_->Finish(&status_, tag_);
    // }
}

void MotionStreamRequestHandler::handleReceivingFileState()
{
    if(counter_ % 10000 == 0){
        UE_LOG(ModuleLog, Warning, TEXT("UE4 Received n poses = %i"), response_.poses_size());
    }
    counter_ +=1;


    std::set<uint32> newSubjectIds;
    state_ = CallState::ReceivingFile;

    int nposes = response_.poses_size();
    for(int i = 0; i < nposes; i++){
        const Mocap::Pose &pose = response_.poses(i);
        int subId = pose.subjectid();
        //this is a new subject
        if(!Subjects_m.count(subId)){
            newSubjectIds.insert(pose.subjectid());
        }
    }

    //query subject structure
    if(!newSubjectIds.empty()){
        UE_LOG(ModuleLog, Warning, TEXT("%d new subjects are found. Query their structures"), newSubjectIds.size());
        Mocap::StructureRequest  modelRequest;
        Mocap::StructureResponse modelResponse;
        //create a new context. reuse stream ctx will cause crashing.
        grpc::ClientContext myCtx;
        myCtx.set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(2));
        
        grpc::Status status = stub_->GetStructure(&myCtx, modelRequest, &modelResponse);
        if (status.ok()){
            int nmodels = modelResponse.structures_size();
            for(int i = 0; i < nmodels; i++){
                const Mocap::Structure &model = modelResponse.structures(i);
                //got a matched structure
                if (newSubjectIds.count(model.structureid())){
                    UE_LOG(ModuleLog, Warning, TEXT("A New Subject is Tracked: %i"), model.structureid());
                    std::unique_ptr<LiveLinkSubjectStream> subStream = std::make_unique<LiveLinkSubjectStream>();
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
            UE_LOG(ModuleLog, Warning, TEXT("Failed to query structure"));
        }
    }


    //now update new poses for subject streams.
    for(int i =0; i < nposes; i++){
        const Mocap::Pose &pose = response_.poses(i);
        uint subid = pose.subjectid();
        if(Subjects_m.count(subid)){
            Subjects_m[subid]->OnNewPose(pose);
        }
    }

    response_.Clear();
    rpc_->Read(&response_, tag_);
}


void MotionStreamRequestHandler::handleCallCompleteState()
{
    std::cout <<"handleCallCompleteState" << std::endl;
    switch (status_.error_code()) {
    case grpc::OK:
        std::cout << "[ download complete: " << std::endl;
        break;

    case grpc::CANCELLED:
        std::cout << "[ download cancelled" << std::endl;
        break;

    default:
        std::cout << "[ download failed: " << status_.error_message() << std::endl;
        break;
    }
}

