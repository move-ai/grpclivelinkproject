#include "LogUtility.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

void LogCurrentTimeWithMilliseconds(const FString& LogMessage, bool bPrintToGameWindow)
{
    FDateTime CurrentTime = FDateTime::Now();
    int32 Milliseconds = CurrentTime.GetMillisecond();

    FString TimeString = CurrentTime.ToString(TEXT("%Y-%m-%d %H:%M:%S"));

    FString FullLogMessage = FString::Printf(TEXT("%s: %s.%03d"), *LogMessage, *TimeString, Milliseconds);

    UE_LOG(LogTemp, Warning, TEXT("%s"), *FullLogMessage);

    // Print the log message on the game window if enabled
    if (bPrintToGameWindow && GEngine)
    {
        FColor Color = FColor::Yellow; // Choose the color for the debug message
        float Duration = 5.0f; // Duration in seconds for how long the message will be visible

        // Use the following line if you want to print the message at the bottom of the screen
        GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FullLogMessage);

        // Use the following line if you want to print the message at a specific location on the screen
        // FVector2D ScreenPosition(100, 100); // Specify the X and Y screen coordinates
        // GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FullLogMessage, false, ScreenPosition);
    }
}