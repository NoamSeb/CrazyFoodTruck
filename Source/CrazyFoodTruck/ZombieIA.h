// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieIA.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnZombieDied, class AZombieIA, Zombie, AActor, Killer);

UCLASS()

class CRAZYFOODTRUCK_API AZombieIA : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Zombie|Events")
	FOnZombieDied OnZombieDied;

	UFUNCTION(BlueprintCallable, Category = "Zombie")
	void BroadcastDeath(AActor* Killer);

	
	AZombieIA();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* MainActorToFollower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* FirstActorToFollower;
	UFUNCTION(BlueprintImplementableEvent)
	void CallRound();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ZombieSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseZombieSpeed;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY()
	bool bIsDead = false;
};
