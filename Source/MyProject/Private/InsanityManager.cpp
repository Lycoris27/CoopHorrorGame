#include "InsanityManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/PostProcessComponent.h"

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
}

void AInsanityManager::BeginPlay()
{
    Super::BeginPlay();

    // Bind CalculateInsanityMeter to run every second
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AInsanityManager::CalculateInsanityMeter, 1.0f, true);



    // Assign post-process volume to ScreenPostProcess
    TArray<AActor*> PostProcessVolumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), UPostProcessComponent::StaticClass(), PostProcessVolumes);
    if (PostProcessVolumes.Num() > 0)
    {
        UPostProcessComponent* PostProcessVolume = Cast<UPostProcessComponent>(PostProcessVolumes[0]);
        ScreenPostProcess->BlendRadius = PostProcessVolume->BlendRadius;
        ScreenPostProcess->Settings = PostProcessVolume->Settings;
    }

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
            if (InsanityMeter > InsanityThreshold)
            {
                // Adjust the post-processing settings to make the screen go gray
                FPostProcessSettings& Settings = ScreenPostProcess->Settings;
                Settings.bOverride_FilmSlope = true;
                Settings.FilmSlope = 0.0f; // Adjust the value as needed
                Settings.bOverride_FilmToe = true;
                Settings.FilmToe = 1.0f; // Adjust the value as needed
                ScreenPostProcess->BlendWeight = 1.0f;
            }
        }
        else
        {
            InsanityMeter -= InsanityDecreaseRate;
            if (InsanityMeter <= InsanityThreshold)
            {
                // Reset the post-processing settings to default
                FPostProcessSettings& Settings = ScreenPostProcess->Settings;
                Settings.bOverride_FilmSlope = false;
                Settings.bOverride_FilmToe = false;
                ScreenPostProcess->BlendWeight = 0.0f;
            }
        }
    }
}
 
 

 



