// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CrazyFoodTruckCameraActor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACrazyFoodTruckCameraActor::ACrazyFoodTruckCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    RootComponent = SpringArm;
    SpringArm->TargetArmLength = 1500.f;
    SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    SpringArm->bDoCollisionTest = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->FieldOfView = 70.f;
}

// Called when the game starts or when spawned
void ACrazyFoodTruckCameraActor::BeginPlay()
{
	Super::BeginPlay();

    if (APlayerController* PC0 = UGameplayStatics::GetPlayerController(this, 0))
    {
        PC0->SetViewTarget(this);
    }
}

// Called every frame
void ACrazyFoodTruckCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

