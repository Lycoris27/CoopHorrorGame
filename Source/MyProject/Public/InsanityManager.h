#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InsanityManager.generated.h"

UCLASS()
class MYPROJECT_API AInsanityManager : public AActor
{
    GENERATED_BODY()

public:
    AInsanityManager();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
        float MaxDistance;

    UPROPERTY(EditAnywhere)
        float InsanityIncreaseRate;

    UPROPERTY(EditAnywhere)
        float InsanityDecreaseRate;

    UPROPERTY(EditAnywhere)
        float InsanityThreshold;

    UPROPERTY(EditAnywhere)
        class UPostProcessComponent* ScreenPostProcess;

    float InsanityMeter;

private:
    FTimerHandle TimerHandle; //Add this line to declare TimerHandle

    void CalculateInsanityMeter();
};

