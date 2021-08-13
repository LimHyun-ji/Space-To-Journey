// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player_Human.h"

// Sets default values
AVehicle::AVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	RootComponent = collComp;

	interactionAreaComp = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Component"));
	interactionAreaComp->SetupAttachment(collComp);

	GetOnPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Get On Arrow"));
	GetOnPosition->SetupAttachment(collComp);

	GetOffPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Get Off Arrow"));
	GetOffPosition->SetupAttachment(collComp);
	
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("what is it"));
	springArmComp->SetupAttachment(collComp);

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Sight"));
	cameraComp->SetupAttachment(springArmComp, USpringArmComponent::SocketName);

	//springArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	springArmComp->TargetArmLength = 400.f;
	//springArmComp->bEnableCameraLag = true;
	//springArmComp->CameraLagSpeed = 3.0f;
}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVehicle::SetController(APlayerController* inputController)
{
	MyController = inputController;
}

FTransform AVehicle::GetVehicleGetOnTransform()
{
	return GetOnPosition->GetComponentTransform();
}

void AVehicle::SetGetOnHuman(APlayer_Human* inputHuman)
{
	player_Human = inputHuman;
	isGetOnHuman = true;
}

void AVehicle::GetOffVehicle()
{
	MyController->UnPossess();
	MyController->Possess(player_Human);
	player_Human->SetUIVisible(1);
	player_Human->SetController();
	player_Human->SetIsGetOnVehicle(0);
	player_Human->SetActorLocation(GetOffPosition->GetComponentLocation());
	player_Human->SetActorRotation(GetOffPosition->GetComponentRotation() - FRotator(GetOffPosition->GetComponentRotation().Pitch,0,0) );
	isGetOnHuman = false;
}

bool AVehicle::GetIsGetOnHuman()
{
	return isGetOnHuman;
}
