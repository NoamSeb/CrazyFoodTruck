// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MatchTimerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerSecondTick, int32, ElapsedSeconds);

UCLASS(ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class CRAZYFOODTRUCK_API UMatchTimerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMatchTimerComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
	int32 ElapsedSeconds = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool bAutoStartOnBeginPlay = true;

	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FTimerSecondTick OnTimerSecond;

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer(float FirstDelay = 1.f);

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StopTimer();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void ResetTimer();

	UFUNCTION(BlueprintPure, Category = "Timer")
	int32 GetElapsedSeconds() const { return ElapsedSeconds; }

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle TimerHandle;

	void HandleSecondTick();
};
