#include "InsanityManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/PostProcessComponent.h"
#include "Components/TimelineComponent.h"

AInsanityManager::AInsanityManager()
{
    PrimaryActorTick.bCanEverTick = false;

    MaxDistance = 1000.0f;
    InsanityIncreaseRate = 1.0f;
    InsanityDecreaseRate = 0.5f;
    InsanityThreshold = 50.0f;
    InsanityMeter = 0.0f;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    ScreenPostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("ScreenPostProcess"));
    ScreenPostProcess->SetupAttachment(RootComponent);

    // Create the timeline component
    FadeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("FadeTimeline"));
}

void AInsanityManager::BeginPlay()
{
    Super::BeginPlay();

    // Bind the CalculateInsanityMeter function to run every second
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AInsanityManager::CalculateInsanityMeter, 1.0f, true);

    APlayerController* Player1Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    APlayerController* Player2Controller = UGameplayStatics::GetPlayerController(GetWorld(), 1);

    if (Player1Controller)
    {
        ScreenPostProcess->AttachToComponent(Player1Controller->GetPawn()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }

    if (Player2Controller)
    {
        ScreenPostProcess->AttachToComponent(Player2Controller->GetPawn()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }

    // Bind the timeline functions
    if (FadeCurve)
    {
        FOnTimelineFloat TimelineCallback;
        TimelineCallback.BindUFunction(this, FName("FadeTimelineCallback"));
        FadeTimeline->AddInterpFloat(FadeCurve, TimelineCallback);
    }

    // Set the timeline properties
    FadeTimeline->SetLooping(false);
    FadeTimeline->SetIgnoreTimeDilation(true);

    // Bind the finished event of the timeline
    FOnTimelineEvent TimelineFinishedCallback;
    TimelineFinishedCallback.BindUFunction(this, FName("FadeTimelineFinished"));
    FadeTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
}

void AInsanityManager::CalculateInsanityMeter()
{
    APlayerController* Player1Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    APlayerController* Player2Controller = UGameplayStatics::GetPlayerController(GetWorld(), 1);

    if (Player1Controller && Player2Controller)
    {
        FVector Player1Location = Player1Controller->GetPawn()->GetActorLocation();
        FVector Player2Location = Player2Controller->GetPawn()->GetActorLocation();

        float Distance = FVector::Distance(Player1Location, Player2Location);

        if (Distance > MaxDistance)
        {
            InsanityMeter += InsanityIncreaseRate;
            if (InsanityMeter > InsanityThreshold && !FadeTimeline->IsPlaying())
            {
                // Start the timeline to fade the screen to gray
                FadeTimeline->PlayFromStart();
            }
        }
        else
        {
            InsanityMeter -= InsanityDecreaseRate;
            if (InsanityMeter <= InsanityThreshold && !FadeTimeline->IsReversing() && FadeTimeline->IsPlaying())
            {
                // Reverse the timeline to fade the screen back to normal
                FadeTimeline->Reverse();
            }
        }
    }
}

void AInsanityManager::FadeTimelineCallback(float Value)
{
    FPostProcessSettings& Settings = ScreenPostProcess->Settings;
    Settings.bOverride_Saturation = true;
    Settings.FilmSaturation = FMath::Lerp(1.0f, 0.5f, Value);
}

void AInsanityManager::FadeTimelineFinished()
{
    if (InsanityMeter <= InsanityThreshold)
    {
        FPostProcessSettings& Settings = ScreenPostProcess->Settings;
        Settings.bOverride_Saturation = false;
    }
}

 

 



