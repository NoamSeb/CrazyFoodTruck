
#include "MunitionDrawer.h"

#include "Sound/SoundManager.h"

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
	bIsFull = false;
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
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("alue : %d"), value));
	UpdateValue(value * _OpenSpeed * GetWorld()->GetDeltaSeconds(), true);
}

void AMunitionDrawer::UpdateValue(float valueChange, bool giveAmmo)
{
	if (bIsFull) return;
	_ActualOpenValue += valueChange;

	
	_ActualOpenValue = FMath::Clamp(_ActualOpenValue, 0.f, 1.f);
	OnAmmoUpdate.Broadcast(_ActualOpenValue);
	if (LinkedTurretController && giveAmmo)
	{
		int Ammo = FMath::Lerp(0, munitionMax, _ActualOpenValue);
		LinkedTurretController->SetCurrentAmmo(Ammo);

		if (soundTimer <= 0.f)
        {
			USoundManager* SM = USoundManager::Get();
			if(SM)SM->PlaySFX(ESfxType::ECC_Reload, this);
            soundTimer = 0.2f;
        }

		
		if (LinkedTurretController->GetAmmo() >= LinkedTurretController->GetAmmoMax())
		{
			bIsFull = true;
			OnAmmoMax.Broadcast();
		}
	}
}

void AMunitionDrawer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (soundTimer > 0.f)
    {
        soundTimer -= DeltaTime;
    }
}