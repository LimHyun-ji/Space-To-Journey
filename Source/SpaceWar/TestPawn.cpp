// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SpaceWarGameModeBase.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATestPawn::ATestPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	collComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = collComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(collComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("what is it"));
	springArmComp->SetupAttachment(collComp);

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Sight"));
	cameraComp->SetupAttachment(springArmComp, USpringArmComponent::SocketName);

	
}

// Called when the game starts or when spawned
void ATestPawn::BeginPlay()
{
	Super::BeginPlay();
	
	gameMode = Cast<ASpaceWarGameModeBase>(GetWorld()->GetAuthGameMode());
	MyController = Cast<APlayerController>(Controller);
	APlayerController* myController = GetWorld()->GetFirstPlayerController();
}

// Called every frame
void ATestPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ATestPawn::ReloadPress);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &ATestPawn::Interaction);

	PlayerInputComponent->BindAxis("Horizontal", this, &ATestPawn::InputHorizontal);
}

void ATestPawn::ReloadPress()
{

	
}

void ATestPawn::Interaction()
{
	gameMode->LoadNewLevel("TheSpace");
}

void ATestPawn::InputHorizontal(float value)
{
	SetActorLocation(GetActorLocation() + FVector(0,value*100,0));
}

void ATestPawn::takeController()
{

	//APlayerController* myController = GetWorld()->GetFirstPlayerController();
	
	
	
	/*	//일단 알아두면 좋을듯. 나중에 퍼즐류 때 사용가능할 수도
	if (myController->InputKey("D", EInputEvent::IE_Axis,1,true))
	{
		UE_LOG(LogTemp, Warning, TEXT("asdfasdf"));
		InputHorizontal(1);
	}
	*/

}