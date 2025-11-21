// Fill out your copyright notice in the Description page of Project Settings.


#include "InputCharacterAmelioration/InputAmeliorationCharacters.h"

#include "Characters/CrazyFoodTruckCharacter.h"
#include "GameFramework/GameSession.h"


// Sets default values for this component's properties
UInputAmeliorationCharacters::UInputAmeliorationCharacters()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UInputAmeliorationCharacters::BeginPlay()
{
	Super::BeginPlay();

	SelfRef = GetOwner();
	APlayerController* PlayerController = GetPlayerControllerFromActor(SelfRef);
	indexPlayerController = GetPlayerIndexFromPlayerController(PlayerController);
	IndexCurrentCible = indexPlayerController;

	IsValidate = false;

	CanMoveOnModule = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "IndexCurrentCible");

	justepourchangerasuprr = SelfRef;
}


// Called every frame
void UInputAmeliorationCharacters::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInputAmeliorationCharacters::SetupPlayerInput(UInputComponent* PlayerInputComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Dans setup");
	if (UEnhancedInputComponent* Eic = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "a move");
			Eic->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UInputAmeliorationCharacters::Move);
			Eic->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UInputAmeliorationCharacters::MoveInModule);
		}
		if (MoveOnSkipAction)
		{
			Eic->BindAction(MoveOnSkipAction, ETriggerEvent::Triggered, this, &UInputAmeliorationCharacters::MoveOnSkip);
		}
		if (ValidateAction)
		{
			Eic->BindAction(ValidateAction, ETriggerEvent::Triggered, this, &UInputAmeliorationCharacters::ValidInModule);
			Eic->BindAction(ValidateAction, ETriggerEvent::Triggered, this, &UInputAmeliorationCharacters::Validate);
		}
		if (RemoveValidateAction)
		{
			Eic->BindAction(RemoveValidateAction, ETriggerEvent::Triggered, this, &UInputAmeliorationCharacters::RemoveValidate);
		}
	}
}



void UInputAmeliorationCharacters::Move(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Mooove");
	if (!OnSkip && !IsValidate)
	{
		float FloatValue = Value.Get<float>();
		FloatValue = FMath::RoundToInt(FloatValue);
		indexPosition += FloatValue;
		indexPosition = (indexPosition % 4 + 4) % 4;
		UpgradeUserWidget->Move(indexPlayerController, indexPosition);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("value : %f"), FloatValue));
	}
}

void UInputAmeliorationCharacters::MoveOnSkip(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Mooove on skip");
	if (!IsValidate)
	{
		float FloatValue = Value.Get<float>();
		FloatValue = FMath::RoundToInt(FloatValue);
		if (!OnSkip && FloatValue < 0)
		{
			OnSkip = true;
			UpgradeUserWidget->MoveOnSkip(indexPlayerController, FloatValue, indexPosition);
		} else if (OnSkip && FloatValue > 0)
		{
			OnSkip = false;
			UpgradeUserWidget->MoveOnSkip(indexPlayerController, FloatValue, indexPosition);
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("value : %f"), FloatValue));
	}
}

void UInputAmeliorationCharacters::Validate(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Validation");
	if (!IsValidate)
	{
		IsValidate = true;
		CanMoveOnModule = true;
		indexPositionForModule = 0; 
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("value can move : %d\n"), CanMoveOnModule));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "coucou");
		UpgradeUserWidget->Validate(indexPlayerController, indexPosition, OnSkip);
	}
}

void UInputAmeliorationCharacters::RemoveValidate()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "RemoveValidate");
	if (IsValidate)
	{
		IsValidate = false;
		CanMoveOnModule = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("value can move : %d\n"), CanMoveOnModule));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "1");
		UpgradeUserWidget->RemoveValidate(indexPlayerController, indexPosition, OnSkip);
	}
}

void UInputAmeliorationCharacters::MoveInModule(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Mooove in module");
	if (CanMoveOnModule && indexPlayerController == 0)
	{
		float FloatValue = Value.Get<float>();
		FloatValue = FMath::RoundToInt(FloatValue);
		indexPositionForModule += FloatValue;
		indexPositionForModule = (indexPositionForModule % 2 + 2) % 2;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("value index pos : %d\n"), indexPositionForModule));

		UpgradeUserWidget->MoveModule(indexPlayerController, indexPositionForModule);
	}
}

void UInputAmeliorationCharacters::ValidInModule(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Valide Module");
	if (CanMoveOnModule && indexPlayerController == 0)
	{
		CanMoveOnModule = false;
		UpgradeUserWidget->ValidModule(indexPlayerController, indexPositionForModule);
	}
}

int32 UInputAmeliorationCharacters::GetPlayerIndexFromPlayerController(APlayerController* PlayerController) const
{
	if (!PlayerController) return -1;
	if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		return LocalPlayer->GetControllerId();
	}
	return -1;
}

APlayerController* UInputAmeliorationCharacters::GetPlayerControllerFromActor(AActor* Actor) const
{
	if (!Actor) return nullptr;
	if (APawn* Pawn = Cast<APawn>(Actor))
	{
		return Cast<APlayerController>(Pawn->GetController());
	}
	return Cast<APlayerController>(Actor);
}