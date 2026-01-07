
#include "Cabestan/CabestanController.h"


ACabestanController::ACabestanController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACabestanController::BeginPlay()
{
	Super::BeginPlay();

	if (UGameInstance* GIBase = GetGameInstance())
	{
		GI = Cast<UGameInstanceCrazyFoodTruck>(GIBase);
	}
	
	TruckSubSystem = GI->GetSubsystem<UFoodTruckDataSubSystem>();

	CurrentRotationSpeed = RotationSpeedOfCabestan + TruckSubSystem->RotationSpeed;
}

void ACabestanController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerInsideCount <= 0)
	{
		if (bPlayerWasPushing)
		{
			bPlayerWasPushing = false;
			OnCabestanStopped();
		}
		return;
	}
	
	bPlayerWasPushing = true;

	float value = towardInput - backwardInput;
	if (value)
	{
		OnCabestanPushed();
	}
	else
	{
		OnCabestanStopped();
	}
	AddRotationInput(value);
	if (LinkedTurretController)
	{
		LinkedTurretController->AddRotationInput(value);
	}
}

bool ACabestanController::CanPush() const
{
	return !LinkedTurretController->MaxTurnReached();
}

bool ACabestanController::CanBring() const
{
	return !LinkedTurretController->MinTurnReached();
}

void ACabestanController::AddPlayerInside()
{
	PlayerInsideCount++;
}

void ACabestanController::RemovePlayerInside()
{
	PlayerInsideCount--;
	if (PlayerInsideCount < 0)
	{
		PlayerInsideCount = 0;
	}
}

void ACabestanController::ReceiveInputToward(float value)
{
	towardInput = value;
}

void ACabestanController::ReceiveInputBackward(float value)
{
	backwardInput = value;
}

void ACabestanController::AddRotationInput(float value)
{
	FRotator CurrentRotation = GetActorRotation();
	float turnValue = value * CurrentRotationSpeed * GetWorld()->GetDeltaSeconds();
	FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + turnValue, CurrentRotation.Roll);
	SetActorRotation(NewRotation);
}
