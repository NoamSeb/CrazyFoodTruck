
#include "MunitionDrawer.h"

AMunitionDrawer::AMunitionDrawer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMunitionDrawer::UpdateMuntionCount(int change)
{
	currentMunition += change;
}

void AMunitionDrawer::BeginPlay()
{
	Super::BeginPlay();
	if (LinkedTurretController)
	{
		LinkedTurretController->OnTypeChangedGetAmmo.AddDynamic(this, &AMunitionDrawer::UpdateValueWithTurret);
		LinkedTurretController->OnShootGetAmmo.AddDynamic(this, &AMunitionDrawer::UpdateValueWithTurret);
	}
}


void AMunitionDrawer::TurretShoot(int AmmoLeft, int AmmoMax)
{
	UpdateValueWithTurret(AmmoLeft, AmmoMax);
}

void AMunitionDrawer::UpdateValueWithTurret(int AmmoLeft, int AmmoMax)
{
	munitionMax = AmmoMax; 
	float targetOpenValue = static_cast<float>(AmmoLeft)/static_cast<float>(AmmoMax);
	float valueChange = targetOpenValue - _ActualOpenValue;
	UpdateValue(valueChange, false);
}

void AMunitionDrawer::IncrementPlayerReloading()
{
	if (LinkedTurretController)
	{
		LinkedTurretController->IncrementPlayerReloading();
	}
}

void AMunitionDrawer::DecrementPlayerReloading()
{
	if (LinkedTurretController)
	{
		LinkedTurretController->DecrementPlayerReloading();
	}
}

void AMunitionDrawer::ReceiveInputOpen(float value)
{
	//int ValueInt = FMath::RoundToInt(value);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("alue : %d"), value));
	UpdateValue(value * _OpenSpeed * GetWorld()->GetDeltaSeconds(), true);
}

void AMunitionDrawer::UpdateValue(float valueChange, bool giveAmmo)
{
	_ActualOpenValue += valueChange;
	_ActualOpenValue = FMath::Clamp(_ActualOpenValue, 0.f, 1.f);
	OnAmmoUpdate.Broadcast(_ActualOpenValue);
	if (LinkedTurretController && giveAmmo)
	{
		int Ammo = FMath::Lerp(0, munitionMax, _ActualOpenValue);
		LinkedTurretController->SetCurrentAmmo(Ammo);
	}
}

void AMunitionDrawer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}