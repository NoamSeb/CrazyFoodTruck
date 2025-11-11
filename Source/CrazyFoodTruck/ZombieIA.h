// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieIA.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AZombieIA : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
