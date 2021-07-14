#pragma once
#include "HandlerInterface.h"
#include "MocapExchange.grpc.pb.h"
#include "LiveLinkSubjectStream.h"

#include <grpcpp/grpcpp.h>

DECLARE_LOG_CATEGORY_EXTERN(ModuleLog, Log, All)

using namespace Mocap;

namespace Mocap {

class MotionStreamRequestHandler : public HandlerInterface
{
public:
    MotionStreamRequestHandler(HandlerTag tag,
                           MocapServer::Stub* stub,
                           grpc::CompletionQueue* cq,
                           ILiveLinkClient *client,
                           FGuid sourceGuid);

    ~MotionStreamRequestHandler() override = default;

    MotionStreamRequestHandler(const MotionStreamRequestHandler&)            = delete;
    MotionStreamRequestHandler& operator=(const MotionStreamRequestHandler&) = delete;
    MotionStreamRequestHandler(MotionStreamRequestHandler&&)                 = delete;
    MotionStreamRequestHandler& operator=(MotionStreamRequestHandler&&)      = delete;

    bool onNext(bool ok) override;

    void cancel() override;

protected:
    enum class CallState
    {
        NewCall,
        SendingRequest,
        ReceivingFile,
        CallComplete
    };

    void handleNewCallState();
    void handleSendingRequestState();
    void handleReceivingFileState();
    void handleCallCompleteState();

private:
    void _ProcessNewFrame();
    void _InitializeNewSubjects(std::set<uint> newSubjectIds);
private:
    HandlerTag              tag_;
    MocapServer::Stub* stub_;
    grpc::CompletionQueue*  cq_;
    grpc::ClientContext     ctx_;

    std::unique_ptr<grpc::ClientAsyncReader<MocapStreamResponse>> rpc_;

    MocapStreamRequest             request_;
    MocapStreamResponse            response_;
    grpc::Status                status_;

    CallState                   state_;

    size_t                      counter_;

    std::unordered_map<uint, std::unique_ptr<LiveLinkSubjectStream>> Subjects_m;

    // Our identifier in LiveLink
    ILiveLinkClient* Client_m;
	FGuid SourceGuid_m;
	
};



}

