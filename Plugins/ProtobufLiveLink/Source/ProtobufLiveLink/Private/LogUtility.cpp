#include "LogUtility.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

void LogCurrentTimeWithMilliseconds(const FString& LogMessage)
{
    FDateTime CurrentTime = FDateTime::Now();
    int32 Milliseconds = CurrentTime.GetMillisecond();

    FString TimeString = CurrentTime.ToString(TEXT("%Y-%m-%d %H:%M:%S"));

    FString FullLogMessage = FString::Printf(TEXT("%s: %s.%03d"), *LogMessage, *TimeString, Milliseconds);

    UE_LOG(LogTemp, Warning, TEXT("%s"), *FullLogMessage);
}