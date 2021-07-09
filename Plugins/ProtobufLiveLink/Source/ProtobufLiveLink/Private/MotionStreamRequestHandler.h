#pragma once
#include "HandlerInterface.h"
#include "MocapExchange.grpc.pb.h"

#include <grpcpp/grpcpp.h>

DECLARE_LOG_CATEGORY_EXTERN(ModuleLog, Log, All)

using namespace Mocap;

namespace Mocap {

class MotionStreamRequestHandler : public HandlerInterface
{
public:
    MotionStreamRequestHandler(HandlerTag tag,
                           MocapServer::Stub* stub,
                           grpc::CompletionQueue* cq);

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
        ExpectingHeader,
        ReceivingFile,
        CallComplete
    };

    void handleNewCallState();
    void handleSendingRequestState();
    void handleExpectingHeaderState();
    void handleReceivingFileState();
    void handleCallCompleteState();

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
};



}

