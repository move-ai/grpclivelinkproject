#include "LiveLinkSubjectStream.h"

#include <grpc/support/log.h>

#include <iostream>


namespace Mocap
{
    
LiveLinkSubjectStream::LiveLinkSubjectStream()
{}

LiveLinkSubjectStream::~LiveLinkSubjectStream()
{}
void LiveLinkSubjectStream::OnInitialized(const Mocap::Structure &subjectStructure)
{}
void LiveLinkSubjectStream::OnNewPose(const Mocap::Pose &pose)
{}
void LiveLinkSubjectStream::OnLost()
{}
} // namespace Mocap

