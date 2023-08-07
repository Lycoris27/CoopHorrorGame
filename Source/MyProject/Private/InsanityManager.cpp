#include "InsanityManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/PostProcessComponent.h"
#include "Components/TimelineComponent.h"

AInsanityManager::AInsanityManager()
{
    PrimaryActorTick.bCanEverTick = false;

    MaxDistance = 180.0f;
    InsanityIncreaseRate = 1.0f;
    InsanityDecreaseRate = 0.5f;
    InsanityThreshold = 50.0f;
    InsanityMeter = 0.0f;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    ScreenPostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("ScreenPostProcess"));
    ScreenPostProcess->SetupAttachment(RootComponent);

    // Create the timeline component
   
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
    
    }

    if (Player2Controller)
    {
        
    }

    // Bind the timeline functions
    

    // Set the timeline properties


    // Bind the finished event of the timeline
    FOnTimelineEvent TimelineFinishedCallback;
    TimelineFinishedCallback.BindUFunction(this, FName("FadeTimelineFinished"));
    
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
    }
}

 

 



