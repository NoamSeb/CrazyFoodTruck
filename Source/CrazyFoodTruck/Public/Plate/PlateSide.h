// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlateController.h"
#include "SidePlate.h"
#include "GameFramework/Pawn.h"
#include "Interactable/InteractBox.h"
#include "EnhancedInputComponent.h"

#include "PlateSide.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API APlateSide : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlateSide();

	UPROPERTY(EditAnywhere, Category="Plate")
	ESidePlate _Side;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* SideCabestanInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* YawAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* QuitAction;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void HandleYaw(const FInputActionValue& Value);
	void HandleQuit(const FInputActionValue& Value);

	void DropYawInput();

	UPROPERTY(EditAnywhere)
	AInteractBox* InteractBox;

	
	void PlayerInteracted(APlayerController* PlayerController);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category="Plate")
	APlateController* _PlateController = nullptr;

};
