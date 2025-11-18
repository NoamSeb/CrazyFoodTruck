// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputSubsystems.h"
#include "InputCharacterAmelioration/InputAmeliorationCharacters.h"
#include "PawnUpgrade.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API APawnUpgrade : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnUpgrade();

	void AddMappingContext(UInputMappingContext* InputMappingContext, int8 Priority);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
